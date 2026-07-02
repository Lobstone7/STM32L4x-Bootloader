#include "../shared/common.h"
#include "../shared/bootloader_config.h"
#include "../shared/gpio.h"
#include "../shared/USART.h"
#include "../shared/clock.h"
#include "../shared/flash.h"
#include "usart_protocol.h"
#include <string.h>

uint32_t crc32_boot(uint8_t *data,uint32_t length,uint8_t *crc_field);
void jump_to_app();
bool verify();
void info();
void main();