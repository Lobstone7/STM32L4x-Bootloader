#include "flash.h"


void flash_init(){
    RCC_AHB1ENR |= (1U << 8);
}

void unlock_flash(){
    while(FLASH->SR & (1U << 16));
    FLASH->KEYR =  0x45670123;
    FLASH->KEYR =  0xCDEF89AB;
}

void lock_flash(){
    while(FLASH->SR & (1U << 16));
    FLASH->CR |= (1U << 31);
}

bool erase_page(uint32_t page_address){
    if(APP_BASE > page_address || APP_END <= page_address ){
        return false;
    }

    uint32_t page_number = (page_address - FLASH_START) / PAGE_SIZE;

    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->SR = FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR | FLASH_SR_MISERR | FLASH_SR_FASTERR;
    
    FLASH->CR |= FLASH_CR_PER;
    FLASH->CR &= ~(FLASH_CR_PNB_Msk);
    FLASH->CR |= (page_number << FLASH_CR_PNB_Pos);
    FLASH->CR |= FLASH_CR_STRT;

    while(FLASH->SR & FLASH_SR_BSY);

    if (FLASH->SR & (FLASH_SR_OPERR  | FLASH_SR_PROGERR | FLASH_SR_WRPERR  | FLASH_SR_PGAERR  | FLASH_SR_SIZERR  | FLASH_SR_PGSERR  | FLASH_SR_MISERR  | FLASH_SR_FASTERR)){
    FLASH->CR &= ~FLASH_CR_PER;
    return false;
    }

    if(FLASH->SR & FLASH_SR_EOP){
        FLASH->SR |= FLASH_SR_EOP;
    }

    FLASH->CR &= ~FLASH_CR_PER;
    return true;
    
}

bool erase_flash(){
    flash_init();
    unlock_flash();
    for(uint32_t i =APP_BASE;i<APP_END;i+=PAGE_SIZE){
        if(!(erase_page(i))){
            lock_flash();
            return false;
        }
    }
    lock_flash();
    return true;
}

bool program_buffer_to_flash(uint8_t *address, uint8_t buffer[256]){
    uint32_t addr = (uint32_t)address;
    if(APP_BASE > addr || APP_END - 256 < addr){
        return false;
    }

    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->SR |= FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR | FLASH_SR_MISERR | FLASH_SR_FASTERR;
    FLASH->CR |= FLASH_CR_PG;
    volatile uint32_t* flash_ptr = (volatile uint32_t*) address;

    for(int i=0;i<256;i+=8){
        while(FLASH->SR & FLASH_SR_BSY);
        uint32_t word = (uint32_t)buffer[i] | (uint32_t)buffer[i+1] << 8 | (uint32_t)buffer[i+2] << 16 | (uint32_t)buffer[i+3] << 24;
        *flash_ptr = word;
        flash_ptr++;
        word = (uint32_t)buffer[i+4] | (uint32_t)buffer[i+5] << 8 | (uint32_t)buffer[i+6] << 16 | (uint32_t)buffer[i+7] << 24;
        *flash_ptr = word;
        flash_ptr++;
        if (FLASH->SR & (FLASH_SR_OPERR  | FLASH_SR_PROGERR | FLASH_SR_WRPERR  | FLASH_SR_PGAERR  | FLASH_SR_SIZERR  | FLASH_SR_PGSERR  | FLASH_SR_MISERR  | FLASH_SR_FASTERR)){
            FLASH->CR &= ~FLASH_CR_PG;
            return false;
        }
        while(FLASH->SR & FLASH_SR_BSY);
    }

    while(FLASH->SR & FLASH_SR_BSY);


    if(FLASH->SR & FLASH_SR_EOP){
        FLASH->SR |= FLASH_SR_EOP;
    }

    FLASH->CR &= ~FLASH_CR_PG;
    return true;

}