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
#include <stdbool.h>
#include "server.h"
#include "registries.h"
#include "Teleportation.h"
#include <time.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

WSADATA wsadata;
TIMEVAL tv = {0,10};

SOCKET server;
SOCKADDR_IN server_addr;

Client clients[MAX_CLIENTS];
PacketQueue send_queue;
ServerSettings settings;
Teleportation teleportations[1024] = {[0 ... 1023] = {true, 0, 0, 0, 0, 0}};
fd_set fd_in, fd_out;
int select_result = 0;


void handle_client(Client *client) {
    time_t tick = time(0);

    if (FD_ISSET(client->socket_info.socket, &fd_in)) {
        if (client->socket_info.recv_buf.count <= 0) {
            client->socket_info.recv_buf.count = recv(client->socket_info.socket, client->socket_info.recv_buf.bytes, BUFFER_SIZE, 0);
            if (client->socket_info.recv_buf.count <= 0) {
                printf("Connection closed\n");
                close_connection(client);
            }
        }
        while (client->socket_info.recv_buf.count > 0) {
            Packet recieved_packet = parse_packet(client->socket_info.recv_buf);
            ba_shift(&client->socket_info.recv_buf, recieved_packet.full_length);
            handle_packet(&recieved_packet, client);
        }
    }

    if (FD_ISSET(client->socket_info.socket, &fd_out)) {
        while (client->socket_info.send_buf.count > 0) {
            Packet packet = parse_packet(client->socket_info.send_buf);
            send(client->socket_info.socket, client->socket_info.send_buf.bytes, packet.full_length, 0);
            ba_shift(&client->socket_info.send_buf, packet.full_length);
        }
    }

    if (client->game_state == PLAY) {
        SendPlayKeepAlive(client);

        if (tick-client->last_keepalive >= 15) {
            close_connection(client);
        }
    
    }
}

int main() {
    settings.is_hardcore = false;
    settings.dimension_count = 1;
    settings.dimension_name = "minecraft:overworld";
    settings.max_players = MAX_CLIENTS;
    settings.view_distance = 4;
    settings.simulation_distance = 4;
    settings.reduced_debug_info = true;
    settings.enable_respawn_screen = true;
    settings.do_limited_crafting = false;
    settings.hashed_seed = 0x0000000000000000;
    settings.enforces_secure_chat = false;

    init_registries();

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
                FD_SET(clients[i].socket_info.socket, &fd_out);
                FD_SET(clients[i].socket_info.socket, &fd_in);
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
                if (client->socket_info.socket == SOCKET_ERROR) continue;
                handle_client(client);
            }
        } else if (select_result < 0) {
            printf("Select error\n");
        }
    }

    WSACleanup();

    return 0;
}