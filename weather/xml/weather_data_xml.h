#ifndef WEATHER_DATA_XML
#define WEATHER_DATA_XML

#include <libxml/parser.h>
#include <libxml/tree.h>


typedef struct WeatherData {
  char* day_of_week;
  char* month;
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
xmlNodePtr weather_data_to_xml(const WeatherData *weatherData);
WeatherDataJson* xml_to_weather_data(xmlNodePtr weathernode);

int print_weather_data(WeatherData *weatherData);

#endif  // WEATHER_DATA_XML_H
