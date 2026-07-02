cmake_minimum_required(VERSION 3.20)

project(BOOTLOADER)

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
)

# ---------------- BOOTLOADER ----------------
add_executable(bootloader.elf
    bootloader/startup.c
    bootloader/main.c
    bootloader/usart_protocol.c
    shared/clock.c
    shared/gpio.c
    shared/USART.c
    shared/flash.c
)

target_compile_options(bootloader.elf PRIVATE
    ${MCU_FLAGS}
    -O0
    -g
)

target_link_options(bootloader.elf PRIVATE
    ${MCU_FLAGS}
    -T${CMAKE_SOURCE_DIR}/bootloader/linker.ld
    -nostartfiles
    -nostdlib
)

# ---------------- APPLICATION ----------------
add_executable(app.elf
    app/startup.c
    app/main.c
    app/firmware_valid.c
    shared/gpio.c
    shared/USART.c
)

target_compile_options(app.elf PRIVATE
    ${MCU_FLAGS}
    -O0
    -g
)

target_link_options(app.elf PRIVATE
    ${MCU_FLAGS}
    -T${CMAKE_SOURCE_DIR}/app/linker.ld
    -nostartfiles
    -nostdlib
)

add_custom_command(TARGET bootloader.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary bootloader.elf bootloader.bin
)

add_custom_command(TARGET app.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary app.elf app.bin
)

