#include "common_include.h"
#include "gpio.h"

#define SCB_VTOR (*(volatile uint32_t*)0xE000ED08)
#define APP_BASE 0x08008000U
#define FLASH_START 0x08000000U
#define FLASH_SIZE 0x00100000U
#define FLASH_END (FLASH_START + FLASH_SIZE - 1U)
#define RAM_START 0x20000000U
#define RAM_SIZE  0x20000U
#define RAM_END (RAM_START + RAM_SIZE - 1U)
#define VECTOR_TABLE_SIZE (0x18C)
#define APP_SIZE      (32U * 1024U)


typedef struct{
    uint32_t sentinel;
    uint32_t device_id;
    uint32_t version;
    uint32_t length;
    uint32_t crc;
}Firmware_Header;

uint32_t crc32_boot(uint8_t *data,uint32_t length,uint8_t *crc_field);
void jump_to_app(uint32_t app_msp, uint32_t app_reset);
void main();