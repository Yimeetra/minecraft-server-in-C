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
    ba_append_string(&respone_packet.data, response, sizeof(response));
    packet_to_bytearray(respone_packet, &client->socket_info.send_buf);
}

void HandlePingRequest(Packet *packet, Client *client) {
    SendPongResponse(packet, client);
}

void SendPongResponse(Packet *packet, Client *client) {
    packet_to_bytearray(*packet, &client->socket_info.send_buf);
}

void HandleLoginStart(Packet *packet, Client *client) {
    ba_pull_string(&packet->data, client->nickname, NULL);
    generate_uuid(client->nickname, client->UUID);
    SendLoginSuccess(packet, client);
}

void SendLoginSuccess(Packet *packet, Client *client) {
    client->login_acknowledged = true;
    Packet respone_packet = Packet_new(0x02);
    ba_append(&respone_packet.data, client->UUID, 16);
    ba_append_string(&respone_packet.data, client->nickname, 16);
    ba_append_varint(&respone_packet.data, 0);
    ba_append_bool(&respone_packet.data, 0x00);

    packet_to_bytearray(respone_packet, &client->socket_info.send_buf);
}

void HandleLoginAckAcknowledged(Packet *packet, Client *client) {
    if (!client->login_acknowledged) {
        close_connection(client);
        return;
    }
    client->game_state = CONFIGURATION;
    SendKnownPacks(packet, client);
}

void SendKnownPacks(Packet *packet, Client *client) {
    Packet respone_packet = Packet_new(0x0E);
    ba_append_varint(&respone_packet.data, 1);
    ba_append_string(&respone_packet.data, "minecraft:core", 15);
    ba_append_string(&respone_packet.data, "0", 2);
    ba_append_string(&respone_packet.data, "1.21", 5);

    packet_to_bytearray(respone_packet, &client->socket_info.send_buf);
}