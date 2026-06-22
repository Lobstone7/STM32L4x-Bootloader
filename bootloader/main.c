#include "common_include.h"
#include "gpio.h"

#define SCB_VTOR (*(volatile uint32_t*)0xE000ED08)
#define APP_BASE 0x08008000U
#define FLASH_START 0x08000000U
#define FLASH_SIZE 0x00100000U
#define FLASH_END (FLASH_START + FLASH_SIZE - 1U)
#define RAM_START 0x20000000U
#define RAM_SIZE  0x20000U
#define RAM_END (RAM_START + RAM_SIZE - 1U)

void jump_to_app(uint32_t app_msp, uint32_t app_reset);
void main();

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

    if((RAM_START <= app_msp) && (app_msp <= RAM_END) && (FLASH_START <= app_reset) && (app_reset <= FLASH_END) && (app_reset & 1) ){  //Checking whether MSP and Reset_Handler are within the valid memory regions as well enduring Reset_Handler has valid thumb bit
        if(gpio_read(GPIOC,13)){
            jump_to_app(app_msp,app_reset);
        }
    }
    
    while(1); 
}