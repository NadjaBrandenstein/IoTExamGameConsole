#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "MqttClient.h"
#include "GameManager.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Shared variables
String playerName = "";
String gameName = "";
String deviceId = "firebeetle01";
void birdieStartGame();
void whackStartGame();

// Topics 
String pubTopic;
String subTopic;
 
// ---------------- WIFI ----------------

void connectWiFi() {
    Serial.print("Connecting to WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println(" connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

// ---------------- MQTT CALLBACK ----------------

// This function is called when a message arrives on a subscribed topic
void onMessageReceived(char* topic, byte* payload, unsigned int length)
{
    Serial.println("MESSAGE RECEIVED!");

    StaticJsonDocument<256> doc;

    DeserializationError error =
        deserializeJson(doc, payload, length);

    if (error)
    {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }

    const char* game = doc["game"];
    const char* action = doc["action"];
    const char* player = doc["playerName"];

    Serial.println("---- MQTT COMMAND ----");

    if (game)
        Serial.println(game);

    if (action)
        Serial.println(action);

    // Save player name globally
    if (player)
    {
        playerName = String(player);
    }

    if (!game || !action)
        return;

    // ---------------- BIRDIE SAYS ----------------

    if (
        strcmp(game, "birdiesays") == 0 &&
        strcmp(action, "start") == 0
    )
    {
        gameName = "birdiesays";

        Serial.println("Starting Birdie Says");

        setGame(GAME_BIRDIE_SAYS);

        birdieStartGame();
    }

    // ---------------- WHACK A BIRD ----------------

    if (
        strcmp(game, "whackabird") == 0 &&
        strcmp(action, "start") == 0
    )
    {
        gameName = "whackabird";

        Serial.println("Starting Whack A Bird");

        setGame(GAME_WACK_A_BIRD);

        whackStartGame();
    }
}

// ---------------- PUBLISH ----------------

void publishScore(String playerName, int score) {

    char payload[150];

    snprintf(payload, sizeof(payload),
        "{\"playerName\":\"%s\",\"game\":\"%s\",\"score\":%d}",
        playerName.c_str(),
        gameName.c_str(),
        score
    );

    Serial.println("Publishing score:");
    Serial.println(payload);

    mqttClient.publish(pubTopic.c_str(), payload);
}

// ---------------- CONNECT MQTT ----------------

bool connectMqtt() {
    
    if (mqttClient.connected()) {
        return true;
    }

    Serial.print("Connecting to MQTT...");

    String clientId = "esp32-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (mqttClient.connect(clientId.c_str(), MQTT_TOKEN, "")) {
        
        Serial.println(" connected!");

        // Subscribe to topic
        if (mqttClient.subscribe(subTopic.c_str())) {
            bool subscribed = mqttClient.subscribe(subTopic.c_str());

            Serial.print("Subscribe result: ");
            Serial.println(subscribed);

            Serial.print("Subscribed topic: ");
            Serial.println(subTopic);
        } else {
            Serial.println("Subscription failed");
        }

        return true;

    } else {
        Serial.print(" failed, rc=");
        Serial.println(mqttClient.state());
        return false;
    }
}

// ---------------- SETUP ----------------

void mqttSetup() {
    
    Serial.println("\nMQTT Setup");
    Serial.println("======================");

    //pinMode(LDR_PIN, INPUT);

    connectWiFi();

    // Default topics
    pubTopic = "iot/game/firebeetle01/score";
    subTopic = "iot/game/firebeetle01/command";

    Serial.print("Publish topic: ");
    Serial.println(pubTopic);

    Serial.print("Subscribe topic: ");
    Serial.println(subTopic);

    // Configure MQTT client
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(onMessageReceived);

    Serial.println("MQTT callback registered");


    // Connect and subscribe
    connectMqtt();

}

// ---------------- LOOP ----------------

void mqttLoop() {

    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected, trying to reconnect...");
        connectWiFi();
    }

    if (!mqttClient.connected()) {
        connectMqtt();
    }

    mqttClient.loop(); // REQUIRED for receiving messages


     /* if (millis() - lastPublish > 5000) {
        lastPublish = millis();
        publishSensorData();
    } */

    delay(10); 
}