#include "Protocol.h"
#include <stdio.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "server.h"
#include "registries.h"
#include "Chunk.h"
#include "NBT.h"
#include "Teleportation.h"

extern ServerSettings settings;
extern Registries registries;
extern Teleportation teleportations[1024];

void print_packest(Packet packet) {
    printf("Parsed packet:\n");
    printf("  Length = %i\n", packet.length);
    printf("  Id = %i\n", packet.id);
    printf("  Data:\n");
    printf("    Length = %i\n", packet.data.length);
    printf("    Data = ");
    for (int i = 0; i < packet.data.length; ++i) {
        printf("%.2x ", packet.data.bytes[i]);
    }
    printf("\n\n");
}

void HandleHandshake(Packet *packet, Client *client)
{
    client->game_state = packet->data.bytes[packet->data.length-1];
}

void HandleStatusRequest(Client *client) {
    SendStatusResponse(client);
}

void SendStatusResponse(Client *client) {
    Byte response_text[] = "{\"version\":{\"name\":\"1.21\",\"protocol\":767}}";

    Packet response_packet = Packet_new(0x00);
    ba_append_string(&response_packet.data, response_text, sizeof(response_text));

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandlePingRequest(Packet *packet, Client *client) {
    SendPongResponse(packet, client);
}

void SendPongResponse(Packet *packet, Client *client) {
    ByteArray response = packet_to_bytearray(*packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandleLoginStart(Packet *packet, Client *client) {
    ba_pull_string(&packet->data, client->nickname, NULL);
    generate_uuid(client->nickname, client->UUID);
    SendLoginSuccess(client);
}

void SendLoginSuccess(Client *client) {
    client->login_acknowledged = true;
    Packet response_packet = Packet_new(0x02);
    ba_append(&response_packet.data, client->UUID, 16);
    ba_append_string(&response_packet.data, client->nickname, 16);
    ba_append_varint(&response_packet.data, 0);
    ba_append_bool(&response_packet.data, 0x00);

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandleLoginAckAcknowledged(Client *client) {
    if (!client->login_acknowledged) {
        close_connection(client);
        return;
    }
    client->game_state = CONFIGURATION;
    SendKnownPacks(client);
    SendAllRegistryData(client);
    SendFinishConfiguration(client);
}

void SendKnownPacks(Client *client) {
    Packet response_packet = Packet_new(0x0E);
    ba_append_varint(&response_packet.data, 1);
    ba_append_string(&response_packet.data, "minecraft", 10);
    ba_append_string(&response_packet.data, "core", 5);
    ba_append_string(&response_packet.data, "1.21", 5);

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void SendAllRegistryData(Client *client) {
    Packet response_packet;
    ByteArray response;

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:painting_variant", 27);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(PAINTING_VARIANT));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == PAINTING_VARIANT) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.paintingVariant.name, strlen(registries.registries[i].registry.paintingVariant.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:wolf_variant", 23);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(WOLF_VARIANT));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == WOLF_VARIANT) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.wolfVariant.name, strlen(registries.registries[i].registry.wolfVariant.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:worldgen/biome", 25);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(WORLDGEN_BIOME));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == WORLDGEN_BIOME) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.worldgenBiome.name, strlen(registries.registries[i].registry.worldgenBiome.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:banner_pattern", 25);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(BANNER_PATTERN));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == BANNER_PATTERN) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.bannerPattern.name, strlen(registries.registries[i].registry.bannerPattern.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:chat_type", 20);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(CHAT_TYPE));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == CHAT_TYPE) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.chatType.name, strlen(registries.registries[i].registry.chatType.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:damage_type", 22);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(DAMAGE_TYPE));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == DAMAGE_TYPE) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.damageType.name, strlen(registries.registries[i].registry.damageType.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:dimension_type", 25);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(DIMENSION_TYPE));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == DIMENSION_TYPE) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.dimensionType.name, strlen(registries.registries[i].registry.dimensionType.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:trim_material", 24);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(TRIM_MATERIAL));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == TRIM_MATERIAL) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.trimMaterial.name, strlen(registries.registries[i].registry.trimMaterial.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);   

    response_packet = Packet_new(0x07);
    ba_append_string(&response_packet.data, "minecraft:trim_pattern", 23);
    ba_append_varint(&response_packet.data, Registries_get_registry_count(TRIM_PATTERN));

    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == TRIM_PATTERN) {
            ba_append_string(&response_packet.data, registries.registries[i].registry.trimPattern.name, strlen(registries.registries[i].registry.trimPattern.name)+1);
            ba_append_bool(&response_packet.data, false);
        } 
    }
    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);     
}

void SendFinishConfiguration(Client *client) {
    ByteArray response = packet_to_bytearray(Packet_new(0x03));
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandleFinishConfigurationAcknowledged(Client *client) {
    client->game_state = PLAY;
    SendPlayLogin(client);
    SendSyncronisePlayerPosition(client, 0, 0, 0, 0, 0, 0b00011111);
    SendGameEvent(client, START_WAITING_FOR_CHUNKS, 0);
    SendSetCenterChunk(client, 0, 0);
}

void SendPlayLogin(Client *client) {
    Packet response_packet = Packet_new(0x2B);

    ba_append_int(&response_packet.data, 1);
    ba_append_bool(&response_packet.data, settings.is_hardcore);
    ba_append_varint(&response_packet.data, settings.dimension_count);
    ba_append_string(&response_packet.data, settings.dimension_name, strlen(settings.dimension_name)+1);
    ba_append_varint(&response_packet.data, settings.max_players);
    ba_append_varint(&response_packet.data, settings.view_distance);
    ba_append_varint(&response_packet.data, settings.simulation_distance);
    ba_append_bool(&response_packet.data, settings.reduced_debug_info);
    ba_append_bool(&response_packet.data, settings.enable_respawn_screen);
    ba_append_bool(&response_packet.data, settings.do_limited_crafting);
    ba_append_varint(&response_packet.data, 0);
    ba_append_string(&response_packet.data, settings.dimension_name, strlen(settings.dimension_name)+1);
    ba_append_int(&response_packet.data, 0xffffffff);
    ba_append_int(&response_packet.data, 0xffffffff);
    ba_append_byte(&response_packet.data, 0x00);
    ba_append_byte(&response_packet.data, 0x00);
    ba_append_bool(&response_packet.data, false);
    ba_append_bool(&response_packet.data, false);
    ba_append_bool(&response_packet.data, false);
    ba_append_varint(&response_packet.data, 0);
    ba_append_bool(&response_packet.data, settings.enforces_secure_chat);

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void SendGameEvent(Client *client, GameEvent event, float value) {
    Packet response_packet = Packet_new(0x22);
    ba_append_byte(&response_packet.data, event);
    ba_append_int(&response_packet.data, value);

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void SendSyncronisePlayerPosition(Client *client, double x, double y, double z, float pitch, float yaw, byte flags) {
    Packet response_packet = Packet_new(0x40);
    ba_append_double(&response_packet.data, x);
    ba_append_double(&response_packet.data, y);
    ba_append_double(&response_packet.data, z);
    ba_append_float(&response_packet.data, pitch);
    ba_append_float(&response_packet.data, yaw);
    ba_append_byte(&response_packet.data, flags);
    for (int i = 0; i < 1024; ++i) {
        if (teleportations[i].ack) {
            teleportations[i].ack = false;
            teleportations[i].x = x;
            teleportations[i].y = y;
            teleportations[i].z = z;
            teleportations[i].pitch = pitch;
            teleportations[i].yaw = yaw;
            ba_append_varint(&response_packet.data, i);
            break;
        }
    }

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void SendChunkDataAndUpdateLight(Client *client, Chunk chunk) {
    Packet response_packet = Packet_new(0x27);
    
    // X and Z fields
    ba_append_int(&response_packet.data, chunk.x);
    ba_append_int(&response_packet.data, chunk.z);

    // Heightmaps NBT
    ba_append_byte(&response_packet.data, 0x0A);
    ba_append_byte(&response_packet.data, 0x09);
    ba_append_string(&response_packet.data, "MOTION_BLOCKING", 16);
    ba_append_int(&response_packet.data, 0);
    ba_append_byte(&response_packet.data, 0x09);
    ba_append_string(&response_packet.data, "MOTION_BLOCKING", 16);
    ba_append_int(&response_packet.data, 0);
    ba_append_byte(&response_packet.data, 0x00);
    
    ByteArray chunk_data = ba_new(0);
    for (int i = 0; i < 24; ++i) {
        ChunkSection section = chunk.sections[i];
    }

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void SendSetCenterChunk(Client *client, int x, int z) {
    Packet response_packet = Packet_new(0x54);
    ba_append_varint(&response_packet.data, x);
    ba_append_varint(&response_packet.data, z);

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandleConfirmTeleportation(Packet *packet, Client *client) {
    int tp_id = ba_read_varint(&packet->data);
    teleportations[tp_id].ack = true;
    client->x = teleportations[tp_id].x;
    client->y = teleportations[tp_id].y;
    client->z = teleportations[tp_id].z;
    client->pitch = teleportations[tp_id].pitch;
    client->yaw = teleportations[tp_id].yaw;
}

void HandleSetPlayerPosition(Packet *packet, Client *client) {
    client->x = ba_pull_double(&packet->data);
    client->y = ba_pull_double(&packet->data) + 1.62;
    client->z = ba_pull_double(&packet->data);
    client->on_ground = ba_pull_bool(&packet->data);
}

void HandleSetPlayerRotation(Packet *packet, Client *client) {
    client->yaw = ba_pull_float(&packet->data);
    client->pitch = ba_pull_float(&packet->data);
    client->on_ground = ba_pull_bool(&packet->data);
}

void HandleSetPlayerPositionAndRotation(Packet *packet, Client *client) {
    client->x = ba_pull_double(&packet->data);
    client->y = ba_pull_double(&packet->data) + 1.62;
    client->z = ba_pull_double(&packet->data);
    client->yaw = ba_pull_float(&packet->data);
    client->pitch = ba_pull_float(&packet->data);
    client->on_ground = ba_pull_bool(&packet->data);
}

void SendPlayKeepAlive(Client *client) {
    if (!client->alive) return;
    Packet response_packet = Packet_new(0x26);
    time_t timestamp = time(0);
    ba_append_long(&response_packet.data, timestamp);
    client->alive = false;
    client->last_keepalive = timestamp;

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandlePlayKeepAlive(Packet *packet, Client *client) {
    if (ba_read_long(&packet->data) == client->last_keepalive) {
        client->alive = true;
    }
}