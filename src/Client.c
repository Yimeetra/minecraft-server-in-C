#include "Client.h"

Client client_new(SOCKET socket) {
    Client client;
    client.game_state = HANDSHAKE;
    client.socket_info.bytes_recv = 0;
    client.socket_info.bytes_send = 0;
    client.socket_info.data_buf = ba_new(1024);
    client.socket_info.socket = socket;
    
    return client;
}