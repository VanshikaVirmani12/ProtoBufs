#include "weather_data.pb.h"

#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifdef _WIN32
#include <winsock.h>
#endif

WeatherData *weather_data_init() {
  WeatherData *weather_data = calloc(1, sizeof(WeatherData));
  if (weather_data == NULL) {
    return NULL;
  }
  return weather_data;
}

void weather_data_free(WeatherData *weather_data) { free(weather_data); }

int weather_data_serialize(WeatherData *weather_data, uint8_t **buffer,
                           size_t *size_ret) {
  size_t size =
      sizeof(weather_data->day_of_week) + sizeof(weather_data->month) +
      sizeof(weather_data->time) + sizeof(weather_data->day) +
      sizeof(weather_data->year) + sizeof(weather_data->temperature) +
      sizeof(weather_data->humidity) + sizeof(weather_data->pressure) +
      sizeof(weather_data->wind_speed) + sizeof(weather_data->wind_direction) +
      sizeof(weather_data->rain_last_hour) +
      sizeof(weather_data->solar_radiation) + sizeof(weather_data->humidex) +
      sizeof(weather_data->dew_point) + sizeof(weather_data->wind_chill) +
      sizeof(weather_data->snow_depth);

  uint8_t *buf = malloc(size);
  *size_ret = size;

  if (buf == NULL) {
    return -1;
  }

  uint32_t tmpl;
  uint8_t *offset_ptr = buf;

  tmpl = htonl(weather_data->day_of_week);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  tmpl = htonl(weather_data->month);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  tmpl = htonl(weather_data->time);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  tmpl = htonl(weather_data->day);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  tmpl = htonl(weather_data->year);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->temperature, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->humidity, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->pressure, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->wind_speed, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->wind_direction, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->rain_last_hour, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->solar_radiation, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->humidex, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->dew_point, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->wind_chill, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, &weather_data->snow_depth, sizeof(uint32_t));
  tmpl = htonl(tmpl);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));

  if (*buffer == NULL) {
    *buffer = malloc(size);
    if (*buffer == NULL) {
      free(buf);
      return -1;
    }
  }

  memcpy(*buffer, buf, size);
  free(buf);

  return 0;
}

int weather_data_deserialize(WeatherData **weather_data, uint8_t *buffer) {
  if (buffer == NULL) {
    return -1;
  }

  WeatherData *wd = weather_data_init();
  if (wd == NULL) {
    return -1;
  }

  uint8_t *offset_ptr = buffer;

  uint32_t tmpl;

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  wd->day_of_week = ntohl(tmpl);
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  wd->month = ntohl(tmpl);
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  wd->time = ntohl(tmpl);
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  wd->day = ntohl(tmpl);
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  wd->year = ntohl(tmpl);
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->temperature, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->humidity, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->pressure, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->wind_speed, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->wind_direction, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->rain_last_hour, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->solar_radiation, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->humidex, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->dew_point, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->wind_chill, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  tmpl = ntohl(tmpl);
  memcpy(&wd->snow_depth, &tmpl, sizeof(uint32_t));

  *weather_data = wd;

  return 0;
}

int print_weather_data(WeatherData *weather_data) {
  char *days[] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                  "Friday", "Saturday", "Sunday"};
  char *months[] = {"January",   "February", "March",    "April",
                    "May",       "June",     "July",     "August",
                    "September", "October",  "November", "December"};
  printf("Day of week: %s\n", days[weather_data->day_of_week]);
  printf("Month: %s\n", months[weather_data->month]);
  printf("Time: %d\n", weather_data->time);
  printf("Day: %d\n", weather_data->day);
  printf("Year: %d\n", weather_data->year);
  printf("Temperature: %f\n", weather_data->temperature);
  printf("Humidity: %f\n", weather_data->humidity);
  printf("Pressure: %f\n", weather_data->pressure);
  printf("Wind speed: %f\n", weather_data->wind_speed);
  printf("Wind direction: %f\n", weather_data->wind_direction);
  printf("Rain last hour: %f\n", weather_data->rain_last_hour);
  printf("Solar radiation: %f\n", weather_data->solar_radiation);
  printf("Humidex: %f\n", weather_data->humidex);
  printf("Dew point: %f\n", weather_data->dew_point);
  printf("Wind chill: %f\n", weather_data->wind_chill);
  printf("Snow depth: %d\n", weather_data->snow_depth);
  return 0;
}
