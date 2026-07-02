#include "USART.h"


static usartcallback usart_cb = 0;

void usart_register_callback(usartcallback cb){
	usart_cb = cb;
}

void usart2_init(){
    RCC_APB1ENR1 |= (1U << 17);

	rcc_gpioa_enable();
    
    gpio_init(GPIOA,2,2);                             //Initialize GPIO pins for RX and TX
    gpio_init(GPIOA,3,2);

	GPIOA->AFRL &= ~(0xFU << (2 * 4));                  //Clear field to enable ALt function 7 for pins 2 and 3 
    GPIOA->AFRL &= ~(0xFU << (3 * 4));

    GPIOA->AFRL |= (7U << (2 * 4));                     //ENable Alt Function 7 for pins 2 and 3
    GPIOA->AFRL |= (7U << (3 * 4));

	USART2->BRR = 694;

	USART2->CR1 |= (1U << 2);                          //Receiver Enable
    USART2->CR1 |= (1U << 3);                          //Transmitter Enable
	//USART2->CR1 |= (1U << 5);                         //RXNE enable

	USART2->CR1 |= (1U << 0);                          //USART Enable

	//NVIC_ISER1 |= (1U << 6);
   
}

void usart2_write_byte(uint8_t data){
    while(!(USART2->ISR & (1U << 7)));                  //Wait for TXE bit to be 1 i.e Wait for line to be empty
    USART2->TDR = data;                                 //Put bytes in the data register
}

void usart2_write(uint8_t *data, uint32_t length){
    while(length){
        usart2_write_byte(*data);
        data++;
        length--;
    }
}

uint8_t usart2_read_byte(){
    while(!(USART2->ISR & (1U << 5))); 
    return USART2->RDR;
}

void usart2_read(uint8_t *buffer,uint32_t length){
    while(length){
        *buffer = usart2_read_byte();
        buffer++;
        length--;
    }
}

void USART2_IRQHandler(void){
	if(USART2->ISR & (1U << 5)){
		uint8_t c = (uint8_t)(USART2->RDR);
		if(usart_cb){
			usart_cb(c);
		}
	}
}