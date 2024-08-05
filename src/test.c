#include <stdio.h>
#include <string.h>
#include "md5.h"
#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

int read_varint(unsigned char* varint)
{
    int value;
    int i = 0;
    while (1) {        
        value |= (varint[i] & SEGMENT_BITS) << i*7;
        if (!(varint[i] & CONTINUE_BIT)) {
            break;
        }
        i++;
    }
    return value;
}

void write_varint(unsigned char* varint, int value)
{
    int i = 0;
    while (1) {
        if (!(value & ~SEGMENT_BITS)) {
            varint[i] = value;
            break;
        };    
        varint[i] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (unsigned int) value >> 7;
        i++;
    }
    return;
}

void generate_uuid(char* nickname, unsigned char* result)
{
    char temp[32];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}

int main()
{
    unsigned char buf[5] = {0};
    buf[0] = 0x1c;
    buf[1] = 0x0f;

    printf("%d",read_varint(buf));
    return 0;
}