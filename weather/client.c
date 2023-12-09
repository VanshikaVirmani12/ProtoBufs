#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/_types/_size_t.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "encrypt.h"
#include "weather_data_def.h"

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

int main(int argc, char **argv) {
#ifdef _WIN32
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    printf("Failed to initialize winsock\n");
    return 1;
  }
#endif

  if (argc != 4) {
    printf("Usage: %s <server ip> <port> <type of encoding>\n", argv[0]);
    return 1;
  }

  int type = 0;
  type = atoi(argv[3]);

  if (type < 0 || type > 2) {
    printf("Invalid encode type\n");
    return 1;
  }

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Error creating socket\n");
    return 1;
  }

  struct sockaddr_in server_address =
      initialize_server_address(argv[1], argv[2]);

  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("Error connecting to server");
    return 1;
  }

  EVP_PKEY *pkey = EVP_PKEY_new();
  // generate_keys(pkey);
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

  char server_public_key[public_key_length];

  if (recv(client_socket, server_public_key, public_key_length, 0) == -1) {
    perror("Error receiving public key from server");
    return 1;
  }
  if (send(client_socket, public_key, public_key_length, 0) == -1) {
    perror("Error sending public key to server");
    return 1;
  }

  size_t ret;
  EVP_PKEY *server_pkey = get_public_key_from_string(server_public_key);

  char *encrypted_message = NULL;
  size_t encrypted_message_length;

  WeatherData *weather_data = weather_data_init();
  strcpy(weather_data->day_of_week, "Monday");
  strcpy(weather_data->month, "June");
  weather_data->time = 1838;
  weather_data->day = 5;
  weather_data->year = 2021;
  weather_data->temperature = 73.0;
  weather_data->humidity = 0.5;
  weather_data->pressure = 0.6;
  weather_data->wind_speed = 0.7;
  weather_data->wind_direction = 0.8;
  weather_data->rain_last_hour = 0.9;
  weather_data->solar_radiation = -1.0;
  weather_data->humidex = 1.1;
  weather_data->dew_point = 1.2;
  weather_data->wind_chill = 1.3;
  weather_data->snow_depth = 19;

  uint8_t *buffer = NULL;
  size_t size;
  if (weather_data_serialize(weather_data, &buffer, &size, type) == -1) {
    printf("Error serializing data");
    return 1;
  };

  print_weather_data(weather_data);
  printf("Sending %lu bytes\n", size);
  printf("Buffer: %s\n", buffer);

  encrypt_message(server_pkey, (char *)buffer, &encrypted_message,
                  &encrypted_message_length);
  printf("Encrypted message: %s\n", encrypted_message);
  printf("Encrypted message length: %zu\n", encrypted_message_length);

  if ((ret = send(client_socket, encrypted_message, encrypted_message_length,
                  0)) != encrypted_message_length) {
    printf("ret: %zu\n", ret);
    perror("Error sending data");
    return 1;
  }

  weather_data_free(weather_data);
  free(buffer);

  free(encrypted_message);
  free(public_key);
  EVP_PKEY_free(pkey);

  close(client_socket);

  return 0;
}
