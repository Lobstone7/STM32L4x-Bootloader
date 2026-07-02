#include "main.h"

uint32_t crc32_boot(uint8_t *data,uint32_t length,uint8_t *crc_field){
    uint8_t byte;
    uint32_t mask;
    uint32_t crc = 0xffffffff;
    for(uint32_t i = 0; i<length;i++){
        byte = data[i];
        if(&data[i]<(crc_field+4) && &data[i]>=(crc_field) ){
            byte = 0xff;
        }
        crc ^= byte;
        for(int j = 0;j<8;j++){
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xedb88320 & mask);
        }
    }
    return ~(crc);
}

void jump_to_app(){
    uint32_t app_msp = *(uint32_t*)(APP_BASE);
    uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U); 

    __asm volatile ("cpsid i");

    __asm volatile ("msr msp, %0" : : "r" (app_msp): );

    SCB_VTOR = APP_BASE;

    __asm volatile (
    "bx %0"
    :
    : "r" (app_reset)
);
}

bool verify(){
    uint32_t app_msp = *(uint32_t*)(APP_BASE);
    uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U); 
    uint8_t *app_start = (uint8_t*)(APP_BASE);
    Firmware_Header *firmware_header = (Firmware_Header*)(APP_BASE + VECTOR_TABLE_SIZE);
    if(firmware_header->sentinel != 0xDEADC0DE){
        return false;
    }
    uint32_t storedcrc = firmware_header->crc;
    uint32_t size = firmware_header->length;

    uint8_t *crc_field = (uint8_t*)&firmware_header->crc;

    if ((size == 0) || (size > APP_SIZE))
        return false;

    if ((app_msp < RAM_START) || (app_msp >= RAM_END))
        return false;

    if ((app_reset < FLASH_START) || (app_reset >= FLASH_END))
        return false;

    if ((app_reset & 1U) == 0)
        return false;

    uint32_t calculated_crc = crc32_boot(app_start,size,crc_field);

    if((calculated_crc != storedcrc))
        return false;

    return true;
}

void info(){
    uint32_t app_msp = *(uint32_t*)(APP_BASE);
    uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U); 
    if(!(verify())){
        usart2_write((uint8_t *)"Invalid App\r\n", 13);
        return;
    }
    
    Firmware_Header *firmware_header = (Firmware_Header*)(APP_BASE + VECTOR_TABLE_SIZE);

    usart2_write((uint8_t*)firmware_header,sizeof(Firmware_Header));
    return;
}


void main(){

    SystemClock_Config();

    usart2_init();

    usart2_write((uint8_t *)"BOOT\r\n", 6); 

    rcc_gpioc_enable();
    gpio_init(GPIOC,13,1);
    gpio_pupd(GPIOC,13,1);

    volatile int i =0;
    while(i<1000000){
        i++;
    }

    uint8_t buffer[256];
    
    while(1){
        uint8_t cmd = usart2_read_byte();
        switch(cmd){
            case 'j':
            if(verify()){
                jump_to_app();
            }
            else{
                usart2_write((uint8_t *)"Invalid App\r\n", 13);
            }
            break;

            case 'i':
            info();
            break;

            case 'e':
            if(erase_flash()){
                usart2_write((uint8_t *)"Flash Erased.\r\n", 15);
            }
            else{
                usart2_write((uint8_t *)"Error.\r\n", 8);
            }
            break;

            case 'w':
            erase_flash();
            write_flash(buffer);
            if(verify()){
                usart2_write_byte('S');
            }
            else{
                usart2_write_byte('F');
            }
            break;

            case 'v':
            if(verify()){
                usart2_write((uint8_t *)"CRC Valid\r\n", 11);
            }
            else{
                usart2_write((uint8_t *)"CRC Invalid\r\n", 13);
            }
            break;

            default:
            usart2_write((uint8_t *)"Invalid\r\n", 9);
            break;
        }
    }; 
}