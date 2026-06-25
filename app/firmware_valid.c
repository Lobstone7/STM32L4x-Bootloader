#include "firmware_valid.h"

const Firmware_Header firmware_header __attribute__((section(".firmware_header"), used)) = {
    .sentinel = 0xDEADC0DE,
    .device_id = 0x415,
    .version = 1,
    .length = 0xFFFFFFFF,
    .crc = 0xFFFFFFFF,
};