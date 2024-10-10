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
void HandleStatusRequest(Packet *packet, Client *client);
void SendStatusResponse(Packet *packet, Client *client);
void HandlePingRequest(Packet *packet, Client *client);
void SendPongResponse(Packet *packet, Client *client);
void HandleLoginStart(Packet *packet, Client *client);
void SendLoginSuccess(Packet *packet, Client *client);
void HandleLoginAckAcknowledged(Packet *packet, Client *client);
void SendKnownPacks(Packet *packet, Client *client);