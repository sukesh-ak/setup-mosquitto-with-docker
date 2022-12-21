// sudo apt update && sudo apt install libmosquitto-dev
// g++ -o mqtt_client main.cpp -lmosquitto

#include <mosquitto.h>
#include <cstring>
#include <iostream>

void on_connect(struct mosquitto *mosq, void *obj, int result)
{
    std::cout << "Connected to the MQTT broker" << std::endl;
}

void on_disconnect(struct mosquitto *mosq, void *obj, int result)
{
    std::cout << "Disconnected from the MQTT broker" << std::endl;
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    std::cout << "Received message: " << (char*)msg->payload << std::endl;
}

int main()
{
    // Initialize the Mosquitto library
    mosquitto_lib_init();

    // Create a new Mosquitto client instance
    struct mosquitto *mosq = mosquitto_new("client1", true, NULL);
    if (!mosq) {
        std::cerr << "Error: Could not create a new Mosquitto client instance" << std::endl;
        return 1;
    }

    // Set the callback functions
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_message_callback_set(mosq, on_message);

    // Connect to the MQTT broker
    int rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "Error: Could not connect to the MQTT broker: " << mosquitto_strerror(rc) << std::endl;
        return 1;
    }

    // Run the Mosquitto event loop
    rc = mosquitto_loop_forever(mosq, -1, 1);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "Error: Could not start the Mosquitto event loop: " << mosquitto_strerror(rc) << std::endl;
        return 1;
    }

    // Clean up
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
