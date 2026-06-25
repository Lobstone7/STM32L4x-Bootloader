#include "gpio.h"

void rcc_gpioa_enable(){
    RCC_AHB2ENR |= (1U << 0);
}

void rcc_gpiob_enable(){
    RCC_AHB2ENR |= (1U << 1);
}

void rcc_gpioc_enable(){
    RCC_AHB2ENR |= (1U << 2);
}

void gpio_pupd(GPIO_Typedef *port, uint32_t pin, uint32_t mode){
    port->PUPDR |= (mode << (pin * 2));
}

void gpio_init(GPIO_Typedef *port, uint32_t pin, uint32_t mode){
    port->MODER &= ~(3U << (pin * 2));
    port->MODER |= (mode << (pin * 2));
}

void gpio_write(GPIO_Typedef *port, uint32_t pin, uint32_t value){
    if(value){
        port->BSRR |= (1U << pin);
    }
    else{
        port->BSRR |= (1U << (pin + 16));
    }
}

uint32_t gpio_read(GPIO_Typedef *port, uint32_t pin){
    return (port->IDR >> pin) & 1;
}

