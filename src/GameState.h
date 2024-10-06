#pragma once

typedef enum {
    HANDSHAKE,
    STATUS,
    LOGIN,
    PLAY,
} GameState;


static const char * const GameState_name[] = {
    [HANDSHAKE] = "HANDSHAKE",
    [STATUS] = "STATUS",
    [LOGIN] = "LOGIN",
    [PLAY] = "PLAY",
};