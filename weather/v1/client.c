#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <sys/types.h>
#include <unistd.h>

#include "weather_data.pb.h"

// Client Code

struct sockaddr_in initialize_server_address(char *ip, char *port) {
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(port));
  if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0) {
    perror("Error converting ip address");
    exit(1);
  }
  return server_address;
}

int send_public_key(int server_socket, EVP_PKEY *public_key) {
    // Get the size of the public key
    int public_key_size = i2d_PUBKEY(public_key, NULL);

    // Send the public key size to the server
    send(server_socket, &public_key_size, sizeof(public_key_size), 0);

    // Allocate a buffer to hold the serialized public key
    uint8_t *public_key_buffer = malloc(public_key_size);
    uint8_t *p = public_key_buffer;

    // Serialize the public key into the buffer
    i2d_PUBKEY(public_key, &p);

    // Send the serialized public key to the server
    send(server_socket, public_key_buffer, public_key_size, 0);

    // Free allocated memory
    free(public_key_buffer);

    return 0;  // Return appropriate error code if needed
}

int receive_public_key(int server_socket) {
    // Receive the public key size from the server
    int public_key_size = 295;
  

    // // Receive the serialized public key from the server
    uint8_t *public_key_buffer = malloc(public_key_size);
    recv(server_socket, public_key_buffer, public_key_size, 0);

    

    // Deserialize the received buffer into an EVP_PKEY object
    EVP_PKEY *public_key = d2i_PUBKEY(NULL, (const unsigned char **)&public_key_buffer, public_key_size);
printf("f\n");
    fflush(stdout);
    free(public_key_buffer);

    // return public_key;
    return 0; 
}

int main(int argc, char **argv) {

// generate public and private keys 
const char *scriptPath = "../key_gen.sh";
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

#ifdef _WIN32

  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    printf("Failed to initialize winsock\n");
    return 1;
  }
#endif

  if (argc != 3) {
    printf("Usage: %s <server ip> <port>\n", argv[0]);
    return 1;
  }

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Error creating socket\n");
    return 1;
  }

  struct sockaddr_in server_address =
      initialize_server_address(argv[1], argv[2]);

  if (connect(server_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("Error connecting to server");
    return 1;
  }

  printf("here\n");

  // EVP_PKEY server_pub_key[sizeof(public_key)];
  // recv(server_socket, server_pub_key, sizeof(server_pub_key), 0);
  // printf("Received %lu bytes\n", sizeof(server_pub_key));
  // printf("%s", server_pub_key); 

  printf("%d\n", server_socket); 
  int result = receive_public_key(server_socket);
  printf("result: %d\n", result);
  // EVP_PKEY *received_public_key = receive_public_key(server_socket);
  // printf("Received public key: %p\n", received_public_key);


  // int encrypted_size;
  // uint8_t *encrypted_data = malloc(EVP_PKEY_size(public_key));
  // encrypted_size = EVP_PKEY_encrypt(encrypted_data, buffer, size, public_key);


  // WeatherData *weather_data = weather_data_init();
  // weather_data->day_of_week = 4;
  // weather_data->month = 5;
  // weather_data->time = 1838;
  // weather_data->day = 5;
  // weather_data->year = 2021;
  // weather_data->temperature = 72.0;
  // weather_data->humidity = 0.5;
  // weather_data->pressure = 0.6;
  // weather_data->wind_speed = 0.7;
  // weather_data->wind_direction = 0.8;
  // weather_data->rain_last_hour = 0.9;
  // weather_data->solar_radiation = 1.0;
  // weather_data->humidex = 1.1;
  // weather_data->dew_point = 1.2;
  // weather_data->wind_chill = 1.3;
  // weather_data->snow_depth = 19;

  // uint8_t *buffer = NULL;
  // size_t size;
  // if (weather_data_serialize(weather_data, &buffer, &size) == -1) {
  //   printf("Error serializing data");
  //   return 1;
  // };

  // print_weather_data(weather_data);
  // size_t ret;
  // if ((ret = send(server_socket, buffer, size, 0)) != size) {
  //   printf("ret: %zu\n", ret);
  //   perror("Error sending data");
  //   return 1;
  // }



  // // recv(client_socket, buffer, size, 0);
  // // printf("Server: %s\n", buffer);

  // weather_data_free(weather_data);
  // free(buffer);

  close(server_socket);

  return 0;
}
