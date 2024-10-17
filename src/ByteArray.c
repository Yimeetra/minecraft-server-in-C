#include "ByteArray.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>

void ba_print(ByteArray byteArray) {
    printf("length = %d\n", byteArray.length);
    printf("count = %d\n", byteArray.count);
    printf("offset = %d\n", byteArray.offset);
    printf("data = ");
    for (int i = 0; i < byteArray.count; ++i) printf("%.2x ", byteArray.bytes[i]);
    printf("\n");
}

ByteArray ba_new(int length) {
    ByteArray byte_array;
    byte_array.bytes = (Byte*)malloc(length*sizeof(Byte));
    byte_array.length = length;
    byte_array.count = 0;
    byte_array.offset = 0;
    return byte_array;
}

void ba_free(ByteArray* byte_array) {
    free(byte_array->bytes);
    byte_array->bytes = NULL;
}

void ba_clear(ByteArray *byteArray) {
    if (byteArray->bytes != NULL) {
        ba_free(byteArray);
    }
    byteArray->bytes = (Byte*)malloc(byteArray->length*sizeof(Byte)); 
    byteArray->count = 0;
    byteArray->offset = 0;
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

void ba_append_byte(ByteArray* byte_array, Byte value) {
    byte_array->count += 1;
    if (byte_array->length < byte_array->count) {
        ba_extend(byte_array, 1);
    }
    byte_array->bytes[byte_array->count-1] = value;
}

Byte ba_read_byte(ByteArray* byte_array) {
    return byte_array->bytes[byte_array->offset];
}

int ba_read_varint(ByteArray* byte_array) {
    int result = 0;
    int i = 0;

    while (1) {
        result += (byte_array->bytes[byte_array->offset+i] & SEGMENT_BITS) << i*7;
        if (!(byte_array->bytes[byte_array->offset+i] & CONTINUE_BIT)) break;
        i++;
    }

    return result;
}

void ba_append_varint(ByteArray* byte_array, int value) {
    do {
        Byte chunk = value & SEGMENT_BITS;
        ba_append_byte(byte_array, chunk | CONTINUE_BIT);
        value >>= 7;
    } while (value);
    byte_array->bytes[byte_array->count-1] &= SEGMENT_BITS;
}

void ba_shift(ByteArray* byte_array, int value) {
    byte_array->count -= value;
    if (byte_array->count < 0) byte_array->count = 0;
    byte_array->offset += value;
}

Byte ba_pull_byte(ByteArray* byte_array) {
    Byte result = byte_array->bytes[byte_array->offset];
    ba_shift(byte_array, 1);
    return result;
}

int ba_pull_varint(ByteArray* byte_array) {
    int result = 0;
    int i = 0;

    while (1) {
        result += (byte_array->bytes[byte_array->offset+i] & SEGMENT_BITS) << i*7;
        if (!(byte_array->bytes[byte_array->offset+i] & CONTINUE_BIT)) break;
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
    memcpy(byte_array->bytes+byte_array->count, (Byte*)value, size);
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
    int result = byte_array->bytes[byte_array->offset+0] << 24 |
                 byte_array->bytes[byte_array->offset+1] << 16 |
                 byte_array->bytes[byte_array->offset+2] << 8  |
                 byte_array->bytes[byte_array->offset+3] << 0  ;
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
    int byteArrayPos = byte_array->offset;
    int length = ba_pull_varint(byte_array);
    byte_array->count += byte_array->offset-byteArrayPos;
    memcpy(string, byte_array->bytes+byte_array->offset-byteArrayPos, length);
    if (size) *size = length;
}

void ba_pull_string(ByteArray* byte_array, char* string, int* size) {
    int length = ba_pull_varint(byte_array);
    memcpy(string, byte_array->bytes+byte_array->offset, length);
    ba_shift(byte_array, length);
    if (size) *size = length;
}

void ba_append_float(ByteArray* byte_array, float value) {
    if (byte_array->length < byte_array->count + 4) {
        ba_extend(byte_array, 4);
    }
    byte_array->bytes[byte_array->count+0] = (int)value >> 24;
    byte_array->bytes[byte_array->count+1] = (int)value >> 16;
    byte_array->bytes[byte_array->count+2] = (int)value >> 8;
    byte_array->bytes[byte_array->count+3] = (int)value >> 0;
    byte_array->count += 4;
}

float ba_read_float(ByteArray* byte_array) {
    float result = byte_array->bytes[byte_array->offset+0] << 24 |
                   byte_array->bytes[byte_array->offset+1] << 16 |
                   byte_array->bytes[byte_array->offset+2] << 8  |
                   byte_array->bytes[byte_array->offset+3] << 0  ;
    return result;
}

float ba_pull_float(ByteArray* byte_array) {
    float result = ba_read_float(byte_array);
    ba_shift(byte_array, 4);
    return result;
}

void ba_append_double(ByteArray* byte_array, double value) {
    if (byte_array->length < byte_array->count + 8) {
        ba_extend(byte_array, 8);
    }
    byte_array->bytes[byte_array->count+0] = (long long)value >> 8*7;
    byte_array->bytes[byte_array->count+1] = (long long)value >> 8*6;
    byte_array->bytes[byte_array->count+2] = (long long)value >> 8*5;
    byte_array->bytes[byte_array->count+3] = (long long)value >> 8*4;
    byte_array->bytes[byte_array->count+4] = (long long)value >> 8*3;
    byte_array->bytes[byte_array->count+5] = (long long)value >> 8*2;
    byte_array->bytes[byte_array->count+6] = (long long)value >> 8*1;
    byte_array->bytes[byte_array->count+7] = (long long)value >> 8*0;
    byte_array->count += 8;
}

double ba_read_double(ByteArray* byte_array) {
    double result = ((long long)byte_array->bytes[byte_array->offset+0] << 8*7) |
                    ((long long)byte_array->bytes[byte_array->offset+1] << 8*6) |
                    ((long long)byte_array->bytes[byte_array->offset+2] << 8*5) |
                    ((long long)byte_array->bytes[byte_array->offset+3] << 8*4) |
                    ((long long)byte_array->bytes[byte_array->offset+4] << 8*3) |
                    ((long long)byte_array->bytes[byte_array->offset+5] << 8*2) |
                    ((long long)byte_array->bytes[byte_array->offset+6] << 8*1) |
                    ((long long)byte_array->bytes[byte_array->offset+7] << 8*0) ;
    return result;
}

double ba_pull_double(ByteArray* byte_array) {
    double result = ba_read_double(byte_array);
    ba_shift(byte_array, 8);
    return result;
}

void ba_append_short(ByteArray* byte_array, short value) {
    if (byte_array->length < byte_array->count + 2) {
        ba_extend(byte_array, 2);
    }
    byte_array->bytes[byte_array->count+0] = value >> 8;
    byte_array->bytes[byte_array->count+1] = value >> 0;
    byte_array->count += 2;
}

short ba_read_short(ByteArray* byte_array) {
    short result = byte_array->bytes[byte_array->offset+0] << 8  |
                   byte_array->bytes[byte_array->offset+1] << 0  ;
    return result;
}

short ba_pull_short(ByteArray* byte_array) {
    short result = ba_read_short(byte_array);
    ba_shift(byte_array, 2);
    return result;
}

void ba_append_long(ByteArray* byte_array, long long value) {
    if (byte_array->length < byte_array->count + 8) {
        ba_extend(byte_array, 8);
    }
    byte_array->bytes[byte_array->count+0] = value >> 8*7;
    byte_array->bytes[byte_array->count+1] = value >> 8*6;
    byte_array->bytes[byte_array->count+2] = value >> 8*5;
    byte_array->bytes[byte_array->count+3] = value >> 8*4;
    byte_array->bytes[byte_array->count+4] = value >> 8*3;
    byte_array->bytes[byte_array->count+5] = value >> 8*2;
    byte_array->bytes[byte_array->count+6] = value >> 8*1;
    byte_array->bytes[byte_array->count+7] = value >> 8*0;
    byte_array->count += 8;
}

long long ba_read_long(ByteArray* byte_array) {
    long long result = ((long long)byte_array->bytes[byte_array->offset+0] << 8*7) |
                       ((long long)byte_array->bytes[byte_array->offset+1] << 8*6) |
                       ((long long)byte_array->bytes[byte_array->offset+2] << 8*5) |
                       ((long long)byte_array->bytes[byte_array->offset+3] << 8*4) |
                       ((long long)byte_array->bytes[byte_array->offset+4] << 8*3) |
                       ((long long)byte_array->bytes[byte_array->offset+5] << 8*2) |
                       ((long long)byte_array->bytes[byte_array->offset+6] << 8*1) |
                       ((long long)byte_array->bytes[byte_array->offset+7] << 8*0) ;
    return result;
}

long long ba_pull_long(ByteArray* byte_array) {
    long long result = ba_read_long(byte_array);
    ba_shift(byte_array, 8);
    return result;
}