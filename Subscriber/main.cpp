#include <iostream>
#include "mqtt/async_client.h"

const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string CLIENT_ID("subscriber");
const std::string TOPIC("test/topic");
const int QOS = 1;
const int TIMEOUT = 10000;

class SubscriberCallback : public virtual mqtt::callback
{
public:
    void connection_lost(const std::string& cause) override
    {
        std::cout << "Connection lost: " << cause << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr message) override
    {
        std::cout << "Message arrived: " << message->get_payload_str() << std::endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override
    {
        std::cout << "Message delivered" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try
    {
        SubscriberCallback callback;
        client.set_callback(callback);

        mqtt::token_ptr connectionToken = client.connect(connOpts);
        connectionToken->wait();

        mqtt::token_ptr subToken = client.subscribe(TOPIC, QOS);
        subToken->wait();

        while (true)
        {
            // Wait for messages
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        mqtt::token_ptr disconnectionToken = client.disconnect();
        disconnectionToken->wait();
    }
    catch (const mqtt::exception& ex)
    {
        std::cerr << "MQTT Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}