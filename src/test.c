#include <stdio.h>
#include <string.h>
#include "md5.h"
//#include "byte_array.h"

#define BUFFER_SIZE 32
#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

int main()
{
    unsigned char buf[BUFFER_SIZE] = {0x11, 0x00, 0xff, 0x05, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x63, 0xdd, 0x02, 0x00, 0x00 };

    for (int i = 0; i < BUFFER_SIZE; ++i) {

    }

    printf("%x\n", read_varint(buf));
    printf("%x\n", read_varint(buf));
    printf("%x\n", read_varint(buf));
    printf("%x\n", read_varint(buf));
    printf("%x\n", read_varint(buf));

    

    return 0;
}