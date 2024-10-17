#include "Packet.h"
#include <assert.h>
#include "ByteArray.h"
#include <stdio.h>

void print_packet(Packet packet) {
    printf("Parsed packet:\n");
    printf("  Length = %i\n", packet.length);
    printf("  Id = %i\n", packet.id);
    printf("  Data:\n");
    printf("    Length = %i\n", packet.data.length);
    printf("    Data = ");
    for (int i = 0; i < packet.data.length; ++i) {
        printf("%.2x ", packet.data.bytes[i]);
    }
    printf("\n");
}

int test_parse_packet() {
    Packet packet;
    ByteArray byte_array = ba_new(0);
    Byte temp[] = {0x10, 0x00, 0xff, 0x05, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x63, 0xdd, 0x02};

    ba_append(&byte_array, temp, sizeof(temp));

    packet = parse_packet(byte_array);

    print_packet(packet);

    assert(packet.length == 16);
    assert(packet.id == 0);
    assert(packet.data.length == 15);
    assert(packet.data.count == 15);

    for (int i = 0; i < 15; ++i) {
        assert(packet.data.bytes[i] == temp[i+2]);
    }
    
    assert(packet.data.bytes != byte_array.bytes);
    
    ba_shift(&byte_array, 1);
    for (int i = 0; i < 15; ++i) {
        assert(packet.data.bytes[i] == temp[i+2]);
    }

    for (int i = 0; i < 15; ++i) {
        assert(byte_array.bytes[byte_array.offset+i] == temp[i+1]);
    }



    return 0;
}

int test_Packet_copy() {
    Packet packet1;
    Packet packet2;
    ByteArray byte_array = ba_new(0);
    Byte temp[] = {0x10, 0x00, 0xff, 0x05, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x63, 0xdd, 0x02};
    ba_append(&byte_array, temp, sizeof(temp));

    packet1 = parse_packet(byte_array);

    packet2 = Packet_copy(packet1);

    assert(&packet1 != &packet2);
    assert(&packet1.data != &packet2.data);
    
    return 0;
}

int test_PacketQueue_new() {
    PacketQueue packet_queue = PacketQueue_new();

    assert(packet_queue.length == 0);
    assert(packet_queue.packets != NULL);

    return 0;
}

int test_PacketQueue_copy() {
    PacketQueue packet_queue1 = PacketQueue_new();
    PacketQueue packet_queue2 = PacketQueue_copy(packet_queue1);
    
    assert(&packet_queue1 != &packet_queue2);
    assert(&packet_queue1.packets != &packet_queue2.packets);

    return 0;
}

int test_PacketQueue_append() {
    PacketQueue packet_queue = PacketQueue_new();
    Packet packet1;
    ByteArray byte_array1 = ba_new(0);
    Byte temp1[] = {0x10, 0x00, 0xff, 0x05, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x63, 0xdd, 0x01};

    ba_append(&byte_array1, temp1, sizeof(temp1));
    packet1 = parse_packet(byte_array1);

    Packet packet2;
    ByteArray byte_array2 = ba_new(0);
    Byte temp2[] = {0x01, 0x00};

    ba_append(&byte_array2, temp2, sizeof(temp2));
    packet2 = parse_packet(byte_array2);

    PacketQueue_append(&packet_queue, packet1);
    PacketQueue_append(&packet_queue, packet2);


    //print_packet(packet_queue.packets[0]);
    //print_packet(packet1);

    //print_packet(packet_queue.packets[1]);
    //print_packet(packet2);
    
    return 0;
}

int main() {
    assert(test_parse_packet() == 0);
    assert(test_PacketQueue_new() == 0);
    assert(test_Packet_copy() == 0);
    assert(test_PacketQueue_copy() == 0);
    assert(test_PacketQueue_append() == 0);

    printf("Passed all tests!");
    return 0;
}