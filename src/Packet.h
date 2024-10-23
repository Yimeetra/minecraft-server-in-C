#pragma once

#include "ByteArray.h"

typedef struct {
    int full_length;
    int length;
    int id;
    ByteArray data;
} Packet;

typedef struct {
    int length;
    Packet *packets;
} PacketQueue;

void print_packet(Packet packet);
Packet parse_packet(ByteArray data);
void PacketQueue_append(PacketQueue* packet_queue, Packet packet);
Packet PacketQueue_pull(PacketQueue* packet_queue);
PacketQueue PacketQueue_new();
PacketQueue PacketQueue_copy(PacketQueue packet_queue);
Packet Packet_copy(Packet packet_queue);
Packet Packet_new(int id);
void Packet_calculate_length(Packet* packet);
ByteArray packet_to_bytearray(Packet packet);
