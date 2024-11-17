#include <iostream>
#include <thread>
#include <cstring>
#include "mqtt/async_client.h"
#include "MQTT_Client.hpp"
#include "Json_DataRW.hpp"

const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string CLIENT_ID_P("publisher");
const std::string CLIENT_ID_S("subscriber");
const std::string TOPIC("test/topic");
const int KEEP_ALLIVE_INTERVALL = 20;

const int QOS = 1;
const int TIMEOUT = 10000;

void pub_main()
{
    MQTTClient_m client(SERVER_ADDRESS, CLIENT_ID_P, 20, 1);
    client.connect();
    while(true)
    {
        std::string message;
        std::cout << "Enter s to send json data: ";
        std::getline(std::cin, message);

        if (message == "s")
        {
            std::string json_data_str = JsonDataToString("shared_data.json");
            client.publish(TOPIC, json_data_str);
        }
    }
}

void sub_main()
{
    MQTTClient_m client(SERVER_ADDRESS, CLIENT_ID_S, 20, 1);
    client.connect();
    client.subscribe(TOPIC);
}

int main(int argc, char* argv[])
{
    std::thread th1(pub_main);
    std::thread th2(sub_main);

    th1.join();
    th2.join();

    return 0;
}