#pragma once

#include "ByteArray.h"

typedef struct {
    int length;
    int id;
    ByteArray* data;
} Packet;

void parse_packet(Packet* packet, ByteArray* data);