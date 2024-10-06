#include "Packet.h"

void parse_packet(Packet* packet, ByteArray* data) {
    packet->length = ba_pull_varint(data);
    packet->id = ba_pull_varint(data);
    packet->data = data;
    packet->data->length = packet->length-1;
}