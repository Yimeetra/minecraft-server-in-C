#include "Protocol.h"
#include <stdio.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "server.h"
#include "registries.h"

extern ServerSettings settings;
extern Registries registries;

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
    ba_append_string(&response_packet.data, "minecraft:dimension_type", 25);
    ba_append_varint(&response_packet.data, 1);
    ba_append_string(&response_packet.data, "minecraft:overworld", 20);
    ba_append_byte(&response_packet.data, false);

    response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);

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
}

void SendFinishConfiguration(Client *client) {
    ByteArray response = packet_to_bytearray(Packet_new(0x03));
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandleFinishConfigurationAcknowledged(Client *client) {
    client->game_state = PLAY;
    SendPlayLogin(client);
    SendGameEvent(client, START_WAITING_FOR_CHUNKS, 0);
    SendSyncronisePlayerPosition(client, 0, 0, 0, 0, 0, 0b00011111);
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
    ba_append_varint(&response_packet.data, 1);

    ByteArray response = packet_to_bytearray(response_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}