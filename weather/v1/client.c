#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("Error connecting to server");
    return 1;
  }

  WeatherData *weather_data = weather_data_init();
  weather_data->day_of_week = 4;
  weather_data->month = 5;
  weather_data->time = 1838;
  weather_data->day = 5;
  weather_data->year = 2021;
  weather_data->temperature = 72.0;
  weather_data->humidity = 0.5;
  weather_data->pressure = 0.6;
  weather_data->wind_speed = 0.7;
  weather_data->wind_direction = 0.8;
  weather_data->rain_last_hour = 0.9;
  weather_data->solar_radiation = 1.0;
  weather_data->humidex = 1.1;
  weather_data->dew_point = 1.2;
  weather_data->wind_chill = 1.3;
  weather_data->snow_depth = 19;

  uint8_t *buffer = NULL;
  size_t size;
  if (weather_data_serialize(weather_data, &buffer, &size) == -1) {
    printf("Error serializing data");
    return 1;
  };

  print_weather_data(weather_data);
  size_t ret;
  if ((ret = send(client_socket, buffer, size, 0)) != size) {
    printf("ret: %zu\n", ret);
    perror("Error sending data");
    return 1;
  }

  // recv(client_socket, buffer, size, 0);
  // printf("Server: %s\n", buffer);

  weather_data_free(weather_data);
  free(buffer);

  close(client_socket);

  return 0;
}
