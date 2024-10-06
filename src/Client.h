#pragma once

#include <winsock2.h>
#include "GameState.h"
#include "ByteArray.h"

typedef struct {
    SOCKET socket;
    ByteArray data_buf;
    int bytes_send;
    int bytes_recv;
} SocketInfo;

typedef struct {
    SocketInfo socket_info;
    GameState game_state;
} Client;

Client client_new(SOCKET socket);