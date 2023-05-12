#include "include/constants.h"
#include "include/message_struct.h"
#include "include/message.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>


int main(int argc, char* argv[]) 
{
    // ssize_t read_size;
    int port = SERVER_PORT, client_socket, player_id = 0;
    char server[50] = "127.0.0.1";
    char buffer[100] = "Unnamed";
    struct sockaddr_in remote_address;
    struct Message msg;
    char type[5] = "JOIN";

    if (argc == 4) {
        strcpy(server, argv[1]); // Server addr
        port = atoi(argv[2]);    // Port
        strcpy(buffer, argv[3]); // Username
    }

    printf("Connecting to %s:%d as %s.\n", server, port, buffer);

    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
    inet_pton(AF_INET, server, &remote_address.sin_addr);
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0) {
        printf("Socket error.\n");
        return -1;
    }
    
    if (connect(client_socket, (struct sockaddr*)&remote_address, sizeof(remote_address)) < 0) {
        printf("Connection error.\n");
        return -1;
    }

    message_encode(&msg, type, player_id, buffer);
    send(client_socket, &msg, sizeof(msg), 0);

    while(1) {
        read(client_socket, &msg, sizeof(msg));
        message_decode(&msg, type, &player_id, buffer);
        message_print(&msg);

        scanf("%s", buffer);
        message_encode(&msg, type, player_id, buffer);
        send(client_socket, &msg, sizeof(msg), 0);
    }


    close(client_socket);
    return 0;
}