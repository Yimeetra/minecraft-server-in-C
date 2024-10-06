#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdbool.h>
#include "ByteArray.h"
#include "md5.h"
#include "GameState.h"
#include "Protocol.h"
#include "Packet.h"
#include "Client.h"

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

WSADATA wsadata;

TIMEVAL tv = {0,1};

SOCKET server;
SOCKADDR_IN server_addr, client_addr;
fd_set fd_in, fd_out;

Client clients[MAX_CLIENTS];

void generate_uuid(char* nickname, unsigned char* result)
{
    char temp[16];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}

void close_connection(Client* client) {
    shutdown(client->socket_info.socket, SD_BOTH);
    closesocket(client->socket_info.socket);
    *client = client_new(SOCKET_ERROR);
}

void handle_packet(Packet* packet, Client* client) {
    switch (client->game_state) {
        case HANDSHAKE:
            switch (packet->id) {
                case 0x00: HandleHandshake(packet, client); break;
                default:
                    printf("Unimplemented packet with id %d for HANDSHAKE game state\n", packet->id);
                    close_connection(client);
                    break;
            }
            break;
        case STATUS:
            switch (packet->id) {
                case 0x00: HanldeStatusRequest(packet, client); break;
                default:
                    printf("Unimplemented packet with id %d for STATUS game state\n", packet->id);
                    close_connection(client);
                    break;
            }
            break;
        case LOGIN:
            switch (packet->id) {
                default:
                    printf("Unimplemented packet with id %d for LOGIN game state\n", packet->id);
                    close_connection(client);
                    break;
            }
            break;
        case PLAY:
            switch (packet->id) {
                default:
                    printf("Unimplemented packet with id %d for PLAY game state\n", packet->id);
                    close_connection(client);
                    break;
            }
            break;
        default:
            printf("Unexpected game state\n");
            close_connection(client);
            break;
    }
}

int main()
{
    // init WSA
    WSAStartup(MAKEWORD(2,2), &wsadata);

    server = socket(AF_INET, SOCK_STREAM, 0);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i] = client_new(SOCKET_ERROR);
    }

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

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i].socket_info.socket != SOCKET_ERROR) {
                if (clients[i].socket_info.bytes_send>clients[i].socket_info.bytes_recv) {
                    FD_SET(clients[i].socket_info.socket, &fd_out);
                } else {
                    FD_SET(clients[i].socket_info.socket, &fd_in);
                }
            }
        }

        int select_result = select(0, &fd_in, &fd_out, NULL, &tv);
        if (select_result > 0) {
            if (FD_ISSET(server, &fd_in)) {
                printf("Incomming connection\n");
                for (int i = 0; i < MAX_CLIENTS; ++i) {
                    if (clients[i].socket_info.socket == SOCKET_ERROR) {
                        clients[i] = client_new(accept(server, NULL, NULL));
                        break;
                    }
                }
            }

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            Client* client = &clients[i];
            if (FD_ISSET(client->socket_info.socket, &fd_in)) {
                if (recv(client->socket_info.socket, client->socket_info.data_buf.bytes, BUFFER_SIZE, 0) <= 0) {
                    printf("Connection closed\n");
                    close_connection(client);
                    break;
                }

                Packet recieved_packet = {0};
                parse_packet(&recieved_packet, &client->socket_info.data_buf);

                printf("Parsed packet:\n");
                printf("  Length = %i\n", recieved_packet.length);
                printf("  Id = %i\n", recieved_packet.id);
                printf("  Data:\n");
                printf("    Length = %i\n", recieved_packet.data->length);
                printf("    Data = ");

                for (int i = 0; i < recieved_packet.data->length; ++i) {
                    printf("%.2x ", recieved_packet.data->bytes[i]);
                }
                printf("\n");

                printf("Current server state: %s\n\n", GameState_name[client->game_state]);
                //handle_packet(&recieved_packet, client);
                printf("Current server state: %s\n\n", GameState_name[client->game_state]);

            }

            if (FD_ISSET(client->socket_info.socket, &fd_out)) {
                send(client->socket_info.socket, client->socket_info.data_buf.bytes, client->socket_info.data_buf.count, 0);
            }
        }
        } if (select_result == -1) {
            printf("Socket error: %d\n", WSAGetLastError());
            break;
        }
        //printf("[");
        //for (int i = 0; i < MAX_CLIENTS; ++i) {
        //    printf("(%s ", GameState_name[clients[i].game_state]);
        //    printf("%d)", clients[i].socket_info.socket);
        //}
        //printf("]\n");


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