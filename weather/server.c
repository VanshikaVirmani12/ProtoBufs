#include "encrypt.h"
#include "protobuf_enc.h"
#include "weather_data_def.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

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

  int type = 0;

  type = atoi(argv[2]);
  if (type < 0 || type > 2) {
    printf("Invalid encode type\n");
    return 1;
  }

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

  if (type < 0 || type > 2) {
    printf("Invalid encode type\n");
    return 1;
  }

  EVP_PKEY *pkey = EVP_PKEY_new();
  int key_size = 4096;
  if (type == 2) {
    key_size = 8192;
  } else if (type == 0) {
    key_size = 2048;
  }
  generate_rsa_key(&pkey, key_size);
  long public_key_length = 0;
  char *public_key = NULL;
  public_key_length = get_public_key(pkey, &public_key);

  char *private_key = NULL;
  get_private_key(pkey, &private_key);
  clock_t t;

  while (1) {
    printf("Waiting for connection...\n");
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
      perror("Error accepting connection");
      continue;
    }
    printf("Connection accepted\n");
    t = clock();

    if (send(client_socket, public_key, public_key_length, 0) == -1) {
      perror("Error sending public key");
      close(server_socket);
      close(client_socket);
      return 1;
    };
    char *client_public_key = malloc(public_key_length);
    if (recv(client_socket, client_public_key, public_key_length, 0) == -1) {
      perror("Error receiving client public key");
      close(server_socket);
      close(client_socket);
      return 1;
    };

    char buffer[BUFSIZ];
    WeatherData *weather_data = NULL;
    size_t received = recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received %lu bytes\n", received);
    printf("Received: %s\n", buffer);
    char *decrypted_message = NULL;
    size_t decrypted_message_length = 0;

    if (type == 0) {
      decrypt_protobuf(buffer, received, public_key, private_key,
                       &decrypted_message, &decrypted_message_length);
    } else {
      decrypt_message(pkey, buffer, &decrypted_message,
                      &decrypted_message_length, received);
    }
    printf("Decrypted message: %s\n", decrypted_message);
    weather_data_deserialize(&weather_data, (uint8_t *)decrypted_message, type);
    print_weather_data(weather_data);

    weather_data_free(weather_data);
    free(decrypted_message);

    printf("Number of clock ticks: %ld (%.2f seconds)\n", t,
           ((float)t) / CLOCKS_PER_SEC);

    close(client_socket);
  }
  free(public_key);
  EVP_PKEY_free(pkey);
  close(server_socket);
}
