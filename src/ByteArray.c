#include "ByteArray.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>

ByteArray ba_new(int length) {
    ByteArray byte_array;
    byte_array.bytes = (byte*)malloc(length*sizeof(byte));
    byte_array.length = length;
    byte_array.count = 0;
    return byte_array;
}

void ba_free(ByteArray* byte_array) {
    free(byte_array->bytes);
}

ByteArray ba_copy(ByteArray byte_array) {
    ByteArray copy = byte_array;
    copy.bytes = malloc(byte_array.length);
    memcpy(copy.bytes, byte_array.bytes, byte_array.length);
    return copy;
}

void ba_extend(ByteArray* byte_array, int size) {
    byte_array->length += size;
    byte_array->bytes = realloc(byte_array->bytes, byte_array->length);
}

void ba_append_byte(ByteArray* byte_array, byte value) {
    byte_array->count += 1;
    if (byte_array->length < byte_array->count) {
        ba_extend(byte_array, 1);
    }
    byte_array->bytes[byte_array->count-1] = value;
}

byte ba_read_byte(ByteArray* byte_array) {
    return byte_array->bytes[0];
}

int ba_read_varint(ByteArray* byte_array) {
    int result = 0;
    int i = 0;

    while (1) {
        result += (byte_array->bytes[i] & SEGMENT_BITS) << i*7;
        if (!(byte_array->bytes[i] & CONTINUE_BIT)) break;
        i++;
    }

    return result;
}

void ba_append_varint(ByteArray* byte_array, int value) {
    do {
        byte chunk = value & SEGMENT_BITS;
        ba_append_byte(byte_array, chunk | CONTINUE_BIT);
        value >>= 7;
    } while (value);
    byte_array->bytes[byte_array->count-1] &= SEGMENT_BITS;
}

void ba_shift(ByteArray* byte_array, int value) {
    byte_array->count -= value;
    if (byte_array->count < 0) byte_array->count = 0;

    memcpy(byte_array->bytes, byte_array->bytes+value, byte_array->length-1);
}

byte ba_pull_byte(ByteArray* byte_array) {
    byte result = byte_array->bytes[0];
    ba_shift(byte_array, 1);
    return result;
}

int ba_pull_varint(ByteArray* byte_array) {
    int result = 0;
    int i = 0;

    while (1) {
        result += (byte_array->bytes[i] & SEGMENT_BITS) << i*7;
        if (!(byte_array->bytes[i] & CONTINUE_BIT)) break;
        i++;
    }

    ba_shift(byte_array, i+1);

    return result;
}


// appends in big endian
void ba_append(ByteArray* byte_array, void* value, int size) {
    if (byte_array->count + size > byte_array->length) {
        ba_extend(byte_array, byte_array->count + size - byte_array->length);
    }
    memcpy(byte_array->bytes+byte_array->count, (byte*)value, size);
    byte_array->count += size;
}

void ba_append_int(ByteArray* byte_array, int value) {
    if (byte_array->length < byte_array->count + 4) {
        ba_extend(byte_array, 4);
    }
    byte_array->bytes[byte_array->count+0] = value >> 24;
    byte_array->bytes[byte_array->count+1] = value >> 16;
    byte_array->bytes[byte_array->count+2] = value >> 8;
    byte_array->bytes[byte_array->count+3] = value >> 0;
    byte_array->count += 4;
}

int ba_read_int(ByteArray* byte_array) {
    int result = byte_array->bytes[0] << 24 |
                 byte_array->bytes[1] << 16 |
                 byte_array->bytes[2] << 8  |
                 byte_array->bytes[3] << 0  ;
    return result;
}

int ba_pull_int(ByteArray* byte_array) {
    int result = ba_read_int(byte_array);
    ba_shift(byte_array, 4);
    return result;
}

void ba_append_string(ByteArray* byte_array, char* string, int size) {
    ba_append_varint(byte_array, size-1);
    ba_append(byte_array, string, size-1);
}

void ba_read_string(ByteArray* byte_array, char* string, int* size) {
    ByteArray copy = ba_copy(*byte_array);
    int temp = ba_pull_varint(&copy);
    memcpy(string, copy.bytes, temp);
    *size = temp;
}

void ba_pull_string(ByteArray* byte_array, char* string, int* size) {
    int temp = ba_pull_varint(byte_array);
    memcpy(string, byte_array->bytes, temp);
    ba_shift(byte_array, temp);
    *size = temp;
}