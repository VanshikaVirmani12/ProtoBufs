#include "weather_data_xml.h"
#include <stdio.h>
#include <stdlib.h>

// Serialize WeatherData to XML
xmlNodePtr weather_data_to_xml(const WeatherData *weather_data) {
  xmlNodePtr weatherNode = xmlNewNode(NULL, BAD_CAST "WeatherData");

  xmlNewChild(weatherNode, NULL, BAD_CAST "DayOfWeek",
              BAD_CAST weather_data->day_of_week);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Month",
              BAD_CAST weather_data->month);

  char timeString[10];
  snprintf(timeString, sizeof(timeString), "%d", weather_data->time);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Time",
              BAD_CAST xmlStrdup(timeString));

  char dayString[10];
  snprintf(dayString, sizeof(dayString), "%d", weather_data->day);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Day", BAD_CAST xmlStrdup(dayString));

  char yearString[10];
  snprintf(yearString, sizeof(yearString), "%d", weather_data->year);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Year",
              BAD_CAST xmlStrdup(yearString));

  char tempString[10];
  snprintf(tempString, sizeof(tempString), "%f", weather_data->temperature);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Temperature", BAD_CAST(tempString));

  char humidityString[10];
  snprintf(humidityString, sizeof(humidityString), "%f",
           weather_data->humidity);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Humidity", BAD_CAST(humidityString));

  char pressureString[10];
  snprintf(pressureString, sizeof(pressureString), "%f",
           weather_data->pressure);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Pressure", BAD_CAST(pressureString));

  char windSpeedString[10];
  snprintf(windSpeedString, sizeof(windSpeedString), "%f",
           weather_data->wind_speed);
  xmlNewChild(weatherNode, NULL, BAD_CAST "WindSpeed",
              BAD_CAST xmlStrdup(windSpeedString));

  char windDirectionString[10];
  snprintf(windDirectionString, sizeof(windDirectionString), "%f",
           weather_data->wind_direction);
  xmlNewChild(weatherNode, NULL, BAD_CAST "WindDirection",
              BAD_CAST xmlStrdup(windDirectionString));

  char rainLastHourString[10];
  snprintf(rainLastHourString, sizeof(rainLastHourString), "%f",
           weather_data->rain_last_hour);
  xmlNewChild(weatherNode, NULL, BAD_CAST "RainLastHour",
              BAD_CAST xmlStrdup(rainLastHourString));

  char solarRadiationString[10];
  snprintf(solarRadiationString, sizeof(solarRadiationString), "%f",
           weather_data->solar_radiation);
  xmlNewChild(weatherNode, NULL, BAD_CAST "SolarRadiation",
              BAD_CAST xmlStrdup(solarRadiationString));

  char humidexString[10];
  snprintf(humidexString, sizeof(humidexString), "%f", weather_data->humidex);
  xmlNewChild(weatherNode, NULL, BAD_CAST "Humidex",
              BAD_CAST xmlStrdup(humidexString));

  char dewPointString[10];
  snprintf(dewPointString, sizeof(dewPointString), "%f",
           weather_data->dew_point);
  xmlNewChild(weatherNode, NULL, BAD_CAST "DewPoint",
              BAD_CAST xmlStrdup(dewPointString));

  char windChillString[10];
  snprintf(windChillString, sizeof(windChillString), "%f",
           weather_data->wind_chill);
  xmlNewChild(weatherNode, NULL, BAD_CAST "WindChill",
              BAD_CAST xmlStrdup(windChillString));

  char snowDepthString[10];
  snprintf(snowDepthString, sizeof(snowDepthString), "%d",
           weather_data->snow_depth);
  xmlNewChild(weatherNode, NULL, BAD_CAST "SnowDepth",
              BAD_CAST xmlStrdup(snowDepthString));

  return weatherNode;
}

// Deserialize WeatherData from XML
WeatherData *xml_to_weather_data(xmlNodePtr weatherNode) {
  WeatherData *weather_data = weather_data_init();
  if (!weather_data) {
    return NULL;
  }

  xmlNodePtr child = weatherNode->children;

  while (child != NULL) {
    if (child->type == XML_ELEMENT_NODE) {
      if (xmlStrEqual(child->name, BAD_CAST "DayOfWeek")) {
        weather_data->day_of_week = xmlNodeGetContent(child);
      } else if (xmlStrEqual(child->name, BAD_CAST "Month")) {
        weather_data->month = xmlNodeGetContent(child);
      } else if (xmlStrEqual(child->name, BAD_CAST "Time")) {
        weather_data->time = atoi(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "Day")) {
        weather_data->day = atoi(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "Year")) {
        weather_data->year = atoi(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "Temperature")) {
        weather_data->temperature = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "Humidity")) {
        weather_data->humidity = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "Pressure")) {
        weather_data->pressure = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "WindSpeed")) {
        weather_data->wind_speed = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "WindDirection")) {
        weather_data->wind_direction = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "RainLastHour")) {
        weather_data->rain_last_hour = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "SolarRadiation")) {
        weather_data->solar_radiation = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "Humidex")) {
        weather_data->humidex = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "DewPoint")) {
        weather_data->dew_point = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "WindChill")) {
        weather_data->wind_chill = atof(xmlNodeGetContent(child));
      } else if (xmlStrEqual(child->name, BAD_CAST "SnowDepth")) {
        weather_data->snow_depth = atoi(xmlNodeGetContent(child));
      }
    }
    child = child->next;
  }

  return weather_data;
}
