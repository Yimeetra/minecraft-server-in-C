#include "ByteArray.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>

int test_append_byte() {
    ByteArray a = ba_new(0);

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
    ByteArray a = ba_new(0);

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
    ByteArray a = ba_new(0);

    ba_append_varint(&a, 2097151);
    
    assert(ba_read_varint(&a) == 2097151);
    return 0;
}

int test_shift() {
    ByteArray a = ba_new(0);

    ba_append_varint(&a, 2097151);
    ba_shift(&a, 2);

    assert(a.bytes[a.offset] == 0x7f);
    assert(a.offset == 2);
    assert(a.count == 1);
    return 0;
}

int test_pull_byte() {
    ByteArray a = ba_new(0);

    ba_append_byte(&a, 0x66);
    ba_append_byte(&a, 0x12);

    assert(ba_pull_byte(&a) == 0x66);
    assert(a.bytes[a.offset] == 0x12);
    assert(a.count == 1);
    return 0;
}

int test_pull_varint() {
    ByteArray a = ba_new(0);

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
    Byte b[] = {0x12, 0x34, 0x56, 0x78, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04};

    ba_append(&a, b, sizeof(b));

    Byte c[] = {0x12, 0x34, 0x56, 0x78, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04};

    assert(memcmp(a.bytes, c, a.count) == 0);
    
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

int test_append_int() {
    ByteArray a = ba_new(0);

    ba_append_int(&a, 0x12345678);

    assert(a.bytes[0] == 0x12);
    assert(a.bytes[1] == 0x34);
    assert(a.bytes[2] == 0x56);
    assert(a.bytes[3] == 0x78);

    ba_append_int(&a, 0xffaaddcc);

    assert(a.bytes[4] == 0xff);
    assert(a.bytes[5] == 0xaa);
    assert(a.bytes[6] == 0xdd);
    assert(a.bytes[7] == 0xcc);

    assert(a.count == 8);
    assert(a.length >= a.count);

    return 0;
}

int test_read_int() {
    ByteArray a = ba_new(0);
    ba_append_int(&a, 0x12345678);
    assert(ba_read_int(&a) == 0x12345678);
    return 0;
}

int test_pull_int() {
    ByteArray a = ba_new(0);
    ba_append_int(&a, 123);
    ba_append_int(&a, 4567);

    assert(ba_pull_int(&a) == 123);
    assert(ba_pull_int(&a) == 4567);

    assert(a.count == 0);
    
    return 0;
}

int test_append_string() {
    ByteArray a = ba_new(0);
    char hello[] = "hello!";
    ba_append_string(&a, hello, sizeof(hello));
    
    assert(memcmp(a.bytes+1, hello, 6) == 0);
    assert(a.count == 7);

    return 0;
}

int test_read_string() {
    ByteArray a = ba_new(0);
    char b[6];
    int b_size;
    char hello[] = "hello!";
    ba_append_string(&a, hello, sizeof(hello));

    ba_read_string(&a, b, &b_size);

    assert(memcmp(b, hello, 6) == 0);
    assert(memcmp(a.bytes+1, hello, 6) == 0);
    assert(b_size == 6);
    assert(a.count == 7);

    return 0;
}

int test_pull_string() {
    ByteArray a = ba_new(0);
    char b[6];
    int b_size;
    char hello[] = "hello!";
    ba_append_string(&a, hello, sizeof(hello));

    ba_pull_string(&a, b, &b_size);

    assert(memcmp(b, hello, b_size) == 0);
    assert(b_size == 6);
    assert(a.count == 0);

    return 0;
}

int main() {
    assert(test_copy() == 0); printf(".");

    assert(test_shift() == 0); printf(".");

    assert(test_append_byte() == 0); printf(".");
    assert(test_pull_byte() == 0); printf(".");

    assert(test_append_varint() == 0); printf(".");
    assert(test_read_varint() == 0); printf(".");
    assert(test_pull_varint() == 0); printf(".");

    assert(test_append() == 0); printf(".");

    assert(test_append_int() == 0); printf(".");
    assert(test_read_int() == 0); printf(".");
    assert(test_pull_int() == 0); printf(".");

    assert(test_append_string() == 0); printf(".");
    assert(test_read_string() == 0); printf(".");
    assert(test_pull_string() == 0); printf(".");


    printf("\nPassed all tests!");
    return 0;
}