#include <stdio.h>
#include <winsock2.h>
#include "md5.h"
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 256

#include "byte_array.h"

typedef struct {
    unsigned char nickname[16];
    int nickname_len;
    unsigned char uuid[16];
    unsigned char locale[16];
    int locale_len;
    char view_distance;
    char chat_mode;
    bool chat_colors;
    char displayed_skin_parts;
    char main_hand;
    bool enable_text_filtering;
    bool allow_server_listings;
} PLAYER;

void generate_uuid(char* nickname, unsigned char* result)
{
    char temp[16];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}


void handshake(unsigned char* recv_buffer, int *state)
{
    *state = recv_buffer[read_varint(recv_buffer)];
}


void login_start(SOCKET *client, PLAYER *player, unsigned char* recv_buffer)
{
    printf("Login start!\n\n");

    pull_varint(recv_buffer, NULL);
    pull_varint(recv_buffer, NULL);

    pull_string(recv_buffer, NULL, player->nickname, &player->nickname_len);
    pull_uuid(recv_buffer, NULL, player->uuid);
}


void login_success(SOCKET *client, PLAYER player)
{
    printf("Loign validation start!\n\n");
    unsigned char send_buffer[BUFFER_SIZE] = {0};
    unsigned char data[BUFFER_SIZE] = {0};
    unsigned int packet_len = 0;
    unsigned int temp = 0;

    //write_varint(temp, 0x02);
    //strcat(data, temp);
    //strcat(data, player.uuid);
    //write_varint(temp, player.nickname_len);
    //strcat(data, temp);
    //strcat(data, player.nickname);
    //write_varint(temp, 0x00);
    //strcat(data, temp);
    //write_varint(temp, 0x01);
    //strcat(data, temp);

    
    append_varint(data, &packet_len, 0x02);
    append_uuid(data, &packet_len, player.uuid);
    append_string(data, &packet_len, player.nickname, player.nickname_len);
    append_varint(data, &packet_len, 0x00);

    //write_varint(packet_len, 23+player.nickname_len);
    append_varint(send_buffer, &temp, packet_len+1);
    strcat(send_buffer, data);

    for (int i = 0 ; i < BUFFER_SIZE; ++i ) {
        printf("%.2x ", send_buffer[i]);
    }
    printf("\n");

    send(*client, send_buffer , BUFFER_SIZE, 0);
}


void client_information(char* recv_buffer, PLAYER *player)
{
    printf("Started client configuration\n\n");
    pull_varint(recv_buffer, NULL);
    pull_varint(recv_buffer, NULL);

    pull_string(recv_buffer, NULL, player->locale, &player->locale_len);
    player->view_distance = pull_byte(recv_buffer, NULL);
    player->chat_mode = pull_varint(recv_buffer, NULL);
    player->chat_colors = pull_byte(recv_buffer, NULL);
    player->displayed_skin_parts = pull_byte(recv_buffer, NULL);
    player->main_hand = pull_varint(recv_buffer, NULL);
    player->enable_text_filtering = pull_byte(recv_buffer, NULL);
    player->allow_server_listings = pull_byte(recv_buffer, NULL);
}


void finish_configuration(SOCKET *client)
{
    printf("Finishing configuration\n");
    unsigned char send_buffer[2] = {0};
    unsigned int packet_len = 0;

    append_byte(send_buffer, &packet_len, 0x01);
    append_byte(send_buffer, &packet_len, 0x03);

    send(*client, send_buffer, 2, 0);
    return;
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
        printf("Recieved packet: ");
        for (int i = 0; i < read_varint(recv_buffer)+3; ++i) {
            printf("%.2x ", recv_buffer[i]);
        }
        printf("\n");
        printf("Current server state: %d\n", state);
        printf("Packet id: %.2x\n\n", recv_buffer[1]);

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
                case 0x02:
                    client_information(recv_buffer, &player);
                    finish_configuration(&client);
                    break;
                case 0x03: state = 4; break;

                default: break;
            }
            break;








        default: break;
        }
    }

    
    WSACleanup();

    return 0;
}