#include "gpio.h"

//Enables Clock for GPIO A, B or C.
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
    port->PUPDR |= (mode << (pin * 2));                                                     //Set GPIO pin pullup or pulldown.
}

void gpio_init(GPIO_Typedef *port, uint32_t pin, uint32_t mode){
    port->MODER &= ~(3U << (pin * 2));                                                      //Set whether input, output, alternate function or analog.
    port->MODER |= (mode << (pin * 2));
}

void gpio_write(GPIO_Typedef *port, uint32_t pin, uint32_t value){
    if(value){                                                                             //Set the GPIO bit.
        port->BSRR |= (1U << pin);
    }
    else{
        port->BSRR |= (1U << (pin + 16));
    }
}

uint32_t gpio_read(GPIO_Typedef *port, uint32_t pin){                                       //Return GPIO bit.
    return (port->IDR >> pin) & 1;
}

