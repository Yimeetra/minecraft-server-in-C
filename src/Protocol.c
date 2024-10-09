#include "Protocol.h"
#include <stdio.h>
#include <winsock2.h>
#include "Client.h"
#include <string.h>

void HandleHandshake(Packet *packet, Client *client)
{
    client->game_state = packet->data.bytes[packet->data.length-1];
}

void HandleStatusRequest(Packet *packet, Client *client) {
    SendStatusResponse(packet, client);
}

void SendStatusResponse(Packet *packet, Client *client) {
    byte response[] = "{\"version\":{\"name\":\"1.21\",\"protocol\":767}}";

    Packet respone_packet = Packet_new(0x00);
    ba_append_varint(&respone_packet.data, sizeof(response)-1);
    ba_append_array(&respone_packet.data, response, sizeof(response)-1, 1);
    packet_to_bytearray(respone_packet, &client->socket_info.send_buf);
}

void HandlePingRequest(Packet *packet, Client *client) {
    SendPongResponse(packet, client);
}

void SendPongResponse(Packet *packet, Client *client) {
    packet_to_bytearray(*packet, &client->socket_info.send_buf);
}