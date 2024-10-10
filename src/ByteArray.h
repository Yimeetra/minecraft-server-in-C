#pragma once

#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

typedef unsigned char byte;
typedef struct {
    int length;
    int count;
    byte *bytes;
} ByteArray;

ByteArray ba_new(int length);
ByteArray ba_copy(ByteArray byte_array);

void ba_extend(ByteArray* byte_array, int size);
void ba_shift(ByteArray* byte_array, int value);

void ba_append_byte(ByteArray* byte_array, byte value);
byte ba_read_byte(ByteArray* byte_array);
byte ba_pull_byte(ByteArray* byte_array);

void ba_append_varint(ByteArray* byte_array, int value);
int ba_read_varint(ByteArray* byte_array);
int ba_pull_varint(ByteArray* byte_array);

void ba_append(ByteArray* byte_array, void* value, int size);

void ba_append_int(ByteArray* byte_array, int value);
int ba_read_int(ByteArray* byte_array);
int ba_pull_int(ByteArray* byte_array);

void ba_append_string(ByteArray* byte_array, char* string, int size);
void ba_read_string(ByteArray* byte_array, char* string, int* size);
void ba_pull_string(ByteArray* byte_array, char* string, int* size);
