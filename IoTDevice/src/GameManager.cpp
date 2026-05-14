#include <Arduino.h>
#include "GameManager.h"
#include "MqttClient.h"
#include "config.h"

// External game functions
void birdieInit();
void birdieUpdate();

void whackInit();
void whackUpdate();

// External MQTT variables (defined in MqttClient.cpp)
/* extern String playerName;
extern String gameName;
extern String pubTopic;
extern String subTopic;

extern const char* MQTT_BASE_TOPIC;  */

GameState currentGame = GAME_NONE;

void setGame(GameState newGame) {

    currentGame = newGame;

    switch(currentGame) {

        case GAME_BIRDIE_SAYS:
            Serial.println("Starting Birdie Says...");
            gameName = "birdiesays";
            birdieInit();
            break;

        case GAME_WACK_A_BIRD:
            Serial.println("Starting Whack-A-Bird...");
            gameName = "wack";
            whackInit();
            break;

        default:
            Serial.println("No game selected");
            break;
    }

    // Update topics
    pubTopic = String(MQTT_BASE_TOPIC) + "/" + deviceId + "/score";
    subTopic = String(MQTT_BASE_TOPIC) + "/" + deviceId + "/command";

    Serial.print("Pub topic: ");
    Serial.println(pubTopic);

    Serial.print("Sub topic: ");
    Serial.println(subTopic);

    // Reconnect MQTT to apply new topic
    connectMqtt();
}

void updateGame() {

    switch(currentGame) {
        case GAME_BIRDIE_SAYS:
            birdieUpdate();
            break;

        case GAME_WACK_A_BIRD:
            whackUpdate();
            break;

        default:
            break;
    }
}