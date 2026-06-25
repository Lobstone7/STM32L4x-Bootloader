#include "header_tool.h"

typedef struct{
    uint32_t sentinel;
    uint32_t device_id;
    uint32_t version;
    uint32_t length;
    uint32_t crc;
}Firmware_Header;

uint32_t crc32(uint8_t *data,uint32_t length){
    uint8_t byte;
    uint32_t mask;
    uint32_t crc = 0xffffffff;
    for(uint32_t i = 0; i<length;i++){
        byte = data[i];
        crc ^= byte;
        for(int j = 0;j<8;j++){
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xedb88320 & mask);
        }
    }
    return ~(crc);
}

bool header_populate(){
    FILE *file = fopen("../build/app.bin","rb+");
    if(!file) return false;

    fseek(file,0,SEEK_END);
    long size = ftell(file);
    printf("File size = %ld\n", size);
    rewind(file);

    if(size<=0){
        fclose(file);
        return false;
    }

    uint8_t *buffer = malloc(size);
    if(!buffer){
        fclose(file);
        free(buffer);
        buffer = NULL;
        return false;
    }

    if(fread(buffer,1,size,file)!=(size_t)size){
        free(buffer);
        buffer = NULL;
        fclose(file);
        return false;
    }

    int match = 0;
    uint32_t* sentinel = NULL;
    
    for(uint32_t i =0;i<=(size-4);i+=4){
        uint32_t *current_word = (uint32_t *)(buffer+i);
        if(*current_word == 0xDEADC0DE){
            match++;
            sentinel = current_word;
        }
    }
    printf("Match count = %d\n", match);

    if(match!=1){
        free(buffer);
        buffer = NULL;
        fclose(file);
        return false;
    }

    Firmware_Header* header_add = (Firmware_Header*) sentinel;

    uint32_t size_uint = (uint32_t)size;

    header_add->length = size_uint;
    header_add->crc = 0xffffffff;

    uint32_t crc = crc32(buffer,size_uint);

    printf("CRC = 0x%08X\n", crc);

    header_add->crc = crc;

    rewind(file);
    size_t bytes_written = fwrite(buffer,1,size,file);
    if(bytes_written != (size_t)size){
        free(buffer);
        buffer = NULL;
        fclose(file);
        return false;
    }

    free(buffer);
    buffer = NULL;
    fclose(file);

    return true;

}

int main(void)
{
    printf("Tool started\n");

    if(header_populate())
        printf("Success\n");
    else
        printf("Failed\n");

    return 0;
}
