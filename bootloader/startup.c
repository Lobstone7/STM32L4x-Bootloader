#include "common_include.h"
volatile uint32_t *VTOR = (uint32_t*)0xE000ED08;


extern uint32_t _estack;                   //Tells the compiler _estack has been declared somewhere outside file
void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));

int main(void);

uint32_t vector[] __attribute__((section(".isr_vector"))) = {              //__attribute__ is used to create separate section for vectore table so that it doesnt end up in data section
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    [2 ... 47] = (uint32_t)&Default_Handler,

};

void Reset_Handler(void){                            //extern declaration of start,end and ram copy of data as well as of start and end of bss
    extern uint32_t _sdata;
    extern uint32_t _edata; 
    extern uint32_t _sidata;

    extern uint32_t _sbss;
    extern uint32_t _ebss;

    uint32_t *_sdatap = &_sdata;
    uint32_t *_edatap = &_edata;
    uint32_t *_sidatap = &_sidata;

    uint32_t *_sbssp = &_sbss;
    uint32_t *_ebssp = &_ebss;

    while(_sdatap<_edatap){                        //Initializing data variables and zeroing bss variables
        (*_sdatap) = (*_sidatap);
        _sdatap++;
        _sidatap++;
    }

    while(_sbssp<_ebssp){
        (*_sbssp) = 0;
        _sbssp++;
    }

    main();                                        //Calling main

    while(1){                                     //Infinite loop in the event of main somehow returning
    }
}

void Default_Handler(void){
    while(1){};
}