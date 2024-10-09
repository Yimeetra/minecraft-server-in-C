#include "Client.h"

Client client_new(SOCKET socket) {
    Client client;
    client.game_state = HANDSHAKE;
    client.socket_info.recv_buf = ba_new(1024);
    client.socket_info.send_buf = ba_new(1024);
    client.socket_info.socket = socket;
    
    return client;
}