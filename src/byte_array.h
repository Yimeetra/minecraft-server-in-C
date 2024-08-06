#include <string.h>

#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000


int pull_varint(unsigned char* buffer, int *buf_len)
{
    int value = 0;
    unsigned char temp[BUFFER_SIZE];

    int i = 0;
    while (1) {        
        value |= (buffer[i] & SEGMENT_BITS) << i*7;
        if (!(buffer[i] & CONTINUE_BIT)) {
            break;
        }
        i++;
    }
    if (buf_len) *buf_len -= i+1;
    for (int j = 0; j < BUFFER_SIZE; ++j) {
        temp[j] = buffer[j+i+1];
    }
    memcpy(buffer, temp, BUFFER_SIZE);
    return value;
}


void pull_string(unsigned char* buffer, int *buf_len, unsigned char* out, int *out_len)
{
    unsigned char temp[BUFFER_SIZE];

    *out_len = buffer[0];
  
    for (int i = 0; i < *out_len; ++i) {
        out[i] = buffer[i+1];
    }
    if (buf_len) *buf_len -= *out_len;

    for (int j = 0; j < BUFFER_SIZE; ++j) {
        temp[j] = buffer[j+*out_len+1];
    }

    memcpy(buffer, temp, BUFFER_SIZE);
    return;
}


void pull_uuid(unsigned char* buffer, int *buf_len, unsigned char* out)
{
    unsigned char temp[BUFFER_SIZE];
    memcpy(out, buffer, 16);

    if (buf_len) *buf_len -= 16;
    for (int j = 0; j < BUFFER_SIZE; ++j) {
        temp[j] = buffer[j+16];
    }
    memcpy(buffer, temp, BUFFER_SIZE);
    return;
}


char pull_byte(unsigned char* buffer, int *buf_len)
{
    unsigned char temp[BUFFER_SIZE];

    char value = buffer[0];
    if (buf_len) *buf_len -= 1;
    for (int j = 0; j < BUFFER_SIZE; ++j) {
        temp[j] = buffer[j+1];
    }
    memcpy(buffer, temp, BUFFER_SIZE);
    return value;
}


void append_varint(unsigned char* buffer, int *buf_len, int value)
{
    int i = *buf_len;
    while (1) {
        if (!(value & ~SEGMENT_BITS)) {
            buffer[i] = value;
            break;
        }   
        buffer[i] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (unsigned int) value >> 7;
        i++;
    }
    *buf_len = i+1;
    return;
}


void append_string(unsigned char* buffer, int *buf_len, unsigned char* in, int in_len)
{
    buffer[*buf_len] = in_len;
    for (int i = 0; i < in_len; ++i) {
        buffer[*buf_len+i+1] = in[i];
    }
    *buf_len += in_len+1;
    return;
}


void append_uuid(unsigned char* buffer, int *buf_len, unsigned char* uuid)
{
    for (int i = 0; i < 16; ++i) {
        buffer[*buf_len+i] = uuid[i];
    }
    *buf_len += 16;
    return;
}


void append_byte(unsigned char* buffer, int *buf_len, char byte)
{
    buffer[*buf_len] = byte;
    *buf_len += 1;
    return;
}


int read_varint(unsigned char* buffer)
{
    int value = 0;
    int i = 0;
    while (1) {        
        value |= (buffer[i] & SEGMENT_BITS) << i*7;
        if (!(buffer[i] & CONTINUE_BIT)) {
            break;
        }
        i++;
    }
    return value;
}


void read_string(unsigned char* buffer, unsigned char* out, int *out_len)
{
    unsigned char temp[BUFFER_SIZE];

    *out_len = buffer[0];
    for (int i = 0; i < *out_len; ++i) {
        out[i] = buffer[i+1];
    }
    return;
}


void read_uuid(unsigned char* buffer, unsigned char* out)
{
    unsigned char temp[BUFFER_SIZE];
    memcpy(out, buffer, 16);

    return;
}


char read_byte(unsigned char* buffer)
{
    return buffer[0];
}