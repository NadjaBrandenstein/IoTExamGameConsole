#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>

// Shared variables
extern String playerName;
extern String gameName;
extern String pubTopic;
extern String subTopic;
extern String deviceId;

// Functions
void mqttSetup();
void mqttLoop();
void publishScore(int score);
bool connectMqtt();

#endif