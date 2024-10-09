#include "ByteArray.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>

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

int test_append() {
    ByteArray a = {0};
    
    ba_append(&a, &(int){0x12345678}, sizeof(int));

    assert(a.bytes[0] == 0x12);
    assert(a.bytes[1] == 0x34);
    assert(a.bytes[2] == 0x56);
    assert(a.bytes[3] == 0x78);

    ba_append(&a, &(int){0xf1d2c3a4}, sizeof(int));

    assert(a.bytes[4] == 0xf1);
    assert(a.bytes[5] == 0xd2);
    assert(a.bytes[6] == 0xc3);
    assert(a.bytes[7] == 0xa4);
    return 0;
}

int test_append_array() {
    ByteArray a = {0};
    int b[] = {0x12345678,2,3,4};
    
    //for (int i = 0; i < sizeof(b)/sizeof(int); ++i) {
    //    ba_append(&a, &b[i], sizeof(int));
    //}
//
    //for (int i = 0; i < a.length; ++i) {
    //    printf("%.2x ", a.bytes[i]);
    //}
    ba_append_array(&a, b, sizeof(b)/sizeof(int), sizeof(int));

    byte c[] = {0x12, 0x34, 0x56, 0x78, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04};

    assert(memcmp(a.bytes, c, a.length) == 0);
    
    return 0;
}

int test_copy() {
    ByteArray a = {0};
    ByteArray b = {0};

    ba_append_byte(&a, 0x11);
    ba_append_byte(&a, 0x22);
    ba_append_byte(&a, 0x44);

    b = ba_copy(a);

    assert(b.bytes[0] = 0x11);
    assert(b.bytes[1] = 0x22);
    assert(b.bytes[2] = 0x44);
    assert(&b != &a);
    assert(b.bytes != a.bytes);

    ba_shift(&a, 1);

    assert(a.bytes[0] = 0x22);
    assert(a.bytes[1] = 0x44);
    assert(a.bytes[2] = 0x44);

    assert(b.bytes[0] = 0x11);
    assert(b.bytes[1] = 0x22);
    assert(b.bytes[2] = 0x44);

    
    return 0;
}

int main() {
    assert(test_append_byte() == 0);
    assert(test_append_varint() == 0);
    assert(test_read_varint() == 0);
    assert(test_shift() == 0);
    assert(test_pull_byte() == 0);
    assert(test_pull_varint() == 0);
    assert(test_append() == 0);
    assert(test_append_array() == 0);
    assert(test_copy() == 0);

    printf("Passed all tests!");
    return 0;
}