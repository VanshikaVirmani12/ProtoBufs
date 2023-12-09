#include "weather_data_def.h"
#include <libxml/tree.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "protobuf/weather_data_pb.h"
#include "xml/weather_data_xml.h"
#include "json/weather_data_json.h"
#include <string.h>

WeatherData *weather_data_init() {
  WeatherData *weather_data = calloc(1, sizeof(WeatherData));
  if (weather_data == NULL) {
    return NULL;
  }
  return weather_data;
}

void weather_data_free(WeatherData *weather_data) { free(weather_data); }

void print_weather_data(WeatherData *weather_data) {
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
}

int weather_data_serialize(WeatherData *weather_data, uint8_t **buffer,
                           size_t *size_ret, int type) {
  switch (type) {
    // PB
  case 0: {
    return weather_data_pb_serialize(weather_data, buffer, size_ret);
    break;
  }

    // JSON
  case 1: {
    json_t *json = weather_data_json_serialize(weather_data);
    char *json_str = json_dumps(json, JSON_COMPACT);
    *buffer = malloc(strlen(json_str) + 1);
    memcpy(*buffer, json_str, strlen(json_str) + 1);
    *size_ret = strlen(json_str);
    free(json_str);
    free(json);
    return 0;
    break;
  }

    // XML
  case 2: {
    xmlNodePtr xml_n_ptr = weather_data_to_xml(weather_data);
    // convert xml to string
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlDocSetRootElement(doc, xml_n_ptr);
    xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    *buffer = malloc(buffersize);
    memcpy(*buffer, xmlbuff, buffersize);
    *size_ret = buffersize;
    xmlFree(xmlbuff);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
    return 0;
    break;
  }

  default: {
    printf("Invalid type\n");
    return -1;
  }
  }
  return -1;
}

int weather_data_deserialize(WeatherData **weather_data, uint8_t *buffer,
                             int type) {
  switch (type) {
    // PB
  case 0: {
    return weather_data_pb_deserialize(weather_data, buffer);
    break;
  }
    // JSON
  case 1: {
    WeatherData *wd = NULL;
    json_error_t error;
    json_t *json_buffer =
        json_loadb((char *)buffer, strlen((char *)buffer), 0, &error);
    if (json_buffer == NULL) {
      printf("Error parsing json: %s\n", error.text);
      return -1;
    }
    wd = weather_data_json_deserialize(json_buffer);
    if (wd == NULL) {
      printf("Error deserializing json\n");
      return -1;
    }
    *weather_data = wd;
    break;
  }
    // XML
  case 2: {
    WeatherData *wd = NULL;
    xmlDocPtr doc = xmlReadMemory((char *)buffer, strlen((char *)buffer),
                                  "noname.xml", NULL, 0);
    if (doc == NULL) {
      printf("Error parsing xml\n");
      return -1;
    }
    xmlNodePtr xml_n_ptr = xmlDocGetRootElement(doc);
    wd = xml_to_weather_data(xml_n_ptr);
    if (wd == NULL) {
      printf("Error deserializing xml\n");
      return -1;
    }
    *weather_data = wd;
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
    return 0;
    break;
  }
  default: {
    printf("Invalid type\n");
    return -1;
  }
  }
  return -1;
}
