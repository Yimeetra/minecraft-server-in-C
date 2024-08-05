#include <stdio.h>
#include <winsock2.h>
#include "md5.h"
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 256
#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

typedef struct {
    unsigned char nickname[16];
    int nickname_len;
    unsigned char uuid[16];
    unsigned char locale[16];
    char view_distance;
    char chat_mode;
    bool chat_colors;
    char displayed_skin_parts;
    char main_hand;
    bool enable_text_filtering;
    bool allow_server_listings;
} PLAYER;


int read_varint(unsigned char* buffer, int *buf_len)
{
    int value;
    unsigned char temp[BUFFER_SIZE];

    int i = 0;
    while (1) {        
        value |= (buffer[i] & SEGMENT_BITS) << i*7;
        if (!(buffer[i] & CONTINUE_BIT)) {
            break;
        }
        i++;
    }
    *buf_len -= i+1;
    for (int j = i; j < BUFFER_SIZE; ++j) {
        temp[j-i-1] = buffer[j];
    }
    memcpy(buffer, temp, BUFFER_SIZE);
    return value;
}


void write_varint(unsigned char* buffer, int *buf_len, int value)
{
    int i = *buf_len;
    while (1) {
        if (!(value & ~SEGMENT_BITS)) {
            buffer[i] = value;
            break;
        };    
        buffer[i] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (unsigned int) value >> 7;
        i++;
    }
    *buf_len = i+1;
    return;
}

void read_string(unsigned char* buffer, int *buf_len, unsigned char* out, int *out_len)
{
    unsigned char temp[BUFFER_SIZE];

    *out_len = buffer[0];
    for (int i = 0; i < *out_len; ++i) {
        out[i] = buffer[i+1];
    }
    *buf_len -= *out_len;
    for (int j = *out_len; j < BUFFER_SIZE; ++j) {
        temp[j-*out_len-1] = buffer[j];
    }
    memcpy(buffer, temp, BUFFER_SIZE);
    return;
}

void write_string(unsigned char* buffer, int *buf_len, unsigned char* in, int in_len)
{
    buffer[*buf_len] = in_len;
    for (int i = 0; i < in_len; ++i) {
        buffer[*buf_len+i+1] = in[i];
    }
    *buf_len += in_len+1;
    return;
}


void generate_uuid(char* nickname, unsigned char* result)
{
    char temp[32];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}


void handshake(char* recv_buffer, int *state)
{
    int i = read_varint(recv_buffer);
    *state = recv_buffer[i];
}


void login_start(SOCKET *client, PLAYER *player, char* recv_buffer)
{
    player->nickname_len = (int) recv_buffer[2];
    for (int i = 0; i < player->nickname_len; ++i) {
        player->nickname[i] = recv_buffer[i+3];
    }
    for (int i = 0; i < 16; ++i) {
        player->uuid[i] = recv_buffer[3+player->nickname_len+i];
    }

}


void login_success(SOCKET *client, PLAYER player)
{
    unsigned char send_buffer[BUFFER_SIZE] = {0};
    unsigned char data[BUFFER_SIZE] = {0};
    unsigned char packet_len[5];
    unsigned char temp[5];

    write_varint(temp, 0x02);
    strcat(data, temp);
    strcat(data, player.uuid);
    write_varint(temp, player.nickname_len);
    strcat(data, temp);
    strcat(data, player.nickname);
    write_varint(temp, 0x00);
    strcat(data, temp);
    write_varint(temp, 0x01);
    strcat(data, temp);

    write_varint(packet_len, 23+player.nickname_len);
    strcat(send_buffer, packet_len);
    strcat(send_buffer, data);

    for (int i = 0 ; i < BUFFER_SIZE; ++i ) {
        printf("%.2x ", send_buffer[i]);
    }
    printf("\n");

    send(*client, send_buffer , BUFFER_SIZE, 0);
}


void client_information(char* recv_buffer, PLAYER *player)
{
    int packet_len = read_varint(recv_buffer);
    int pointer = 3;
    
    for (int i = 0; i < recv_buffer[2]; ++i) {
        player->locale[i] = recv_buffer[pointer+i];
    }
    pointer += recv_buffer[2];
    player->view_distance = read_varint()

}



int main()
{
    WSADATA wsadata;
    SOCKET server;
    SOCKET client;
    SOCKADDR_IN server_addr, client_addr;
    PLAYER player;
    unsigned char recv_buffer[BUFFER_SIZE] = {0};
    unsigned char send_buffer[BUFFER_SIZE] = {0};
    unsigned char nickname[16];
    int nickname_len;

    int state = 0;

    WSAStartup(MAKEWORD(2,2), &wsadata);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    server_addr.sin_port = htons(25565);

    server = socket(AF_INET, SOCK_STREAM, 0);
    bind(server,(SOCKADDR*) &server_addr, sizeof(server_addr));
    listen(server, 0);

    client = accept(server, NULL, NULL);
    
    while (1) {
        if (recv(client, recv_buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) break;

        switch (state)
        {
        case 0:
            switch (recv_buffer[1])
            {
                case 0x00: handshake(recv_buffer, &state); break; 
                default: break;
            }
            break;

        case 1: break;

        case 2: 
            switch (recv_buffer[1])
            {
                case 0x00:
                    login_start(&client, &player, recv_buffer);
                    login_success(&client, player);
                    break;
                case 0x03: state = 3; break;

                default: break;
            }
            break;

        case 3: 
            switch (recv_buffer[1])
            {
                case 0x00:
                    client_information(recv_buffer, &player);
                    break;
            
                default: break;
            }
        }
    }

    
    WSACleanup();

    return 0;
}