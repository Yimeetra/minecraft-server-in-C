#include "Client.h"
#include <string.h>

Client client_new(SOCKET socket) {
    Client client = {0};
    client.game_state = HANDSHAKE;
    client.socket_info.recv_buf = ba_new(1024);
    client.socket_info.send_buf = ba_new(1024);
    client.socket_info.socket = socket;
    memset(client.nickname, 0, 16);
    memset(client.UUID, 0, 16);
    client.login_acknowledged = false;
    client.alive = true;
    return client;
}

void generate_uuid(char* nickname, unsigned char* result) {
    char temp[32];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}

void close_connection(Client* client) {
    shutdown(client->socket_info.socket, SD_BOTH);
    closesocket(client->socket_info.socket);
    *client = client_new(SOCKET_ERROR);
}