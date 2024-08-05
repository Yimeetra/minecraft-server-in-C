#include <stdio.h>
#include <string.h>
#include "md5.h"
#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000
#define BUFFER_SIZE 256


int read_varint(unsigned char* buffer, int *buf_len)
{
    int value;
    unsigned char temp[BUFFER_SIZE];

    int i = 0;
    while (1) {        
        value |= (buffer[i] & SEGMENT_BITS) << i*7;
        if (!(buffer[i] & CONTINUE_BIT)) {
            break;
        }
        i++;
    }
    *buf_len -= i+1;
    for (int j = i; j < BUFFER_SIZE; ++j) {
        temp[j-i-1] = buffer[j];
    }
    memcpy(buffer, temp, BUFFER_SIZE);
    return value;
}


void write_varint(unsigned char* buffer, int *buf_len, int value)
{
    int i = *buf_len;
    while (1) {
        if (!(value & ~SEGMENT_BITS)) {
            buffer[i] = value;
            break;
        };    
        buffer[i] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (unsigned int) value >> 7;
        i++;
    }
    *buf_len = i+1;
    return;
}

void read_string(unsigned char* buffer, int *buf_len, unsigned char* out, int *out_len)
{
    unsigned char temp[BUFFER_SIZE];

    *out_len = buffer[0];
    for (int i = 0; i < *out_len; ++i) {
        out[i] = buffer[i+1];
    }
    *buf_len -= *out_len;
    for (int j = *out_len; j < BUFFER_SIZE; ++j) {
        temp[j-*out_len-1] = buffer[j];
    }
    memcpy(buffer, temp, BUFFER_SIZE);
    return;
}

void write_string(unsigned char* buffer, int *buf_len, unsigned char* in, int in_len)
{
    buffer[*buf_len] = in_len;
    for (int i = 0; i < in_len; ++i) {
        buffer[*buf_len+i+1] = in[i];
    }
    *buf_len += in_len+1;
    return;
}


int main()
{
    unsigned char buf[BUFFER_SIZE];
    int len = 0;
    
    unsigned char out[BUFFER_SIZE] = {0};
    int out_len;

    write_varint(buf, &len, 25565);
    write_string(buf, &len, "qwerty", 6);
    write_varint(buf, &len, 128);

    for (int i = 0 ; i < BUFFER_SIZE; ++i ) {
        printf("%.2x ", buf[i]);
    }
    printf("%d\n\n",len);

    printf("%d\n",read_varint(buf, &len));
    
    read_string(buf, &len, out, &out_len);
    printf("%s\n", out);

    printf("%d\n",read_varint(buf, &len));

    return 0;
}