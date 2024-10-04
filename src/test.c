#include <stdio.h>
#include <string.h>
#include "md5.h"
#include "ByteArray.h"

#define BUFFER_SIZE 128
#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000


int main()
{
    unsigned char send_buffer[BUFFER_SIZE] = {0};
    unsigned char data[BUFFER_SIZE] = {0};
    unsigned int packet_len = 0;
    unsigned int temp = 0;

    append_byte(data, &packet_len, 0x2b); // EID
    append_int(data, &packet_len, 0); // EID
    append_byte(data, &packet_len, 0x00); // Is hardcore
    append_varint(data, &packet_len, 1); // Dimension count
    append_string(data, &packet_len, "minecraft:overworld", 19); // array of dimension names (length 1)
    append_varint(data, &packet_len, 5); // Max players
    append_varint(data, &packet_len, 5); // View distance
    append_varint(data, &packet_len, 5); // Simulation distance
    append_byte(data, &packet_len, 0x00); // Reduced debug info
    append_byte(data, &packet_len, 0x01); // Enable respawn screen
    append_byte(data, &packet_len, 0x00); // Do limited crafting
    append_varint(data, &packet_len, 0); // Simulation distance
    append_string(data, &packet_len, "minecraft:overworld", 19); // Dimension name
    append_int(data, &packet_len, 0xa89f); // Hashed seed
    append_int(data, &packet_len, 0xd103); // Hashed seed
    append_byte(data, &packet_len, 0x00); // Gamemode  0 su  1 cr  2 ad  3 sp
    append_byte(data, &packet_len, 0x01); // Previous gamemode  -1 null  0 su  1 cr  2 ad  3 sp
    append_byte(data, &packet_len, 0x00); // Is debug world
    append_byte(data, &packet_len, 0x00); // Is flat world
    append_byte(data, &packet_len, 0x00); // Has death location
    append_varint(data, &packet_len, 10); // Tick before portal
    append_byte(data, &packet_len, 0x00); // Enforce secure chat
    
    append_varint(send_buffer, &temp, packet_len);
    memcpy(send_buffer+temp, data, BUFFER_SIZE-temp);

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        printf("%.2x ", send_buffer[i]);
    }

    printf("   %d\n", packet_len);

    return 0;
}