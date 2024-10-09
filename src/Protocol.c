#include "Protocol.h"
#include <stdio.h>
#include <winsock2.h>
#include "Client.h"
#include <string.h>

void HandleHandshake(Packet *packet, Client *client)
{
    client->game_state = packet->data.bytes[packet->data.length-1];
}

void HanldeStatusRequest(Packet *packet, Client *client) {
    SendStatusResponse(packet, client);
}

void SendStatusResponse(Packet *packet, Client *client) {
    byte response[] = "{\"version\":{\"name\":\"1.19.4\",\"protocol\":762}}";

    memset(client->socket_info.send_buf.bytes, 0, client->socket_info.send_buf.length);
    ba_append_varint(&client->socket_info.send_buf, sizeof(response)+2);
    ba_append_byte(&client->socket_info.send_buf, 0x00);
    ba_append_varint(&client->socket_info.send_buf, sizeof(response)+1);
    ba_append_array(&client->socket_info.send_buf, response, sizeof(response), 1);
    client->socket_info.send_buf.count = sizeof(response)+3;
    printf("Bytes to send: ");
    for (int i = 0; i < client->socket_info.send_buf.count; ++i) {
        printf("%.2x ", client->socket_info.send_buf.bytes[i]);
    }
    printf("\n");
}

/*
void HandleLoginStart(SOCKET *clientSock, Client *client, unsigned char* recvBuffer)
{
    printf("Login start!\n\n");

    pull_varint(recvBuffer, NULL);
    pull_varint(recvBuffer, NULL);

    pull_string(recvBuffer, NULL, client->nickname, &client->nickname_len);
    pull_uuid(recvBuffer, NULL, client->uuid);

    SendLoginSuccess(clientSock, client);
}


void SendLoginSuccess(SOCKET *client, Client *Client)
{
    printf("Loign validation start!\n\n");

    unsigned char send_buffer[BUFFER_SIZE] = {0};
    unsigned char data[BUFFER_SIZE] = {0};
    unsigned int packet_len = 0;
    unsigned int temp = 0;
    
    append_varint(data, &packet_len, 0x02);
    append_uuid(data, &packet_len, Client->uuid);
    append_string(data, &packet_len, Client->nickname, Client->nickname_len);
    append_varint(data, &packet_len, 0x00);

    append_varint(send_buffer, &temp, packet_len+1);
    strcat(send_buffer, data);

    send(*client, send_buffer , BUFFER_SIZE, 0);
}


void HandleConfingurationStart(SOCKET *clientSock, Client *Client, char* recvBuffer)
{
    printf("Started client configuration\n\n");
    pull_varint(recvBuffer, NULL);
    pull_varint(recvBuffer, NULL);

    pull_string(recvBuffer, NULL, Client->locale, &Client->locale_len);
    Client->view_distance = pull_byte(recvBuffer, NULL);
    Client->chat_mode = pull_varint(recvBuffer, NULL);
    Client->chat_colors = pull_byte(recvBuffer, NULL);
    Client->displayed_skin_parts = pull_byte(recvBuffer, NULL);
    Client->main_hand = pull_varint(recvBuffer, NULL);
    Client->enable_text_filtering = pull_byte(recvBuffer, NULL);
    Client->allow_server_listings = pull_byte(recvBuffer, NULL);

    SendConfigurationSucces(clientSock);
}


void SendConfigurationSucces(SOCKET *clientSock)
{
    printf("Finishing configuration\n");
    unsigned char send_buffer[2] = {0};
    unsigned int packet_len = 0;

    append_byte(send_buffer, &packet_len, 0x01);
    append_byte(send_buffer, &packet_len, 0x03);

    send(*clientSock, send_buffer, 2, 0);
    return;
}


void SendLogin(SOCKET *clientSock)
{
    printf("Login2 start!\n\n");

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

    send(*clientSock, send_buffer , BUFFER_SIZE, 0);
}
*/