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

void jump_to_app(uint32_t app_msp, uint32_t app_reset){

    __asm volatile ("cpsid i");

    __asm volatile ("msr msp, %0" : : "r" (app_msp): );

    SCB_VTOR = APP_BASE;

    __asm volatile (
    "bx %0"
    :
    : "r" (app_reset)
);
}

void main(){
    uint32_t app_msp = *(uint32_t*)(APP_BASE);
    uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U);   

    rcc_gpioc_enable();
    gpio_init(GPIOC,13,0x00);
    gpio_pupd(GPIOC,13,0x01);

    volatile int i =0;
    while(i<1000000){
        i++;
    }


    uint8_t *app_start = (uint8_t*)(APP_BASE);
    Firmware_Header *firmware_header = (Firmware_Header*)(APP_BASE + VECTOR_TABLE_SIZE);
    uint32_t storedcrc = firmware_header->crc;
    uint32_t size = firmware_header->length;

    uint8_t *crc_field = (uint8_t*)&firmware_header->crc;

    uint32_t calculated_crc = crc32_boot(app_start,size,crc_field);

    if((RAM_START <= app_msp) && (app_msp <= RAM_END) && (FLASH_START <= app_reset) && (app_reset <= FLASH_END) && (app_reset & 1) && (storedcrc == calculated_crc) && ((size > 0 )) && (size <= APP_SIZE) ){  //Checking whether MSP and Reset_Handler are within the valid memory regions as well enduring Reset_Handler has valid thumb bit
        if(gpio_read(GPIOC,13)){
            jump_to_app(app_msp,app_reset);
        }
    }
    
    while(1); 
}