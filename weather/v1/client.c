#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "weather_data.pb.h"

struct sockaddr_in initialize_server_address() {
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;
    return server_address;
}

int main() {
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize winsock\n");
        return 1;
    }
    #endif

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_address = initialize_server_address();

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    WeatherData *weather_data = weather_data_init(); 
    weather_data->temperature = 72.0;

    uint8_t *buffer;
    size_t size;
    weather_data_serialize(weather_data, buffer, &size);
    
    print_weather_data(weather_data);
    send(client_socket, buffer, size, 0);
    printf("here\n");
    // recv(client_socket, buffer, size, 0);
    // printf("Server: %s\n", buffer);

    close(client_socket);

    return 0;
}


