#include "weather_data_pb.h"

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

int weather_data_pb_serialize(WeatherData *weather_data, uint8_t **buffer,
                              size_t *size_ret) {
  size_t size =
      sizeof(uint32_t) + sizeof(uint32_t) + sizeof(weather_data->time) +
      sizeof(weather_data->day) + sizeof(weather_data->year) +
      sizeof(weather_data->temperature) + sizeof(weather_data->humidity) +
      sizeof(weather_data->pressure) + sizeof(weather_data->wind_speed) +
      sizeof(weather_data->wind_direction) +
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

  char *days[] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                  "Friday", "Saturday", "Sunday"};
  char *months[] = {"January",   "February", "March",    "April",
                    "May",       "June",     "July",     "August",
                    "September", "October",  "November", "December"};
  uint32_t temp_date = 0;
  for (int i = 0; i < 7; i++) {
    if (strcmp(weather_data->day_of_week, days[i]) == 0) {
      temp_date = i;
      break;
    }
  }

  tmpl = htonl(temp_date);
  memcpy(offset_ptr, &tmpl, sizeof(uint32_t));
  offset_ptr += sizeof(uint32_t);

  for (int i = 0; i < 7; i++) {
    if (strcmp(weather_data->month, months[i]) == 0) {
      temp_date = i;
      break;
    }
  }

  tmpl = htonl(temp_date);
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

int weather_data_pb_deserialize(WeatherData **weather_data, uint8_t *buffer) {
  if (buffer == NULL) {
    return -1;
  }

  WeatherData *wd = weather_data_init();
  if (wd == NULL) {
    return -1;
  }

  uint8_t *offset_ptr = buffer;

  uint32_t tmpl;

  char *days[] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                  "Friday", "Saturday", "Sunday"};
  char *months[] = {"January",   "February", "March",    "April",
                    "May",       "June",     "July",     "August",
                    "September", "October",  "November", "December"};

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  wd->day_of_week = malloc(strlen(days[ntohl(tmpl)]) + 1);
  strcpy(wd->day_of_week, days[ntohl(tmpl)]);
  offset_ptr += sizeof(uint32_t);

  memcpy(&tmpl, offset_ptr, sizeof(uint32_t));
  wd->month = malloc(strlen(months[ntohl(tmpl)]) + 1);
  strcpy(wd->month, months[ntohl(tmpl)]);
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
