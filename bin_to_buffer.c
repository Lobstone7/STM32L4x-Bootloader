#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

bool process_and_patch_bin(const char *filename)
{
    FILE *fp = fopen(filename, "rb+");  // read + write
    if (!fp) return false;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    if (size <= 0)
    {
        fclose(fp);
        return false;
    }

    uint8_t *buffer = malloc(size);
    if (!buffer)
    {
        fclose(fp);
        return false;
    }

    if (fread(buffer, 1, size, fp) != (size_t)size)
    {
        free(buffer);
        fclose(fp);
        return false;
    }

    // ===== MODIFY BIN HERE =====
    // example: update header, compute CRC, etc.
    // ===========================

    rewind(fp);
    fwrite(buffer, 1, size, fp);

    free(buffer);
    fclose(fp);

    return true;
}