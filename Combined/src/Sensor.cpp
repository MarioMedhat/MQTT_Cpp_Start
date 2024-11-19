#include <iostream>
#include <cstring>
#include "Sensor.hpp"
#include "Json_DataRW.hpp"
#include <thread>
#include <chrono>

Temp_Sensor::Temp_Sensor (std::string topic) :
    topic (std::move(topic)) {};

void Temp_Sensor::update_sensor_reading()
{
        static int count = 0;
        count ++;
        reading = count;
        updateJsonFile("temperature.json", "temperature", reading);
}

std::string Temp_Sensor::get_sensor_reading()
{
    std::string json_data_str = JsonDataToString("temperature.json");
    return json_data_str;
}

std::string Temp_Sensor::get_sensor_topic()
{
    return topic;
}

Hum_Sensor::Hum_Sensor (std::string topic) :
    topic (std::move(topic)) {};

void Hum_Sensor::update_sensor_reading()
{
        static int count = 0;
        count ++;
        reading = (count + 100);
        updateJsonFile("humidity.json", "humidity", reading);
}

std::string Hum_Sensor::get_sensor_reading()
{
    std::string json_data_str = JsonDataToString("humidity.json");
    return json_data_str;
}

std::string Hum_Sensor::get_sensor_topic()
{
    return topic;
}