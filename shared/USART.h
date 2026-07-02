#ifndef USART_H
#define USART_H
#include "common.h"
#include "gpio.h"
typedef struct {
  volatile uint32_t CR1;
  volatile uint32_t CR2; 
  volatile uint32_t CR3; 
  volatile uint32_t BRR; 
  volatile uint32_t GTPR; 
  volatile uint32_t RTOR; 
  volatile uint32_t RQR; 
  volatile uint32_t ISR; 
  volatile uint32_t ICR; 
  volatile uint32_t RDR; 
  volatile uint32_t TDR; 
}usart;

#define RCC_APB1ENR1_OFFSET (0x58)
#define RCC_APB1ENR1 (*(volatile uint32_t*)(RCC + RCC_APB1ENR1_OFFSET))

#define USART2_OFFSET (0x4400)
#define USART2 ((usart*) (APB1 + USART2_OFFSET))

typedef void (*usartcallback) (uint8_t);

void usart_register_callback(usartcallback cb);
void usart2_init();
void usart2_write_byte(uint8_t data);
void usart2_write(uint8_t *data, uint32_t length);
uint8_t usart2_read_byte();
uint8_t usart_receive_buffer();
void usart2_read(uint8_t *buffer,uint32_t length);
void USART2_IRQHandler(void);


#endif