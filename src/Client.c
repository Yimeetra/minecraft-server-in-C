#include "Client.h"

Client client_new(SOCKET socket) {
    Client client;
    client.game_state = HANDSHAKE;
    client.socket_info.recv_buf = ba_new(1024);
    client.socket_info.send_buf = ba_new(1024);
    client.socket_info.socket = socket;
    
    return client;
}

void generate_uuid(char* nickname, unsigned char* result) {
    char temp[16];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}