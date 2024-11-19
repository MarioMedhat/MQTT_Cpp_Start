#include <iostream>
#include <thread>
#include <cstring>
#include "mqtt/async_client.h"
#include "MQTT_Client.hpp"

void MQTTCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void MQTTCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered to the Subscriber" << std::endl;
}

void MQTTCallback::message_arrived(mqtt::const_message_ptr message)
{
    std::cout << "Message arrived: " << std::endl << message->get_payload_str() << std::endl;

}

MQTTService::MQTTService(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service)
:   server_address(std::move(server_address)), 
    client_id(std::move(client_id)), 
    keep_alive_interval(keep_alive_interval), 
    quality_of_service(quality_of_service),
    client(this->server_address, this->client_id){}

bool MQTTService::connect()
{
    try
    {
        mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(keep_alive_interval);
        connOpts.set_clean_session(true);

        client.set_callback(callback);
        mqtt::token_ptr connectionToken = client.connect(connOpts);
        connectionToken->wait();

        connection_successful = true;
    }

    catch (const mqtt::exception& ex)
    {
        std::cerr << "MQTT Exception: " << ex.what() << std::endl;
        bool connection_successful = false;
    }

    return connection_successful;
}

void MQTTService::disconnect()
{
    if (connection_successful == true)
    {
        mqtt::token_ptr disconnectionToken = client.disconnect();
        disconnectionToken->wait();
    }
    else
    {
        std::cout << "Your client is not connected" << std::endl;
    }
}

SensorService::SensorService(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service)
:   MQTTService(std::move(server_address), std::move(client_id), keep_alive_interval, quality_of_service)
{
    Temp_Sensor temp_sensor ("temperature");
    Hum_Sensor hum_sensor ("humidity");

    Sensors.push_back(std::make_unique<Temp_Sensor>("temperature"));
    Sensors.push_back(std::make_unique<Hum_Sensor>("humidity"));

    connect();
}

void SensorService::publish(std::string topic, std::string message)
{
    if (connection_successful == true)
    {
        mqtt::message_ptr pubMessage = mqtt::make_message(topic, message, quality_of_service, false);
        client.publish(pubMessage)->wait();
    }
    else
    {
        std::cout << "Your client is not connected" << std::endl;
    }
}

void SensorService::subscribe(std::string topic) {
    std::cout << "SensorService does not support subscribing to topics." << std::endl;
}

void SensorService::sensor_update()
{
    while(true)
    {
        for (auto& sensor : Sensors)
        {
            sensor->update_sensor_reading();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void SensorService::sensor_publish()
{
    while(true)
    {
        for (auto& sensor : Sensors)
        {
            std::string json_data_str = sensor->get_sensor_reading();
            publish(sensor->get_sensor_topic(), sensor->get_sensor_reading());
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}


LoggerService::LoggerService(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service)
:   MQTTService(std::move(server_address), std::move(client_id), keep_alive_interval, quality_of_service)
{
    connect();
}

void LoggerService::subscribe(std::string topic)
{
    if (connection_successful == true)
    {
        mqtt::token_ptr subToken = client.subscribe(topic, quality_of_service);
        subToken->wait();
        first_time = false;
    }
    else
    {
        std::cout << "Your client is not connected" << std::endl;
    }
}

void LoggerService::publish(std::string topic, std::string message)
{
    std::cout << "LoggerService does not support publishing messages." << std::endl;
}

void LoggerService::read_sensors()
{
    subscribe("humidity");
    subscribe("temperature");
    while (true)
    {
        // Wait for messages
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

