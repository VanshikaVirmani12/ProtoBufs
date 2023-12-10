#ifndef WEATHER_DATA_PB_H
#define WEATHER_DATA_PB_H

#include "../weather_data_def.h"

int weather_data_pb_serialize(WeatherData *weather_data, uint8_t **buffer,
                              size_t *size_ret);
int weather_data_pb_deserialize(WeatherData **weather_data, uint8_t *buffer);

int weather_data_pb_serialize2(WeatherData *weather_data, uint8_t **buffer,
                               size_t *size_ret);
int weather_data_pb_deserialize2(WeatherData **weather_data, uint8_t *buffer);

#endif // WEATHER_DATA_PB_H
