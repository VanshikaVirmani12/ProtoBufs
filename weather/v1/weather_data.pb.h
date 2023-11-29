#ifndef WEATHER_DATA_PB_H
#define WEATHER_DATA_PB_H

#include <stddef.h>
#include <stdint.h>

typedef enum DayOfWeek {
  MONDAY = 0,
  TUESDAY = 1,
  WEDNESDAY = 2,
  THURSDAY = 3,
  FRIDAY = 4,
  SATURDAY = 5,
  SUNDAY = 6
} DayOfWeek;

typedef enum Month {
  JANUARY = 0,
  FEBRUARY = 1,
  MARCH = 2,
  APRIL = 3,
  MAY = 4,
  JUNE = 5,
  JULY = 6,
  AUGUST = 7,
  SEPTEMBER = 8,
  OCTOBER = 9,
  NOVEMBER = 10,
  DECEMBER = 11
} Month;

typedef struct WeatherData {
  DayOfWeek day_of_week;
  // NOTE: Using enum should be better than using string, since it's more
  // compact.
  Month month;
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
                           size_t *size_ret);
int weather_data_deserialize(WeatherData **weather_data, uint8_t *buffer);
int print_weather_data(WeatherData *weather_data);

#endif // WEATHER_DATA_PB_H
