#include "clock.h"


#define RCC_CR          (*(volatile uint32_t *)(RCC + 0x00))
#define RCC_ICSCR       (*(volatile uint32_t *)(RCC + 0x04))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC + 0x08))
#define RCC_PLLCFGR     (*(volatile uint32_t *)(RCC + 0x0C))

#define FLASH_ACR       (*(volatile uint32_t *)(0x40022000))

void SystemClock_Config(void){
    // Set flash latency to 4 wait states for 80MHz
    FLASH_ACR |= (4U << 0);

    // Set MSI range to 4MHz (range 6)
    RCC_CR |= (1U << 3);        // MSIPLLEN
    RCC_CR = (RCC_CR & ~(0xFU << 4)) | (6U << 4);  // MSIRANGE = 6

    // Configure PLL: M=1, N=40, R=2, source=MSI
    RCC_PLLCFGR = (1U << 0)    // MSI as PLL source
                | (0U << 4)    // PLLM = 1 (value 0 = divide by 1)
                | (40U << 8)   // PLLN = 40
                | (0U << 25)   // PLLR = 2 (value 0 = divide by 2)
                | (1U << 24);  // PLLREN — enable PLLR output

    // Enable PLL
    RCC_CR |= (1U << 24);
    // Wait for PLL to lock
    while(!(RCC_CR & (1U << 25)));

    // Switch SYSCLK to PLL
    RCC_CFGR = (RCC_CFGR & ~(0x3U << 0)) | (3U << 0);
    // Wait until PLL is used
    while((RCC_CFGR & (0x3U << 2)) != (3U << 2));
}