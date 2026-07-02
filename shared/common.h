#ifndef COMMON_H
#define COMMON_H

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>

#define PERIPHERAL_BASE (0x40000000U)

#define APB1_OFFSET (0x00)
#define APB1 (PERIPHERAL_BASE + APB1_OFFSET)

#define AHB1_OFFSET (0x00020000U)
#define AHB1 (PERIPHERAL_BASE + AHB1_OFFSET)

#define AHB2_OFFSET (0x08000000U)
#define AHB2 (PERIPHERAL_BASE + AHB2_OFFSET)

#define RCC_OFFSET (0x00001000U)
#define RCC (AHB1 + RCC_OFFSET)

#define NVIC (0xE000E100)
#define NVIC_ISER0 (*(unsigned volatile int*)(NVIC + 0x00))
#define NVIC_ISER1 (*(unsigned volatile int*)(NVIC + 0x04))
#define NVIC_ISER2 (*(volatile unsigned int*)(NVIC + 0x08))

#endif