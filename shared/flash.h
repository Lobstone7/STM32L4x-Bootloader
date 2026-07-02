#ifndef FLASH_H
#define FLASH_H
#include "common.h"
#include "bootloader_config.h"
#include <stdbool.h>

typedef struct{
    volatile uint32_t ACR;
    volatile uint32_t PDKEYR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t ECCR;
    volatile uint32_t OPTR;
    volatile uint32_t PCROP1SR;
    volatile uint32_t PCROP1ER;
    volatile uint32_t WRP1AR;
    volatile uint32_t WRP1BR;
    volatile uint32_t PCROP2SR;
    volatile uint32_t PCROP2ER;
    volatile uint32_t WRP2AR;
    volatile uint32_t WRP2BR;
}Flash;

#define RCC_AHB1ENR_OFFSET (0x48U)
#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC + RCC_AHB1ENR_OFFSET))

#define FLASH_REG_BASE 0x40022000U
#define FLASH_BASE (AHB1 + FLASH_OFFSET)
#define FLASH ((Flash*)(FLASH_REG_BASE))

#define PAGE_SIZE 2048U

#define FLASH_CR_PNB_Pos 3
#define FLASH_CR_PNB_Msk (0x7FU << FLASH_CR_PNB_Pos)

#define FLASH_CR_PG (1U << 0)
#define FLASH_CR_PER (1U << 1)
#define FLASH_CR_STRT (1U << 16)

#define FLASH_SR_EOP (1U << 0)
#define FLASH_SR_OPERR (1U << 1)
#define FLASH_SR_PROGERR (1U << 3)
#define FLASH_SR_WRPERR (1U << 4)
#define FLASH_SR_PGAERR (1U << 5)
#define FLASH_SR_SIZERR (1U << 6)
#define FLASH_SR_PGSERR (1U << 7)
#define FLASH_SR_MISERR (1U << 8)
#define FLASH_SR_FASTERR (1U << 9)
/*#define FLASH_SR_RDERR (1U << 14)
#define FLASH_SR_OPTVERR (1U << 15)*/
#define FLASH_SR_BSY (1U << 16)

void flash_init();
void unlock_flash();
void lock_flash();
bool erase_page(uint32_t page_address);
bool erase_flash();
bool program_buffer_to_flash(uint8_t *address, uint8_t buffer[256]);

#endif