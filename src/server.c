#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdbool.h>
#include "ByteArray.h"
#include "md5.h"
//#include "Protocol.h"
#include "Packet.h"

#define BUFFER_SIZE 1024

typedef enum {
    HANDSHAKE,
    STATUS,
    LOGIN,
    PLAY,
} GameState;


WSADATA wsadata;

TIMEVAL tv = {0};

GameState state = 0;

ByteArray recv_buffer = {0};
ByteArray send_buffer = {0};

SOCKET server;
SOCKET client;
SOCKADDR_IN server_addr, client_addr;
fd_set fd_in, fd_out;


void generate_uuid(char* nickname, unsigned char* result)
{
    char temp[16];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}

void close_connection(SOCKET* client) {
    shutdown(*client, SD_BOTH);
    closesocket(*client);
    *client = SOCKET_ERROR;
}

void handle_packet(Packet* packet) {
    switch (state) {
        case HANDSHAKE:
            switch (packet->id) {
                default:
                    printf("Unimplemented packet with id %d for HANDSHAKE game state\n", packet->id);
                    close_connection(&client);
                    break;
            }
            break;
        case STATUS:
            switch (packet->id) {
                default:
                    printf("Unimplemented packet with id %d for STATUS game state\n", packet->id);
                    close_connection(&client);
                    break;
            }
            break;
        case LOGIN:
            switch (packet->id) {
                default:
                    printf("Unimplemented packet with id %d for LOGIN game state\n", packet->id);
                    close_connection(&client);
                    break;
            }
            break;
        case PLAY:
            switch (packet->id) {
                default:
                    printf("Unimplemented packet with id %d for PLAY game state\n", packet->id);
                    close_connection(&client);
                    break;
            }
            break;
        default:
            printf("Unexpected game state\n");
            close_connection(&client);
            break;
    }
}

int main()
{
    // init WSA
    WSAStartup(MAKEWORD(2,2), &wsadata);

    ba_new(&recv_buffer, BUFFER_SIZE);
    ba_new(&send_buffer, BUFFER_SIZE);

    server = socket(AF_INET, SOCK_STREAM, 0);
    client = SOCKET_ERROR;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    server_addr.sin_port = htons(25565);

    bind(server,(SOCKADDR*) &server_addr, sizeof(server_addr));
    listen(server, 0);
    printf("Server is running on port 25565\n");
    
    while (1) {
        FD_ZERO(&fd_in);
        FD_ZERO(&fd_out);

        FD_SET(server, &fd_in);

        if (client != SOCKET_ERROR) {
            FD_SET(client, &fd_in);
            FD_SET(client, &fd_out);
        }

        int select_result = select(0, &fd_in, &fd_out, NULL, &tv);
        if (select_result > 0) {
            if (FD_ISSET(server, &fd_in)) {
                printf("Incomming connection\n");
                client = accept(server, NULL, NULL);        
            }

            if (FD_ISSET(client, &fd_in)) {
                if (recv(client, recv_buffer.bytes, BUFFER_SIZE, 0) <= 0) {
                    printf("Connection closed\n");
                    close_connection(&client);
                }
                Packet recieved_packet = {0};
                parse_packet(&recieved_packet, &recv_buffer);

                printf("Received packet:\n");
                printf("  Length = %i\n", recieved_packet.length);
                printf("  Id = %i\n", recieved_packet.id);
                printf("  Data = ");
                for (int i = 0; i < recieved_packet.length; ++i) {
                    printf("%.2x ", recieved_packet.data->bytes[i]);
                }
                printf("\n");
                printf("Current server state: %d\n\n", state);

                handle_packet(&recieved_packet);
            }
        } if (select_result == -1) {
            printf("Socket error: %d\n", WSAGetLastError());
            break;
        }
        /*
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
                case 0x03: 
                    state = 4; 
                    SendLogin(&client);
                    
                    break;
                default: break;
            }
            break;

        default: break;
        }
        */
    }

    
    WSACleanup();

    return 0;
}