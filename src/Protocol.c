#include "Protocol.h"
#include <stdio.h>
#include <string.h>
#include "cJSON/cJSON.h"

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

void HandleStatusRequest(Packet *packet, Client *client) {
    SendStatusResponse(packet, client);
}

void SendStatusResponse(Packet *packet, Client *client) {
    byte response_text[] = "{\"version\":{\"name\":\"1.21\",\"protocol\":767}}";

    Packet respone_packet = Packet_new(0x00);
    ba_append_string(&respone_packet.data, response_text, sizeof(response_text));

    ByteArray response = packet_to_bytearray(respone_packet);
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
    SendLoginSuccess(packet, client);
}

void SendLoginSuccess(Packet *packet, Client *client) {
    client->login_acknowledged = true;
    Packet respone_packet = Packet_new(0x02);
    ba_append(&respone_packet.data, client->UUID, 16);
    ba_append_string(&respone_packet.data, client->nickname, 16);
    ba_append_varint(&respone_packet.data, 0);
    ba_append_bool(&respone_packet.data, 0x00);

    ByteArray response = packet_to_bytearray(respone_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void HandleLoginAckAcknowledged(Packet *packet, Client *client) {
    if (!client->login_acknowledged) {
        close_connection(client);
        return;
    }
    client->game_state = CONFIGURATION;
    SendKnownPacks(packet, client);
    SendAllRegistryData(packet, client);
    SendFinishConfiguration(packet, client);
    SendUpdateTags(packet, client);
}

void SendKnownPacks(Packet *packet, Client *client) {
    Packet respone_packet = Packet_new(0x0E);
    ba_append_varint(&respone_packet.data, 1);
    ba_append_string(&respone_packet.data, "minecraft", 10);
    ba_append_string(&respone_packet.data, "core", 5);
    ba_append_string(&respone_packet.data, "1.21", 5);

    ByteArray response = packet_to_bytearray(respone_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void SendAllRegistryData(Packet *packet, Client *client) {
    FILE *f = fopen("registries.json", "r");

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;

    cJSON *registries = cJSON_Parse(string);
    free(string);

    cJSON *registry = NULL;
    cJSON_ArrayForEach(registry, registries) {
        Packet response_packet = Packet_new(0x07);
        char *temp;
        temp = strtok(registry->string, ":");
        ba_append_string(&response_packet.data, temp, strlen(temp)+1);
        temp = strtok(NULL, ":");
        ba_append_string(&response_packet.data, temp, strlen(temp)+1);
        ba_append_varint(&response_packet.data, cJSON_GetArraySize(registry));
        cJSON *entry = NULL;
        cJSON_ArrayForEach(entry, registry) {
            char *temp;
            temp = strtok(entry->string, ":");
            ba_append_string(&response_packet.data, temp, strlen(temp)+1);
            temp = strtok(NULL, ":");
            ba_append_string(&response_packet.data, temp, strlen(temp)+1);
            ba_append_bool(&response_packet.data, 0x00);
        }
        ByteArray response = packet_to_bytearray(response_packet);
        ba_append(&client->socket_info.send_buf, response.bytes, response.count);
    }
}

void SendFinishConfiguration(Packet *packet, Client *client) {
    ByteArray response = packet_to_bytearray(Packet_new(0x03));
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}

void SendUpdateTags(Packet *packet, Client *client) {
    Packet respone_packet = Packet_new(0x0D);
    ba_append_varint(&respone_packet.data, 0);

    ByteArray response = packet_to_bytearray(respone_packet);
    ba_append(&client->socket_info.send_buf, response.bytes, response.count);
}