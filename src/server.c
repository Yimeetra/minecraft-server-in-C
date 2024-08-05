#include <stdio.h>
#include <winsock2.h>

#define RECV_BUFFER_SIZE 256
#define SEND_BUFFER_SIZE 256
#define SEGMENT_BITS 0b01111111
#define CONTINUE_BIT 0b10000000

int read_varint(unsigned char* varint)
{
    int value;
    int i = 0;
    while (1) {        
        value |= (varint[i] & SEGMENT_BITS) << i*7;
        if (!(varint[i] & CONTINUE_BIT)) {
            break;
        }
        i++;
    }
    return value;
}


void write_varint(unsigned char* varint, int value)
{
    int i = 0;
    while (1) {
        if (!(value & ~SEGMENT_BITS)) {
            varint[i] = value;
            break;
        };    
        varint[i] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (unsigned int) value >> 7;
        i++;
    }
    return;
}


void handshake();
void login_start();
void login_success();
void login_acknowledged();


int main()
{
    WSADATA wsadata;
    SOCKET server;
    SOCKET client;
    SOCKADDR_IN server_addr, client_addr;
    unsigned char recv_buffer[RECV_BUFFER_SIZE] = {0};
    unsigned char send_buffer[SEND_BUFFER_SIZE] = {0};

    int state = 0;
    char nickname[16];
    int nickname_len;

    WSAStartup(MAKEWORD(2,2), &wsadata);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    server_addr.sin_port = htons(25565);

    server = socket(AF_INET, SOCK_STREAM, 0);
    bind(server,(SOCKADDR*) &server_addr, sizeof(server_addr));
    listen(server, 0);

    client = accept(server, NULL, NULL);
    
    while (1) {
        recv(client, recv_buffer, RECV_BUFFER_SIZE, 0);
        for (int i = 0 ; i < RECV_BUFFER_SIZE; ++i ) {
            printf("%.2x ", recv_buffer[i]);
        }
        printf("\n");

        switch (state)
        {
        case 0:
            switch (recv_buffer[1])
            {
                case 0x00: state = 2; break; 
                default: break;
            }
            break;

        case 1: break;
        
        case 2: 
            switch (recv_buffer[1])
            {
            case 0x00:  
                for (int i = 0; i <recv_buffer[2]; ++i) {
                    nickname[i] = recv_buffer[i+3];
                }
                nickname_len = (int) recv_buffer[2];
                printf("%s\n",nickname);
                send_buffer[0] = 0x1f;
                send_buffer[1] = 0x02;
                for (int i = 0; i < nickname_len+1; ++i) {
                    send_buffer[i+16-nickname_len] = nickname[i];
                }
                send_buffer[17] = nickname_len;
                for (int i = 0; i < nickname_len+1; ++i) {
                    send_buffer[i+18] = nickname[i];
                }
                for (int i = 0 ; i < SEND_BUFFER_SIZE; ++i ) {
                    printf("%.2x ", send_buffer[i]);
                }
                printf("\n");
                send(client,send_buffer,SEND_BUFFER_SIZE,0);
                break;
            
            default:
                break;
            }
            break;

        case 3: break;
        
        default: break;
        }
    }

    
    WSACleanup();

    return 0;
}