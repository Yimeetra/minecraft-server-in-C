#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdbool.h>
#include "ByteArray.h"
#include "md5.h"
//#include "Protocol.h"
#include "Packet.h"

#define BUFFER_SIZE 1024

void generate_uuid(char* nickname, unsigned char* result)
{
    char temp[16];
    strcat(temp, "OfflinePlayer:");
    strcat(temp, nickname);
    md5String(temp, result);
}

int main()
{
    WSADATA wsadata;
    SOCKET server;
    SOCKET client;
    SOCKADDR_IN server_addr, client_addr;
    //Client player;
    ByteArray recv_buffer = {0};
    ByteArray send_buffer = {0};

    ba_new(&recv_buffer, BUFFER_SIZE);
    ba_new(&send_buffer, BUFFER_SIZE);

    int state = 0;

    WSAStartup(MAKEWORD(2,2), &wsadata);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    server_addr.sin_port = htons(25565);

    server = socket(AF_INET, SOCK_STREAM, 0);
    bind(server,(SOCKADDR*) &server_addr, sizeof(server_addr));
    listen(server, 0);

    printf("Server is running on port 25565\n");

    client = accept(server, NULL, NULL);

    
    while (1) {
        if (recv(client, recv_buffer.bytes, BUFFER_SIZE, 0) == 0) {
            printf("Client closed connection\n");
            break;
        }
        recv_buffer.length = ba_read_varint(&recv_buffer);
        recv_buffer.count = ba_read_varint(&recv_buffer);

        Packet recieved_packet = {0};
        parse_packet(&recieved_packet, &recv_buffer);

        printf("Received packet:\n");
        printf("  Length = %i\n", recieved_packet.length);
        printf("  Id = %i\n", recieved_packet.id);
        printf("  Data = ");
        for (int i = 0; i < recieved_packet.length; ++i) {
            printf("%.2x ", recieved_packet.data->bytes[i]);
        }
        printf("\n");
        printf("Current server state: %d\n\n", state);
        
        /*
        switch (state)
        {
        case 0:
            switch (recv_buffer[1])
            {
                case 0x00: Handshake(recv_buffer, &state); break; 
                default: break;
            }
            break;

        case 1: break;

        case 2: 
            switch (recv_buffer[1])
            {
                case 0x00: HandleLoginStart(&client, &player, recv_buffer); break;
                case 0x03: state = 3; break;
                default: break;
            }
            break;

        case 3: 
            switch (recv_buffer[1])
            {
                case 0x02: HandleConfingurationStart(&client, &player, recv_buffer); break;
                case 0x03: 
                    state = 4; 
                    SendLogin(&client);
                    
                    break;
                default: break;
            }
            break;

        default: break;
        }
        */
    }

    
    WSACleanup();

    return 0;
}