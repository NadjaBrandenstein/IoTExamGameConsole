#include <Arduino.h>
#include "Core/GameManager.h"
#include "System/MqttClient.h"
#include "config.h"

// External game functions
void birdieInit();
void birdieUpdate();

void whackInit();
void whackUpdate();

void redGreenInit();
void redGreenUpdate();

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
            gameName = "whackabird";
            whackInit();
            break;

        case GAME_RED_BIRD_GREEN_BIRD:
            Serial.println("Starting Red Bird Green Bird...");
            gameName = "redbirdgreenbird";
            redGreenInit();
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

        case GAME_RED_BIRD_GREEN_BIRD:
            redGreenUpdate();
            break;

        default:
            break;
    }
}