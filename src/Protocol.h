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

typedef enum {
    START_WAITING_FOR_CHUNKS = 13,
    TOP
} GameEvent;

void HandleHandshake(Packet *packet, Client *client);
void HandleStatusRequest(Client *client);
void SendStatusResponse(Client *client);
void HandlePingRequest(Packet *packet, Client *client);
void SendPongResponse(Packet *packet, Client *client);
void HandleLoginStart(Packet *packet, Client *client);
void SendLoginSuccess(Client *client);
void HandleLoginAckAcknowledged(Client *client);
void SendKnownPacks(Client *client);
void SendAllRegistryData(Client *client);
void SendFinishConfiguration(Client *client);
void SendUpdateTags(Client *client);
void HandleFinishConfigurationAcknowledged(Client *client);
void SendPlayLogin(Client *client);
void SendGameEvent(Client *client, GameEvent event, float value);
void SendSyncronisePlayerPosition(Client *client, double x, double y, double z, float pitch, float yaw, byte flags);
void SendSetCenterChunk(Client *client, int x, int z);
void HandleConfirmTeleportation(Packet *packet, Client *client);
void HandleSetPlayerPosition(Packet *packet, Client *client);
void HandleSetPlayerRotation(Packet *packet, Client *client);
void HandleSetPlayerPositionAndRotation(Packet *packet, Client *client);
void SendPlayKeepAlive(Client *client);
void HandlePlayKeepAlive(Packet *packet, Client *client);