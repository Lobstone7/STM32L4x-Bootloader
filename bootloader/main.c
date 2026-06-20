#include "common_include.h"
#define SCB_VTOR (*(volatile uint32_t*)0xE000ED08)
#define APP_BASE 0x08008000U

void jump_to_app(){
    uint32_t app_msp = *(uint32_t*)(APP_BASE);
    uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U);

    void (*app_entry) (void) = (void (*) (void))app_reset;

    __asm volatile ("cpsid i");

    SCB_VTOR = APP_BASE;

    __asm volatile ("msr msp, %0" : : "r" (app_msp): );

    __asm volatile (
    "bx %0"
    :
    : "r" (app_reset)
);
}

void main(){
    jump_to_app();
}