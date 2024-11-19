#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include <vector>
#include "mqtt/async_client.h"
#include "Sensor.hpp"

class MQTTCallback : public virtual mqtt::callback
{
public:
    void connection_lost(const std::string& cause) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;
    void message_arrived(mqtt::const_message_ptr message) override;
};

class MQTTService
{
    public:
        MQTTService(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service);
        bool connect();
        void disconnect();
        virtual void subscribe(std::string topic) = 0;
        virtual void publish(std::string topic, std::string message) = 0;

        std::string server_address;
        std::string client_id;
        int keep_alive_interval;
        int quality_of_service;
        mqtt::async_client client;
        bool connection_successful = false;
        bool first_time = true;
        MQTTCallback callback;
};

class LoggerService : public MQTTService
{
    public:
        LoggerService(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service);
        void subscribe(std::string topic) override;
        void publish(std::string topic, std::string message) override;
        void read_sensors();
};

class SensorService : public MQTTService
{
    public:
        SensorService(std::string server_address, std::string client_id, int keep_alive_interval, int quality_of_service);
        void publish(std::string topic, std::string message) override;
        void subscribe(std::string topic) override;
        void sensor_update();
        void sensor_publish();
    private:
        std::vector<std::unique_ptr<Sensor>> Sensors;
};

#endif