#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include "mqtt/async_client.h"

class MQTTCallback : public virtual mqtt::callback
{
public:
    void connection_lost(const std::string& cause) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override;

    void message_arrived(mqtt::const_message_ptr message) override;
};

class MQTTClient_m
{
    public:
        MQTTClient_m(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service);
        bool connect();
        void disconnect();
        void subscribe(std::string topic);
        void publish(std::string topic, std::string message);

    private:
        std::string server_address;
        std::string client_id;
        int keep_alive_interval;
        int quality_of_service;
        mqtt::async_client client;
        bool connection_successful = false;
        bool first_time = true;
        MQTTCallback callback;
};

#endif