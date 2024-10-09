#pragma once

#include <winsock2.h>
#include "GameState.h"
#include "ByteArray.h"

typedef struct {
    SOCKET socket;
    ByteArray recv_buf;
    ByteArray send_buf;
} SocketInfo;

typedef struct {
    SocketInfo socket_info;
    GameState game_state;
} Client;

Client client_new(SOCKET socket);