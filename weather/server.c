#include "weather_data_def.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BACKLOG 5
#define PORT 8080

struct sockaddr_in initialize_server_address(char *port) {
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(port));
  server_address.sin_addr.s_addr = INADDR_ANY;
  memset(&server_address.sin_zero, 0, 8);
  return server_address;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s <port> <type of encoding>\n", argv[0]);
    return 1;
  };

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1) {
    perror("Error creating socket\n");
    return 1;
  }

  struct sockaddr_in server_address = initialize_server_address(argv[1]);

  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(struct sockaddr_in)) == -1) {
    perror("Error binding socket");
    return 1;
  }

  if (listen(server_socket, MAX_BACKLOG) == -1) {
    perror("Error listening on socket");
    close(server_socket);
    return 1;
  }

  int client_socket = accept(server_socket, NULL, NULL);
  if (client_socket == -1) {
    perror("Error accepting connection");
    close(server_socket);
    return 1;
  }

  int type = 0;

  type = atoi(argv[2]);
  if (type < 0 || type > 2) {
    printf("Invalid encode type\n");
    return 1;
  }

  uint8_t buffer[BUFSIZ];
  WeatherData *weather_data = NULL;
  recv(client_socket, buffer, sizeof(buffer), 0);
  printf("Received %lu bytes\n", sizeof(buffer));
  weather_data_deserialize(&weather_data, buffer, type);
  print_weather_data(weather_data);

  // send(client_socket, "Hello from server", sizeof("Hello from server"), 0);

  close(server_socket);
  close(client_socket);
}
