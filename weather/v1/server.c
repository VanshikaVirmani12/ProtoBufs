#include "weather_data.pb.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#define MAX_BACKLOG 5
#define PORT 8080

// Server Code

struct sockaddr_in initialize_server_address(char *port) {
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(port));
  server_address.sin_addr.s_addr = INADDR_ANY;
  memset(&server_address.sin_zero, 0, 8);
  return server_address;
}

EVP_PKEY *receive_public_key(int client_socket) {
    // Receive the public key size from the client
    int public_key_size;
    recv(client_socket, &public_key_size, sizeof(public_key_size), 0);

    // Receive the serialized public key from the client
    uint8_t *public_key_buffer = malloc(public_key_size);
    recv(client_socket, public_key_buffer, public_key_size, 0);

    // Deserialize the received buffer into an EVP_PKEY object
    EVP_PKEY *public_key = d2i_PUBKEY(NULL, (const unsigned char **)&public_key_buffer, public_key_size);

    // Free allocated memory
    free(public_key_buffer);

    return public_key;
}
int send_public_key(int client_socket, EVP_PKEY *public_key) {
    // Get the size of the public key
    int public_key_size = i2d_PUBKEY(public_key, NULL);
    // printf("a"); 
    // printf("public key size: %d\n", public_key_size);

    size_t ret;
    // // Send the public key size to the client
    // if ((ret = send(client_socket, &public_key_size, sizeof(public_key_size), 0)) != sizeof(public_key_size)) {
    //   printf("ret: %zu\n", ret);
    //   perror("Error sending data");
    //   return 1;
    // }

    // Allocate a buffer to hold the serialized public key
    uint8_t *public_key_buffer = malloc(public_key_size);
    uint8_t *p = public_key_buffer;

    // Serialize the public key into the buffer
    i2d_PUBKEY(public_key, &p);

    printf("%d\n", public_key_size); 

    // Send the serialized public key to the client
    if ((ret = send(client_socket, public_key_buffer, public_key_size, 0)) != public_key_size) {
      printf("ret: %zu\n", ret);
      perror("Error sending data");
      return 1;
    }

    // Free allocated memory
    free(public_key_buffer);

    return 0;  // Return appropriate error code if needed
}


int main(int argc, char **argv) {

  // generate public and private keys 
  const char *scriptPath = "../key_gen.sh >/dev/null 2>&1";
  int returnValue = system(scriptPath);

  if (returnValue != 0) {
    printf("Error generating keys\n");
    return 1;
  } 

  // read the client's private and public keys
  FILE *private_key_file = fopen("private_key.pem", "r");
  EVP_PKEY *private_key = PEM_read_PrivateKey(private_key_file, NULL, NULL, NULL);
  printf("private key: %p\n", private_key);
  fclose(private_key_file);

  FILE *public_key_file = fopen("public_key.pem", "r");
  EVP_PKEY *public_key = PEM_read_PUBKEY(public_key_file, NULL, NULL, NULL);
  printf("public key: %p\n", public_key);
  fclose(public_key_file);

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
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

  // // Send public key to client
  // size_t ret;
  // size_t size = sizeof(public_key);
  // if ((ret = send(client_socket, public_key, sizeof(public_key), 0)) != size) {
  //   printf("ret: %zu\n", ret);
  //   perror("Error sending data");
  //   return 1;
  // }

  send_public_key(client_socket, public_key);

  // uint8_t buffer[sizeof(WeatherData)];
  // WeatherData *weather_data = NULL;
  // recv(client_socket, buffer, sizeof(buffer), 0);
  // printf("Received %lu bytes\n", sizeof(buffer));
  // weather_data_deserialize(&weather_data, buffer);
  // print_weather_data(weather_data);

  // // send(client_socket, "Hello from server", sizeof("Hello from server"), 0);

  close(server_socket);
  close(client_socket);
}
