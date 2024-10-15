#pragma once

#include <winsock2.h>
#include "GameState.h"
#include "ByteArray.h"
#include "md5.h"
#include <stdbool.h>
#include <pthread.h>

typedef struct {
    SOCKET socket;
    ByteArray recv_buf;
    ByteArray send_buf;
} SocketInfo;

typedef struct {
    SocketInfo socket_info;
    GameState game_state;
    pthread_t tid;
    bool alive;
    time_t last_keepalive;
    char nickname[17];
    char UUID[17];
    bool login_acknowledged;
    double x;
    double y;
    double z;
    float pitch;
    float yaw;
    bool on_ground;
} Client;

Client client_new(SOCKET socket);
void generate_uuid(char* nickname, unsigned char* result);
void close_connection(Client* client);