#include <Arduino.h>
#include "MqttClient.h"
#include "GameManager.h"

void setup() {
    Serial.begin(115200);

    mqttSetup();

    Serial.println("\nGame Console Ready");
}

void loop() {
    mqttLoop();
    updateGame();
    delay(10);
}