#ifndef GPIO_H
#define GPIO_H
#include "common.h"
#define PERIPHERAL_BASE (0x40000000U)

#define AHB1_OFFSET (0x00020000U)
#define AHB1 (PERIPHERAL_BASE + AHB1_OFFSET)

#define AHB2_OFFSET (0x08000000U)
#define AHB2 (PERIPHERAL_BASE + AHB2_OFFSET)

#define RCC_OFFSET (0x00001000U)
#define RCC (AHB1 + RCC_OFFSET)

#define RCC_AHB2ENR_OFFSET (0x4C)
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC + RCC_AHB2ENR_OFFSET))

#define GPIOA_BASE (AHB2)
#define GPIOB_BASE (AHB2 + 0x400U)
#define GPIOC_BASE (AHB2 + 0x800U)

typedef struct{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
    volatile uint32_t BRR;
    volatile uint32_t ASCR;
}GPIO_Typedef;

#define GPIOA ((GPIO_Typedef*)GPIOA_BASE)
#define GPIOB ((GPIO_Typedef*)GPIOB_BASE)
#define GPIOC ((GPIO_Typedef*)GPIOC_BASE)

void gpio_init(GPIO_Typedef *port, uint32_t pin, uint32_t mode);
void gpio_write(GPIO_Typedef *port, uint32_t pin, uint32_t value);
uint32_t gpio_read(GPIO_Typedef *port, uint32_t pin);
void rcc_gpioa_enable();
void rcc_gpiob_enable();
void rcc_gpioc_enable();
void gpio_pupd(GPIO_Typedef *port, uint32_t pin, uint32_t mode);

#endif



