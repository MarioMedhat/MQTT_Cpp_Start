#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstring>
#include "mqtt/async_client.h"
#include "MQTT_Client.hpp"
#include "Json_DataRW.hpp"
#include "Sensor.hpp"

const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string CLIENT_ID_P("publisher");
const std::string CLIENT_ID_S("subscriber");
const std::string TOPIC("test/topic");
const int KEEP_ALLIVE_INTERVALL = 20;

const int QOS = 1;
const int TIMEOUT = 10000;

// void sensors_main()
// {
//     while(true)
//     {
//         temp_sensor.update_sensor_reading();
//         hum_sensor.update_sensor_reading();
//         // Wait for 1 seconds
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// }

// void pub_main()
// {
    
//     sensor.connect();

//     while(true)
//     {
//         std::string json_data_str1 = temp_sensor.get_sensor_reading();
//         sensor1.publish("temperature", json_data_str1);

//         std::string json_data_str2 = hum_sensor.get_sensor_reading();
//         sensor2.publish("humidity", json_data_str2);

//         // Wait for 3 seconds
//         std::this_thread::sleep_for(std::chrono::seconds(3));
//     }
// }

// void sub_main()
// {
//     LoggerService logger(SERVER_ADDRESS, "temp_logger", 20, 1);
//     logger.connect();
//     logger.subscribe("humidity");
//     logger.subscribe("temperature");
//     while (true)
//     {
//         // Wait for messages
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//     }
// }

int main(int argc, char* argv[])
{
    SensorService sensor (SERVER_ADDRESS, "Sensors", 20, 1);
    LoggerService logger(SERVER_ADDRESS, "logger", 20, 1);
    //std::thread th1(sensors_main);
    //std::thread th2(pub_main);
    //std::thread th3(sub_main);
    std::thread th1([&sensor]() {sensor.sensor_update();});
    std::thread th2([&sensor]() {sensor.sensor_publish();});
    std::thread th3([&logger]() {logger.read_sensors();});

    th1.join();
    th2.join();
    th3.join();

    return 0;
}