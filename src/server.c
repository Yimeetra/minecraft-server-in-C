#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdbool.h>
#include "defines.h"
#include "ByteArray.h"
#include "md5.h"
#include "Protocol.h"

void generate_uuid(char* nickname, unsigned char* result)

{
    char temp[16];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}

int main()
{
    WSADATA wsadata;
    SOCKET server;
    SOCKET client;
    SOCKADDR_IN server_addr, client_addr;
    Client player;
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
        for (int i = 0; i < read_varint(recv_buffer)+1; ++i) {
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
                case 0x00: Handshake(recv_buffer, &state); break; 
                default: break;
            }
            break;

        case 1: break;

        case 2: 
            switch (recv_buffer[1])
            {
                case 0x00: HandleLoginStart(&client, &player, recv_buffer); break;
                case 0x03: state = 3; break;
                default: break;
            }
            break;

        case 3: 
            switch (recv_buffer[1])
            {
                case 0x02: HandleConfingurationStart(&client, &player, recv_buffer); break;
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