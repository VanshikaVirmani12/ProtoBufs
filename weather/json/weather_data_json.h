#ifndef WEATHER_DATA_JSON_H
#define WEATHER_DATA_JSON_H

#include "../weather_data_def.h"

// C library for manipulating JSON data
#include <jansson.h>

json_t *weather_data_json_serialize(const WeatherData *weatherDataJson);
WeatherData *weather_data_json_deserialize(json_t *json);

#endif // WEATHER_DATA_JSON_H
