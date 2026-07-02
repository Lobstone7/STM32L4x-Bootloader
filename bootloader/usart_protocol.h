#ifndef USART_PROTOCOL_H
#define USART_PROTOCOL_H

#include "../shared/common.h"
#include "../shared/USART.h"
#include "../shared/flash.h"
#include "../shared/bootloader_config.h"

void receive_usart_buffer(uint8_t buffer[256]);
void write_flash(uint8_t buffer[256]);
void usart_protocol(uint8_t buffer[256]);

#endif