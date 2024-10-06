#pragma once

#include <winsock2.h>
#include <stdbool.h>
#include "Packet.h"
#include "ByteArray.h"
#include "GameState.h"
#include "Client.h"

//typedef struct {
//    unsigned char nickname[16];
//    int nickname_len;
//    unsigned char uuid[16];
//    unsigned char locale[16];
//    int locale_len;
//    char view_distance;
//    char chat_mode;
//    bool chat_colors;
//    char displayed_skin_parts;
//    char main_hand;
//    bool enable_text_filtering;
//    bool allow_server_listings;
//} Client;

void HandleHandshake(Packet *packet, Client *client);
void HanldeStatusRequest(Packet *packet, Client *client);
void SendStatusResponse(Packet *packet, Client *client);
//void HandleLoginStart(SOCKET *clientSock, Client *client, unsigned char* recv_buffer);
//void SendLoginSuccess(SOCKET *clientSock, Client *client);
//void HandleConfingurationStart(SOCKET *clientSock, Client *Client, char* recvBuffer);
//void SendConfigurationSucces(SOCKET *clientSock);
//void SendLogin(SOCKET *clientSock);
