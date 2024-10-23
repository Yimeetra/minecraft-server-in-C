#include "Packet.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>

void print_packet(Packet packet) {
    printf("Length = %i\n", packet.length);
    printf("Id = %i\n", packet.id);
    printf("Data:\n");
    printf("  Length = %i\n", packet.data.length);
    printf("  Data = ");
    for (int i = 0; i < packet.data.length; ++i) {
        printf("%.2x ", packet.data.bytes[i]);
    }
    printf("\n\n");
}

Packet Packet_new(int id) {
    Packet packet;
    packet.full_length = 1;
    packet.id = id;
    packet.length = 0;
    packet.data = ba_new(0);
    return packet;
}

Packet parse_packet(ByteArray data) {
    Packet packet;
    int offset = data.offset;
    packet.length = ba_pull_varint(&data);
    packet.id = ba_pull_varint(&data);
    packet.full_length = packet.length + data.offset - offset;
    packet.data = ba_new(0);
    ba_append(&packet.data, data.bytes+data.offset, packet.length-1);
    return packet;
}

Packet Packet_copy(Packet packet) {
    Packet copy = packet;
    copy.data = ba_copy(packet.data);
    return copy;
}

void Packet_calculate_length(Packet* packet) {
    packet->length = 1 + packet->data.count;
}

ByteArray packet_to_bytearray(Packet packet) {
    ByteArray array = ba_new(0);
    Packet_calculate_length(&packet);
    ba_append_varint(&array, packet.length);
    ba_append_varint(&array, packet.id);
    ba_append(&array, packet.data.bytes, packet.data.count);
    return array;
}

void PacketQueue_append(PacketQueue* packet_queue, Packet packet) {
    PacketQueue temp = PacketQueue_copy(*packet_queue);
    packet_queue->length++;

    packet_queue->packets = malloc(sizeof(Packet)*packet_queue->length);
    for (int i = 0; i < temp.length; ++i) {
        packet_queue->packets[i] = temp.packets[i];
    }
    packet_queue->packets[packet_queue->length-1] = Packet_copy(packet);
}

Packet PacketQueue_pull(PacketQueue* packet_queue) {

}

PacketQueue PacketQueue_copy(PacketQueue packet_queue) {
    PacketQueue copy = packet_queue;
    for (int i = 0; i < packet_queue.length; ++i) {
        copy.packets[i] = Packet_copy(packet_queue.packets[i]);
    }
    return copy;
}

PacketQueue PacketQueue_new() {
    PacketQueue packet_queue;
    packet_queue.length = 0;
    packet_queue.packets = malloc(0);
    return packet_queue;
}