## Overview

This project implements a **minimal custom bootloader** for STM32 with support for:

* UART-based firmware updates
* AES encryption (planned)
* CRC integrity verification (planned)

The goal is to deeply understand **low-level MCU startup, memory layout, and control transfer** between bootloader and application.

---

## Boot Process Flow

1. **Reset occurs**
2. MCU loads:

   * MSP from bootloader vector table
   * Reset Handler address
3. Bootloader Reset Handler executes:

   * Initializes `.data` and `.bss`
   * Calls `main()`
4. Bootloader performs validation (future: CRC/AES)
5. Bootloader jumps to application:

   * Loads application's MSP
   * Sets VTOR to application vector table
   * Jumps to application's Reset Handler

---

## Linker Script Design

Each image (bootloader + application) has its own linker script.

### Key Sections

* `.isr_vector` → Interrupt vector table
* `.text` → Code + read-only data
* `.data` → Initialized variables (Flash → RAM)
* `.bss` → Zero-initialized variables

### Memory Layout

* Bootloader Flash: **32 KB**
* Application Flash: remaining region
* RAM defined based on MCU (96 KB)

### Important Symbols

* `_estack` → Top of stack
* `_sidata` → `.data` load address (Flash)
* `_sdata`, `_edata` → `.data` in RAM
* `_sbss`, `_ebss` → `.bss` in RAM

### Address Concepts

* **LMA (Load Memory Address)** → Flash location
* **VMA (Virtual Memory Address)** → Execution location (RAM)

---

## Startup Code

The Reset Handler performs:

1. Copy `.data` from Flash to RAM
2. Zero initialize `.bss`
3. Call `main()`
4. Enter infinite loop if `main()` returns

Vector table is defined in `.isr_vector` and includes:

* Initial MSP
* Reset Handler
* Interrupt handlers (weakly linked to `Default_Handler`)

---

## Bootloader → Application Jump

### Extract Application Entry

```c
uint32_t app_msp   = *(uint32_t*)(APP_BASE);
uint32_t app_reset = *(uint32_t*)(APP_BASE + 4U);
```

### Jump Procedure

1. Disable interrupts
2. Set VTOR to application base
3. Set MSP to application stack
4. Jump to application reset handler

```c
typedef void (*app_entry_t)(void);
app_entry_t app_entry = (app_entry_t)app_reset;

__disable_irq();
SCB->VTOR = APP_BASE;
__set_MSP(app_msp);

app_entry();
```

## Current Status

* [x] Custom linker scripts (bootloader + app)
* [x] Startup code implementation
* [x] Bootloader → application jump
* [ ] UART firmware update
* [ ] CRC validation
* [ ] AES decryption

---

## Future Work

* Implement UART protocol for firmware flashing
* Add CRC verification before jump
* Integrate AES decryption for secure firmware
* Add rollback / fail-safe mechanism


