#pragma once

typedef enum {
    HANDSHAKE,
    STATUS,
    LOGIN,
    CONFIGURATION,
    PLAY,
} GameState;


static const char * const GameState_name[] = {
    [HANDSHAKE] = "HANDSHAKE",
    [STATUS] = "STATUS",
    [LOGIN] = "LOGIN",
    [CONFIGURATION] = "CONFIGURATION",
    [PLAY] = "PLAY",
};