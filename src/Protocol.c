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
    //byte response[] = "{\"version\":{\"name\":\"1.21\",\"protocol\":767}}";
    byte response[] = "{\"version\":{\"name\":\"1.21\",\"protocol\":767}}";

    memset(client->socket_info.send_buf.bytes, 0, client->socket_info.send_buf.length);
    ba_append_varint(&client->socket_info.send_buf, sizeof(response)+1);
    ba_append_byte(&client->socket_info.send_buf, 0x00);
    ba_append_varint(&client->socket_info.send_buf, sizeof(response)-1);
    ba_append_array(&client->socket_info.send_buf, response, sizeof(response), 1);
    client->socket_info.send_buf.count = sizeof(response)+2;
    printf("Bytes to send: ");
    for (int i = 0; i < client->socket_info.send_buf.count; ++i) {
        printf("%.2x ", client->socket_info.send_buf.bytes[i]);
    }
    printf("\n");
}

void HandlePingRequest(Packet *packet, Client *client) {
    SendPongResponse(packet, client);
}

void SendPongResponse(Packet *packet, Client *client) {
    ba_append_varint(&client->socket_info.send_buf, packet->length);
    ba_append_varint(&client->socket_info.send_buf, packet->id);
    ba_append_array(&client->socket_info.send_buf, packet->data.bytes, packet->data.length, 1);
}