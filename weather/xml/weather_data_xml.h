#ifndef WEATHER_DATA_XML
#define WEATHER_DATA_XML

#include "../weather_data_def.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

xmlNodePtr weather_data_to_xml(const WeatherData *weatherData);
WeatherData *xml_to_weather_data(xmlNodePtr weathernode);

#endif // WEATHER_DATA_XML_H
