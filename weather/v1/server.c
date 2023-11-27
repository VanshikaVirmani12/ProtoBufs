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
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        perror("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_address = initialize_server_address();

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    listen(server_socket, 5);

    int client_socket = accept(server_socket, NULL, NULL);

    uint8_t buffer[sizeof(WeatherData)];
    WeatherData *weather_data = NULL;
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received %lu bytes\n", sizeof(buffer));
    weather_data_deserialize(weather_data, buffer);
    print_weather_data(weather_data);

    // send(client_socket, "Hello from server", sizeof("Hello from server"), 0);

    close(server_socket);
    close(client_socket);

}


