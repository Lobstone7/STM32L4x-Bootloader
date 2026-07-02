#include "../shared/common.h"
volatile uint32_t *VTOR = (uint32_t*)0xE000ED08;


extern uint32_t _estack;                   //Tells the compiler _estack has been declared somewhere outside file
void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Debug_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WWDG_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_PVM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_TAMP_STAMP_CSS_LSE_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_CH7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1_2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM15_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM16_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM17_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_ALARM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FMC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SDMMC1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM6_DACUNDER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DFSDM1_FLT2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void COMP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LPTIM1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LPTIM2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void OTG_FS_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_CH7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LPUART1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void QUADSPI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SAI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SAI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SWPMI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TSC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void LCD_Handler(void) __attribute__((weak, alias("Default_Handler")));
void AES_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RNG_HASH_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HASH_CRS_Handler(void) __attribute__((weak, alias("Default_Handler")));

int main(void);

uint32_t vector[] __attribute__((section(".isr_vector"))) = {              //__attribute__ is used to create separate section for vectore table so that it doesnt end up in data section
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uint32_t)&SVCall_Handler,
    (uint32_t)&Debug_Handler,
    0,
    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,

    (uint32_t)&WWDG_Handler,
    (uint32_t)&PVD_PVM_Handler,
    (uint32_t)&RTC_TAMP_STAMP_CSS_LSE_Handler,
    (uint32_t)&RTC_WKUP_Handler,
    (uint32_t)&FLASH_Handler,
    (uint32_t)&RCC_Handler,
    (uint32_t)&EXTI0_Handler,
    (uint32_t)&EXTI1_Handler,
    (uint32_t)&EXTI2_Handler,
    (uint32_t)&EXTI3_Handler,
    (uint32_t)&EXTI4_Handler,
    (uint32_t)&DMA1_CH1_Handler,
    (uint32_t)&DMA1_CH2_Handler,
    (uint32_t)&DMA1_CH3_Handler,
    (uint32_t)&DMA1_CH4_Handler,
    (uint32_t)&DMA1_CH5_Handler,
    (uint32_t)&DMA1_CH6_Handler,
    (uint32_t)&DMA1_CH7_Handler,
    (uint32_t)&ADC1_2_Handler,
    (uint32_t)&CAN1_TX_Handler,
    (uint32_t)&CAN1_RX0_Handler,
    (uint32_t)&CAN1_RX1_Handler,
    (uint32_t)&CAN1_SCE_Handler,
    (uint32_t)&EXTI9_5_Handler,
    (uint32_t)&TIM1_BRK_TIM15_Handler,
    (uint32_t)&TIM1_UP_TIM16_Handler,
    (uint32_t)&TIM1_TRG_COM_TIM17_Handler,
    (uint32_t)&TIM1_CC_Handler,
    (uint32_t)&TIM2_Handler,
    (uint32_t)&TIM3_Handler,
    (uint32_t)&TIM4_Handler,
    (uint32_t)&I2C1_EV_Handler,
    (uint32_t)&I2C1_ER_Handler,
    (uint32_t)&I2C2_EV_Handler,
    (uint32_t)&I2C2_ER_Handler,
    (uint32_t)&SPI1_Handler,
    (uint32_t)&SPI2_Handler,
    (uint32_t)&USART1_Handler,
    (uint32_t)&USART2_Handler,
    (uint32_t)&USART3_Handler,
    (uint32_t)&EXTI15_10_Handler,
    (uint32_t)&RTC_ALARM_Handler,
    (uint32_t)&DFSDM1_FLT3_Handler,
    (uint32_t)&TIM8_BRK_Handler,
    (uint32_t)&TIM8_UP_Handler,
    (uint32_t)&TIM8_TRG_COM_Handler,
    (uint32_t)&TIM8_CC_Handler,
    (uint32_t)&ADC3_Handler,
    (uint32_t)&FMC_Handler,
    (uint32_t)&SDMMC1_Handler,
    (uint32_t)&TIM5_Handler,
    (uint32_t)&SPI3_Handler,
    (uint32_t)&UART4_Handler,
    (uint32_t)&UART5_Handler,
    (uint32_t)&TIM6_DACUNDER_Handler,
    (uint32_t)&TIM7_Handler,
    (uint32_t)&DMA2_CH1_Handler,
    (uint32_t)&DMA2_CH2_Handler,
    (uint32_t)&DMA2_CH3_Handler,
    (uint32_t)&DMA2_CH4_Handler,
    (uint32_t)&DMA2_CH5_Handler,
    (uint32_t)&DFSDM1_FLT0_Handler,
    (uint32_t)&DFSDM1_FLT1_Handler,
    (uint32_t)&DFSDM1_FLT2_Handler,
    (uint32_t)&COMP_Handler,
    (uint32_t)&LPTIM1_Handler,
    (uint32_t)&LPTIM2_Handler,
    (uint32_t)&OTG_FS_Handler,
    (uint32_t)&DMA2_CH6_Handler,
    (uint32_t)&DMA2_CH7_Handler,
    (uint32_t)&LPUART1_Handler,
    (uint32_t)&QUADSPI_Handler,
    (uint32_t)&I2C3_EV_Handler,
    (uint32_t)&I2C3_ER_Handler,
    (uint32_t)&SAI1_Handler,
    (uint32_t)&SAI2_Handler,
    (uint32_t)&SWPMI1_Handler,
    (uint32_t)&TSC_Handler,
    (uint32_t)&LCD_Handler,
    (uint32_t)&AES_Handler,
    (uint32_t)&RNG_HASH_Handler,
    (uint32_t)&FPU_Handler,
    (uint32_t)&HASH_CRS_Handler

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