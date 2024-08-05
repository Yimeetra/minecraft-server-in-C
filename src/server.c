#include <stdio.h>
#include <winsock2.h>
#include "md5.h"
#include <string.h>

#define RECV_BUFFER_SIZE 256
#define SEND_BUFFER_SIZE 256
#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

typedef struct {
    unsigned char nickname[16];
    int nickname_len;
    unsigned char uuid[16];
} PLAYER;


int read_varint(unsigned char* varint)
{
    int value;
    int i = 0;
    while (1) {        
        value |= (varint[i] & SEGMENT_BITS) << i*7;
        if (!(varint[i] & CONTINUE_BIT)) {
            break;
        }
        i++;
    }
    return value;
}


void write_varint(unsigned char* varint, int value)
{
    memset(varint, 0, 5);
    int i = 0;
    while (1) {
        if (!(value & ~SEGMENT_BITS)) {
            varint[i] = value;
            break;
        };    
        varint[i] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (unsigned int) value >> 7;
        i++;
    }
    return;
}


void generate_uuid(char* nickname, unsigned char* result)
{
    char temp[32];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}


void handshake();
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
    unsigned char send_buffer[SEND_BUFFER_SIZE] = {0};
    unsigned char data[SEND_BUFFER_SIZE] = {0};
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

    for (int i = 0 ; i < SEND_BUFFER_SIZE; ++i ) {
        printf("%.2x ", send_buffer[i]);
    }
    printf("\n");

    send(*client, send_buffer , SEND_BUFFER_SIZE, 0);
}
void login_acknowledged();



int main()
{
    WSADATA wsadata;
    SOCKET server;
    SOCKET client;
    SOCKADDR_IN server_addr, client_addr;
    PLAYER player;
    unsigned char recv_buffer[RECV_BUFFER_SIZE] = {0};
    unsigned char send_buffer[RECV_BUFFER_SIZE] = {0};
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
        if (recv(client, recv_buffer, RECV_BUFFER_SIZE, 0) == SOCKET_ERROR) break;

        switch (state)
        {
        case 0:
            switch (recv_buffer[1])
            {
                case 0x00: state = 2; break; 
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
            case 0x03: state = 3;

            
            default:
                break;
            }
            break;

        case 3: 
            
            break;
        
        default: break;
        }
    }

    
    WSACleanup();

    return 0;
}