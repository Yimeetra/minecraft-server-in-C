#include "ByteArray.h"
#include <malloc.h>
#include <memory.h>

void ba_new(ByteArray* byte_array, int length) {
    byte_array->bytes = (byte*)malloc(length*sizeof(byte));
    byte_array->length = length;
}

void ba_append_byte(ByteArray* byte_array, byte value) {
    byte_array->count += 1;
    if (byte_array->length < byte_array->count) {
        byte_array->length += 1;
        byte_array->bytes = realloc(byte_array->bytes, byte_array->length*sizeof(value));
    }
    byte_array->bytes = realloc(byte_array->bytes, byte_array->length*sizeof(value));
    byte_array->bytes[byte_array->length-1] = value;
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
    while (value) {
        byte chunk = value & SEGMENT_BITS;
        ba_append_byte(byte_array, chunk | CONTINUE_BIT);
        value >>= 7;
    }
    byte_array->bytes[byte_array->length-1] &= SEGMENT_BITS;
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