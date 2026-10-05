# STM32 UART Bootloader

A minimal STM32L476RG bootloader for receiving, programming, and verifying application firmware over UART.

The bootloader currently supports:

- Separate bootloader and application memory regions
- Custom linker scripts and startup code
- Application handoff using the application's vector table
- UART-based firmware transfer
- 256-byte firmware packets
- ACK/NACK-based packet handling with retries
- STM32 Flash erase and programming
- CRC32 firmware integrity verification
- Firmware metadata stored in a custom header
- GDB-based debugging during development

> **Current version:** V1  
> **MCU:** STM32L476RG  
> **Communication:** USART  
> **Firmware verification:** CRC32

---

## Table of Contents

- [Overview](#overview)
- [Memory Layout](#memory-layout)
- [Boot Flow](#boot-flow)
- [Linker Scripts](#linker-scripts)
- [Startup Code](#startup-code)
- [Application Jump](#application-jump)
- [Firmware Verification](#firmware-verification)
- [UART Firmware Update](#uart-firmware-update)
- [Flash Programming](#flash-programming)
- [Debugging](#debugging)
- [Project Structure](#project-structure)
- [Future Work](#future-work)

---

## Overview

The bootloader occupies the first 32 KB of the STM32L476RG Flash and is responsible for deciding whether to remain in bootloader mode or launch the application.

When a firmware update is requested, the bootloader:

1. Receives the firmware over UART.
2. Receives and validates the firmware header.
3. Erases the application Flash region.
4. Programs the received firmware into Flash.
5. Verifies the complete image using CRC32.
6. Launches the application if verification succeeds.

The application is built as a separate firmware image with its own vector table, startup code, and linker script.

---

## Memory Layout

The bootloader and application are linked to separate Flash regions.

```text
STM32L476RG FLASH

0x0800 0000
┌─────────────────────────────────────┐
│                                     │
│             BOOTLOADER              │
│                                     │
│             32 KB                   │
│                                     │
│  .isr_vector                        │
│  .text                              │
│  .rodata                            │
│                                     │
└─────────────────────────────────────┘
0x0800 8000
┌─────────────────────────────────────┐
│                                     │
│             APPLICATION             │
│                                     │
│             32 KB                   │
│                                     │
│  .isr_vector                        │
│  .firmware_header                   │
│  .text                              │
│  .rodata                            │
│                                     │
└─────────────────────────────────────┘
0x0801 0000

RAM
0x2000 0000
┌─────────────────────────────────────┐
│                                     │
│       Bootloader / Application      │
│              RAM                    │
│                                     │
│              96 KB                  │
│                                     │
└─────────────────────────────────────┘
```

### Flash regions

| Image | Start Address | Allocated Size |
|---|---:|---:|
| Bootloader | `0x08000000` | 32 KB |
| Application | `0x08008000` | 32 KB |

Both images use the same 96 KB RAM region starting at `0x20000000`.

---

## Boot Flow

After reset, the Cortex-M processor reads the initial Main Stack Pointer (MSP) and Reset Handler address from the bootloader's vector table.

The processor then enters the Reset Handler.

The Reset Handler initializes the C runtime environment:

1. Copies initialized `.data` values from Flash to RAM.
2. Clears the `.bss` section.
3. Calls `main()`.

The bootloader then decides whether to remain in bootloader mode or launch the application.

The application follows the same startup process using its own vector table and linker configuration.

---

## Linker Scripts

The bootloader and application are built as independent firmware images using separate linker scripts.

The bootloader is linked at:

```text
0x08000000
```

with a 32 KB Flash region.

The application is linked at:

```text
0x08008000
```

with its own 32 KB Flash region.

The main sections defined by the linker scripts are:

- `.isr_vector` — interrupt and exception vector table
- `.firmware_header` — application firmware metadata
- `.text` — executable code
- `.rodata` — read-only data
- `.data` — initialized variables
- `.bss` — zero-initialized variables

The linker scripts also expose symbols used by the startup code, including:

```text
_estack
_etext
_sdata
_edata
_sidata
_sbss
_ebss
```

These symbols allow the startup code to determine where sections begin and end.

For `.data`, the initialization values are stored in Flash while the section itself executes from RAM. `_sidata` provides the Flash address used by the startup code when copying the initial values into RAM.

---

## Startup Code

The startup code defines the vector table containing:

- Initial MSP
- Reset Handler
- Cortex-M exception handlers
- Interrupt handlers

The vector table is placed into the `.isr_vector` linker section.

The Reset Handler uses the linker-provided symbols to initialize `.data` and `.bss` before calling `main()`.

Unimplemented exception and interrupt handlers currently use weak aliases to a default handler.

---

## Application Jump

The application has its own vector table beginning at:

```text
0x08008000
```

The first two entries contain:

```text
Application MSP
Application Reset Handler
```

The bootloader reads these values before transferring execution to the application.

The jump sequence is:

1. Read the application's initial MSP.
2. Read the application's Reset Handler address.
3. Disable interrupts.
4. Set `VTOR` to the application's vector table.
5. Set the MSP to the application's initial stack pointer.
6. Branch to the application's Reset Handler.

This allows the application to execute as an independent firmware image using its own vector table and startup code.

---

## Firmware Verification

CRC32 is used to verify the integrity of the firmware before the bootloader launches it.

The application contains a firmware header containing:

```text
┌─────────────────────┐
│ Sentinel            │
│ Device ID           │
│ Firmware Length     │
│ Version             │
│ CRC32               │
└─────────────────────┘
```

The header is placed in the application image using a dedicated `.firmware_header` linker section.

During image preparation, the CRC field is initially set to `0xFFFFFFFF`. The CRC is then calculated over the firmware image and the resulting value is written back into the header.

After programming, the bootloader calculates the CRC of the received application image and compares it with the CRC stored in the firmware header.

The application is only considered valid if the values match.

---

## UART Firmware Update

The firmware update protocol uses a simple request/response mechanism between the PC and bootloader.

```text
PC                         Bootloader
 │                              │
 │──── Firmware Request ───────>│
 │                              │
 │<──────── READY ──────────────│
 │                              │
 │──── 256-byte packet ────────>│
 │                              │
 │<──────── ACK ────────────────│
 │                              │
 │──── 256-byte packet ────────>│
 │                              │
 │<──────── NACK ───────────────│
 │                              │
 │────── Retry packet ─────────>│
 │                              │
 │<──────── ACK ────────────────│
 │                              │
 │       CRC verification       │
 │                              │
 │<──────── SUCCESS ────────────│
```

Firmware is received in 256-byte packets.

After successfully programming a packet, the bootloader sends an `ACK`.

If the packet cannot be accepted or programmed successfully, the bootloader sends a `NACK`.

The PC retries failed packets up to five times.

The bootloader first receives enough data to obtain the firmware header and determine the complete firmware length. It then receives the remaining firmware until the expected image size has been reached.

The current implementation uses polling-based UART reception.

---

## Flash Programming

The bootloader contains routines for erasing and programming the STM32L4 Flash.

The STM32L476RG uses 2 KB Flash pages for erase operations.

Before a Flash operation, the bootloader:

1. Waits for any previous Flash operation to complete.
2. Clears previous error flags.
3. Unlocks the Flash peripheral when required.
4. Performs the requested erase or programming operation.
5. Waits for the operation to complete.
6. Checks and clears status flags.

Because Flash programming uses an 8-byte programming granularity while UART provides data byte-by-byte, received firmware is accumulated in a RAM buffer before being written to Flash.

The current implementation uses a 256-byte buffer.

---

## Future Work

Planned improvements for Version 2:

- USART DMA
- Interrupt-driven UART reception
- AES-based firmware encryption
- Improved boot-mode entry mechanism