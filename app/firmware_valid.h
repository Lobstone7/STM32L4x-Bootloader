#include "../shared/common.h"

typedef struct{
    uint32_t sentinel;
    uint32_t device_id;
    uint32_t version;
    uint32_t length;
    uint32_t crc;

}Firmware_Header;

