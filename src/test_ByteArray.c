#include "ByteArray.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>

int test_append_byte() {
    ByteArray a = {0};

    ba_append_byte(&a, 0x11);
    ba_append_byte(&a, 0x22);
    ba_append_byte(&a, 0x44);

    assert(a.length == 3);
    assert(a.bytes[0] == 0x11);
    assert(a.bytes[1] == 0x22);
    assert(a.bytes[2] == 0x44);
    return 0;
}

int test_append_varint() {
    ByteArray a = {0};

    ba_append_varint(&a, 2097151);
    ba_append_varint(&a, 2097151);


    assert(a.length == 6);
    assert(a.bytes[0] == 0xff);
    assert(a.bytes[1] == 0xff);
    assert(a.bytes[2] == 0x7f);

    assert(a.bytes[3] == 0xff);
    assert(a.bytes[4] == 0xff);
    assert(a.bytes[5] == 0x7f);
    return 0;
}

int test_read_varint() {
    ByteArray a = {0};

    ba_append_varint(&a, 2097151);
    
    assert(ba_read_varint(&a) == 2097151);
    return 0;
}

int test_shift() {
    ByteArray a = {0};

    ba_append_varint(&a, 2097151);
    ba_shift(&a, 2);

    assert(a.bytes[0] == 0x7f);
    assert(a.count == 1);
    return 0;
}

int test_pull_byte() {
    ByteArray a = {0};

    ba_append_byte(&a, 0x66);
    ba_append_byte(&a, 0x12);

    assert(ba_pull_byte(&a) == 0x66);
    assert(a.bytes[0] == 0x12);
    assert(a.count == 1);
    return 0;
}

int test_pull_varint() {
    ByteArray a = {0};

    ba_append_varint(&a, 12345);
    ba_append_varint(&a, 76543);

    assert(ba_pull_varint(&a) == 12345);
    assert(ba_read_varint(&a) == 76543);
    assert(a.count == 3);
    return 0;
}

int test_new() {
    return 1;
}

int main() {
    assert(test_append_byte() == 0);
    assert(test_append_varint() == 0);
    assert(test_read_varint() == 0);
    assert(test_shift() == 0);
    assert(test_pull_byte() == 0);
    assert(test_pull_varint() == 0);
    printf("Passed all tests!");
    return 0;
}