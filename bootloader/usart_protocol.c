#include "usart_protocol.h"

void receive_usart_buffer(uint8_t buffer[256]){
    for(int i = 0;i<256;i++){
        buffer[i] = usart2_read_byte();
    }
}


void write_flash(uint8_t buffer[256]){
    flash_init();
    unlock_flash();
    usart_protocol(buffer);
    lock_flash();
}

void usart_protocol(uint8_t buffer[256]){
    usart2_write_byte('R');
    uint8_t *app_address = (uint8_t*) (APP_BASE);
    int retry_count = 0;
    uint32_t bytes_received = 0;
    uint32_t header_bytes = VECTOR_TABLE_SIZE + sizeof(Firmware_Header);
    while(bytes_received < header_bytes){
        receive_usart_buffer(buffer);
        if(program_buffer_to_flash(app_address + bytes_received,buffer)){
            usart2_write_byte('A');
            bytes_received +=256;
            retry_count = 0;
        }
        else{
            if(++retry_count>=5){
                usart2_write_byte('E');
                return;
            } 
            usart2_write_byte('F'); 
        }
    }

    Firmware_Header *firmware_header = (Firmware_Header*) (APP_BASE + VECTOR_TABLE_SIZE);
    uint32_t firmware_length = firmware_header->length;

    if(firmware_length < (VECTOR_TABLE_SIZE + sizeof(Firmware_Header))){
        usart2_write_byte('F');
        return;
    }

    if(firmware_length > APP_SIZE){
        usart2_write_byte('F');
        return;
    }

    uint8_t *app_address_continued = app_address + bytes_received;
    uint32_t remaining_app_size = ((uint32_t)firmware_length - bytes_received);
    bytes_received = 0;
    
    while(bytes_received < remaining_app_size){
        receive_usart_buffer(buffer);
        if(program_buffer_to_flash(app_address_continued + bytes_received,buffer)){
            usart2_write_byte('A');
            bytes_received +=256;
            retry_count = 0;
        }
        else{
            if(++retry_count>=5){
                usart2_write_byte('E');
                return;
            } 
            usart2_write_byte('F'); 
        }
    }

    uint32_t app_msp = *(uint32_t*)(APP_BASE);
    uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U); 

}