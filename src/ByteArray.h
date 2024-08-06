#pragma once

#include <string.h>
#include "defines.h"

#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

int pull_varint(unsigned char* buffer, int *buf_len);
void pull_string(unsigned char* buffer, int *buf_len, unsigned char* out, int *out_len);
void pull_uuid(unsigned char* buffer, int *buf_len, unsigned char* out);
char pull_byte(unsigned char* buffer, int *buf_len);
void append_varint(unsigned char* buffer, int *buf_len, int value);
void append_string(unsigned char* buffer, int *buf_len, unsigned char* in, int in_len);
void append_uuid(unsigned char* buffer, int *buf_len, unsigned char* uuid);
void append_byte(unsigned char* buffer, int *buf_len, char byte);
int read_varint(unsigned char* buffer);
void read_string(unsigned char* buffer, unsigned char* out, int *out_len);
void read_uuid(unsigned char* buffer, unsigned char* out);
char read_byte(unsigned char* buffer);