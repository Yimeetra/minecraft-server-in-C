#pragma once

#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

typedef unsigned char Byte;
typedef struct {
    int length;
    int count;
    int offset;
    Byte *bytes;
} ByteArray;

void ba_print(ByteArray byteArray);

ByteArray ba_new(int length);
ByteArray ba_copy(ByteArray byte_array);

void ba_extend(ByteArray* byte_array, int size);
void ba_shift(ByteArray* byte_array, int value);

void ba_append_byte(ByteArray* byte_array, Byte value);
Byte ba_read_byte(ByteArray* byte_array);
Byte ba_pull_byte(ByteArray* byte_array);

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

#define ba_append_bool ba_append_byte
#define ba_read_bool ba_read_byte
#define ba_pull_bool ba_pull_byte

void ba_append_float(ByteArray* byte_array, float value);
float ba_read_float(ByteArray* byte_array);
float ba_pull_float(ByteArray* byte_array);

void ba_append_double(ByteArray* byte_array, double value);
double ba_read_double(ByteArray* byte_array);
double ba_pull_double(ByteArray* byte_array);

void ba_append_short(ByteArray* byte_array, short value);
short ba_read_short(ByteArray* byte_array);
short ba_pull_short(ByteArray* byte_array);

void ba_append_long(ByteArray* byte_array, long long value);
long long ba_read_long(ByteArray* byte_array);
long long ba_pull_long(ByteArray* byte_array);