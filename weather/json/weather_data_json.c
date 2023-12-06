#include "weather_data_json.h"
#include <stdlib.h>
#include <sys/_endian.h>
#include <sys/_types/_off_t.h>

WeatherDataJson *weather_data_json_init() {
  WeatherDataJson *weather_data = calloc(1, sizeof(WeatherDataJson));
  if (weather_data == NULL) {
    return NULL;
  }
  return weather_data;
}

void weather_data_json_free(WeatherDataJson *weather_data) {
  if (weather_data == NULL) {
    return;
  }
  // free(weather_data->day_of_week);
  // free(weather_data->month);
  free(weather_data);
}

// Serialize WeatherDataJson struct to JSON
json_t *weather_data_json_serialize(const WeatherDataJson *weatherDataJson) {
  json_t *obj = json_object();
  json_object_set_new(obj, "day_of_week",
                      json_string(weatherDataJson->day_of_week));
  json_object_set_new(obj, "month", json_string(weatherDataJson->month));
  json_object_set_new(obj, "time", json_integer(weatherDataJson->time));
  json_object_set_new(obj, "day", json_integer(weatherDataJson->day));
  json_object_set_new(obj, "year", json_integer(weatherDataJson->year));
  json_object_set_new(obj, "temperature",
                      json_real(weatherDataJson->temperature));
  json_object_set_new(obj, "humidity", json_real(weatherDataJson->humidity));
  json_object_set_new(obj, "pressure", json_real(weatherDataJson->pressure));
  json_object_set_new(obj, "wind_speed",
                      json_real(weatherDataJson->wind_speed));
  json_object_set_new(obj, "wind_direction",
                      json_real(weatherDataJson->wind_direction));
  json_object_set_new(obj, "rain_last_hour",
                      json_real(weatherDataJson->rain_last_hour));
  json_object_set_new(obj, "solar_radiation",
                      json_real(weatherDataJson->solar_radiation));
  json_object_set_new(obj, "humidex", json_real(weatherDataJson->humidex));
  json_object_set_new(obj, "dew_point", json_real(weatherDataJson->dew_point));
  json_object_set_new(obj, "wind_chill",
                      json_real(weatherDataJson->wind_chill));
  json_object_set_new(obj, "snow_depth",
                      json_integer(weatherDataJson->snow_depth));
  return obj;
}

// Deserialize JSON to WeatherDataJson struct

WeatherDataJson *weather_data_json_deserialize(json_t *json) {
  WeatherDataJson *weather_data = weather_data_json_init();
  if (weather_data == NULL) {
    return NULL;
  }

  json_t *day_of_week = json_object_get(json, "day_of_week");
  if (day_of_week == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->day_of_week = strdup(json_string_value(day_of_week));

  json_t *month = json_object_get(json, "month");
  if (month == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->month = strdup(json_string_value(month));

  json_t *time = json_object_get(json, "time");
  if (time == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->time = json_integer_value(time);

  json_t *day = json_object_get(json, "day");
  if (day == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->day = json_integer_value(day);

  json_t *year = json_object_get(json, "year");
  if (year == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->year = json_integer_value(year);

  json_t *temperature = json_object_get(json, "temperature");
  if (temperature == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->temperature = json_real_value(temperature);

  json_t *humidity = json_object_get(json, "humidity");
  if (humidity == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->humidity = json_real_value(humidity);

  json_t *pressure = json_object_get(json, "pressure");
  if (pressure == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->pressure = json_real_value(pressure);

  json_t *wind_speed = json_object_get(json, "wind_speed");
  if (wind_speed == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->wind_speed = json_real_value(wind_speed);

  json_t *wind_direction = json_object_get(json, "wind_direction");
  if (wind_direction == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->wind_direction = json_real_value(wind_direction);

  json_t *rain_last_hour = json_object_get(json, "rain_last_hour");
  if (rain_last_hour == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->rain_last_hour = json_real_value(rain_last_hour);

  json_t *solar_radiation = json_object_get(json, "solar_radiation");
  if (solar_radiation == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->solar_radiation = json_real_value(solar_radiation);

  json_t *humidex = json_object_get(json, "humidex");
  if (humidex == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->humidex = json_real_value(humidex);

  json_t *dew_point = json_object_get(json, "dew_point");
  if (dew_point == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->dew_point = json_real_value(dew_point);

  json_t *wind_chill = json_object_get(json, "wind_chill");
  if (wind_chill == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->wind_chill = json_real_value(wind_chill);

  json_t *snow_depth = json_object_get(json, "snow_depth");
  if (snow_depth == NULL) {
    free(weather_data);
    return NULL;
  }
  weather_data->snow_depth = json_integer_value(snow_depth);

  return weather_data;
}

// Print weather data
int print_weather_data(const WeatherDataJson *weather_data) {
  printf("Day of week: %s\n", weather_data->day_of_week);
  printf("Month: %s\n", weather_data->month);
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
