#ifndef WEATHER_DATA_STRUCT_H
#define WEATHER_DATA_STRUCT_H

#include <stddef.h>
#include <stdint.h>

typedef struct WeatherData {
  char *day_of_week;
  char *month;
  int32_t time;
  int32_t day;
  int32_t year;
  float temperature;
  float humidity;
  float pressure;
  float wind_speed;
  float wind_direction;
  float rain_last_hour;
  float solar_radiation;
  float humidex;
  float dew_point;
  float wind_chill;
  int32_t snow_depth;
} WeatherData;

WeatherData *weather_data_init();
void weather_data_free(WeatherData *weather_data);

int weather_data_serialize(WeatherData *weather_data, uint8_t **buffer,
                           size_t *size_ret, int type);
int weather_data_deserialize(WeatherData **weather_data, uint8_t *buffer,
                             int type);

void print_weather_data(WeatherData *weather_data);

#endif // WEATHER_DATA_STRUCT_H
