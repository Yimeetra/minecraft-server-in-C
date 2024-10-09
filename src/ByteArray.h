#pragma once

#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

typedef unsigned char byte;
typedef struct {
    int length;
    int count;
    byte *bytes;
} ByteArray;

void ba_append_byte(ByteArray* byte_array, byte value);
void ba_append_varint(ByteArray* byte_array, int value);
int ba_read_varint(ByteArray* byte_array);
byte ba_read_byte(ByteArray* byte_array);
void ba_shift(ByteArray* byte_array, int value);
byte ba_pull_byte(ByteArray* byte_array);
int ba_pull_varint(ByteArray* byte_array);
ByteArray ba_new(int length);
void ba_append(ByteArray* byte_array, void* value, int size);
void ba_append_array(ByteArray* byte_array, void* array, int length, int items_size);
ByteArray ba_copy(ByteArray byte_array);