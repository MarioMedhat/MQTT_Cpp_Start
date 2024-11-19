#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>


class Sensor
{
    public:
        virtual void update_sensor_reading() = 0;
        virtual std::string get_sensor_reading() = 0;
        virtual std::string get_sensor_topic() = 0;
};

class Temp_Sensor : public Sensor
{
    public:
        Temp_Sensor (std::string topic);
        void update_sensor_reading() override;
        std::string get_sensor_reading() override;
        std::string get_sensor_topic() override;
    
    private:
        int reading;
        std::string topic;
};

class Hum_Sensor : public Sensor
{
    public:
        Hum_Sensor (std::string topic);
        void update_sensor_reading() override;
        std::string get_sensor_reading() override;
        std::string get_sensor_topic() override;
    
    private:
        int reading;
        std::string topic;
};

#endif