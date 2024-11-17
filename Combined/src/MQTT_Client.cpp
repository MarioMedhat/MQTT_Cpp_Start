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

MQTTClient_m::MQTTClient_m(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service)
: server_address(std::move(server_address)), 
    client_id(std::move(client_id)), 
    keep_alive_interval(keep_alive_interval), 
    quality_of_service(quality_of_service),
    client(this->server_address, this->client_id){}

bool MQTTClient_m::connect()
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

void MQTTClient_m::disconnect()
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

void MQTTClient_m::subscribe(std::string topic)
{
    if (connection_successful == true)
    {
        if (first_time == true)
        {
            mqtt::token_ptr subToken = client.subscribe(topic, quality_of_service);
            subToken->wait();
            first_time = false;
        }

        while (true)
        {
            // Wait for messages
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    else
    {
        std::cout << "Your client is not connected" << std::endl;
    }
}

void MQTTClient_m::publish(std::string topic, std::string message)
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