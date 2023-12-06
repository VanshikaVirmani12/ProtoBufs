#ifndef WEATHER_DATA_JSON_H
#define WEATHER_DATA_JSON_H

#include <stdint.h>
#include <string.h>

// C library for manipulating JSON data
#include <jansson.h>

typedef struct WeatherDataJson {
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
} WeatherDataJson;

WeatherDataJson *weather_data_json_init();
void weather_data_json_free(WeatherDataJson *wd);

json_t *weather_data_json_serialize(const WeatherDataJson *weatherDataJson);
WeatherDataJson *weather_data_json_deserialize(json_t *json);

int print_weather_data(const WeatherDataJson *weatherDataJson);

#endif // WEATHER_DATA_JSON_H
