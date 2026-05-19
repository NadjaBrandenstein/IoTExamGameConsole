#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "System/MqttClient.h"

extern String playerName;

constexpr uint8_t  PIN_PIR    = 34;
constexpr uint8_t  PIN_BUTTON = 26;

LiquidCrystal_I2C lcd(0x27, 16, 2);

constexpr uint32_t DEBOUNCE_MS      = 200;
constexpr uint32_t TITLE_DISPLAY_MS = 3000;
constexpr uint32_t RESULT_HOLD_MS   = 4000;
constexpr uint32_t PIR_SETTLE_MS    = 3000;  // wait after RED starts before watching
constexpr int      MAX_SCORE        = 10000;
constexpr uint32_t PIR_WARMUP_MS = 45000; // 45 seconds

constexpr uint32_t GREEN_MIN_MS = 2500;
constexpr uint32_t GREEN_MAX_MS = 5000;
constexpr uint32_t RED_MIN_MS   = 1500;
constexpr uint32_t RED_MAX_MS   = 3500;

enum class State : uint8_t {
    TITLE,
    WAITING,
    //COUNTDOWN,
    GREEN_LIGHT,
    RED_LIGHT,
    CAUGHT,
    WIN
};

State    gState        = State::TITLE;
uint32_t gPhaseStart   = 0;
uint32_t gPhaseDuration= 0;
uint32_t gGameStart    = 0;
uint32_t gLastBtnPress = 0;

// PIR settle tracking
bool     gPirReady     = false;
uint32_t gRedEntryTime = 0;

// ---------------- Helpers ----------------

bool movementDetected()
{
    if (digitalRead(PIN_PIR) == HIGH) {
        delay(10);
        return digitalRead(PIN_PIR) == HIGH;
    }
    return false;
}

bool buttonJustPressed()
{
    if (digitalRead(PIN_BUTTON) == LOW) {
        uint32_t now = millis();
        if (now - gLastBtnPress > DEBOUNCE_MS) {
            gLastBtnPress = now;
            return true;
        }
    }
    return false;
}

int calculateScore(uint32_t elapsedMs)
{
    int penalty = static_cast<int>(elapsedMs / 100);
    return max(0, MAX_SCORE - penalty);
}

// ---------------- LCD ----------------

void showTitle()
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print(" GREEN / RED  ");
    lcd.setCursor(0, 1); lcd.print("    LIGHT!    ");
}

void showWaiting()
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Stand still &  ");
    lcd.setCursor(0, 1); lcd.print("press START!   ");
}

void showCountdown(int n)
{
    lcd.clear();
    lcd.setCursor(6, 0); lcd.print("Ready?");
    lcd.setCursor(7, 1); lcd.print(n);
}

void showGreenLight()
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("** GREEN LIGHT *");
    lcd.setCursor(0, 1); lcd.print(" MOVE FORWARD!  ");
}

void showRedLight()
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("*** RED LIGHT **");
    lcd.setCursor(0, 1); lcd.print("   FREEZE!!!    ");
}

void showCaught()
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("  YOU MOVED!!   ");
    lcd.setCursor(0, 1); lcd.print("  GAME OVER! 0  ");
}

void showWin(int score)
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("  REACHED IT!   ");
    lcd.setCursor(0, 1);
    char buf[17];
    snprintf(buf, sizeof(buf), "  Score: %5d  ", score);
    lcd.print(buf);
}

// ---------------- Start Game ----------------

void redGreenStartGame()
{
    Serial.println("Red Green Game START");

    gGameStart = millis();

    gPhaseStart = millis();

    gPhaseDuration =
        random(GREEN_MIN_MS, GREEN_MAX_MS);

    gPirReady = false;

    gRedEntryTime = 0;

    showCountdown(3);

    delay(1000);

    showCountdown(2);

    delay(1000);

    showCountdown(1);

    delay(1000);

    showGreenLight();

    gState = State::GREEN_LIGHT;
}

// ---------------- Setup ----------------

void redGreenInit()
{
    Serial.begin(115200);

    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_PIR,    INPUT_PULLDOWN);

    Wire.begin();
    lcd.init();
    lcd.backlight();

    //randomSeed(analogRead(34));
    randomSeed(analogRead(0));

    showTitle();
    gState = State::TITLE;
}

// ---------------- Loop ----------------

void redGreenUpdate()
{
    switch (gState) {

    case State::TITLE:
        delay(TITLE_DISPLAY_MS);
        showWaiting();
        gState = State::WAITING;
        break;

    case State::WAITING:
    if (millis() < PIR_WARMUP_MS) {
        lcd.setCursor(0, 1);
        lcd.print("Warming up...   ");
        break;
    }
    /* if (buttonJustPressed()) {
        gState = State::COUNTDOWN;
    } */
    break;

    /* case State::COUNTDOWN:
        for (int i = 3; i >= 1; i--) {
            showCountdown(i);
            delay(900);
        }
        gGameStart     = millis();
        gPhaseStart    = millis();
        gPhaseDuration = random(GREEN_MIN_MS, GREEN_MAX_MS);
        showGreenLight();
        gState = State::GREEN_LIGHT;
        break; */

    case State::GREEN_LIGHT:
        if (buttonJustPressed()) {
            uint32_t elapsed = millis() - gGameStart;
            int score = calculateScore(elapsed);
            showWin(score);
            Serial.printf("[WIN] elapsed=%lu ms  score=%d\n", elapsed, score);
            publishScore(playerName, score);
            gState = State::WIN;
            break;
        }
        if (millis() - gPhaseStart >= gPhaseDuration) {
            gPirReady     = false;
            gRedEntryTime = 0;
            gPhaseStart    = millis();
            gPhaseDuration = random(RED_MIN_MS, RED_MAX_MS);
            showRedLight();
            gState = State::RED_LIGHT;
        }
        break;

    case State::RED_LIGHT:
        // Wait for PIR to settle before watching (clears lingering HIGH)
        if (!gPirReady) {
            if (gRedEntryTime == 0) gRedEntryTime = millis();
            if (millis() - gRedEntryTime < PIR_SETTLE_MS) break;
            gPirReady = true;
        }

        if (movementDetected()) {
            showCaught();
            Serial.println("[CAUGHT] movement on red");
            publishScore(playerName, 0);
            gState = State::CAUGHT;
            break;
        }
        if (buttonJustPressed()) {
            uint32_t elapsed = millis() - gGameStart;
            int score = calculateScore(elapsed);
            showWin(score);
            Serial.printf("[WIN on RED] elapsed=%lu ms  score=%d\n", elapsed, score);
            publishScore(playerName, score);
            gState = State::WIN;
            break;
        }
        if (millis() - gPhaseStart >= gPhaseDuration) {
            gPirReady     = false;
            gRedEntryTime = 0;
            gPhaseStart    = millis();
            gPhaseDuration = random(GREEN_MIN_MS, GREEN_MAX_MS);
            showGreenLight();
            gState = State::GREEN_LIGHT;
        }
        break;

        

    case State::CAUGHT:
        delay(RESULT_HOLD_MS);
        showWaiting();
        gState = State::WAITING;
        break;

    case State::WIN:
        delay(RESULT_HOLD_MS);
        showWaiting();
        gState = State::WAITING;
        break;
    }
}