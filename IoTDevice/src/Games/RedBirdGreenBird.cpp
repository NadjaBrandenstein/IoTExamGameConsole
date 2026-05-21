#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "System/MqttClient.h"

extern String playerName;

constexpr uint8_t  PIN_PIR    = 15;
constexpr uint8_t  PIN_BUTTON = 26;

// ── LED pins ─────────────────────────────────────────────────
constexpr uint8_t LED_PINS[]  = {2, 4, 12, 13, 14, 25};
constexpr uint8_t LED_COUNT   = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

LiquidCrystal_I2C lcd(0x27, 16, 2);

constexpr uint32_t DEBOUNCE_MS      = 200;
constexpr uint32_t TITLE_DISPLAY_MS = 3000;
constexpr uint32_t RESULT_HOLD_MS   = 4000;
constexpr uint32_t PIR_SETTLE_MS    = 3000;
constexpr int      MAX_SCORE        = 10000;

constexpr uint32_t GREEN_MIN_MS = 2500;
constexpr uint32_t GREEN_MAX_MS = 5000;
constexpr uint32_t RED_MIN_MS   = 1500;
constexpr uint32_t RED_MAX_MS   = 3500;

enum class State : uint8_t {
    TITLE,
    WAITING,
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

bool     gPirReady     = false;
uint32_t gRedEntryTime = 0;

// ---------------- LCD Helpers ----------------

void ledsOn()
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
        digitalWrite(LED_PINS[i], HIGH);
}

void ledsOff()
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
        digitalWrite(LED_PINS[i], LOW);
}

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
    int penalty = static_cast<int>(elapsedMs / 6);
    return max(0, MAX_SCORE - penalty);
}

// ---------------- LCD Helpers ----------------

void showTitle()
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print(" GREEN / RED  ");
    lcd.setCursor(0, 1); lcd.print("    BIRD!    ");
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
    lcd.setCursor(0, 0); lcd.print("** GREEN BIRD *");
    lcd.setCursor(0, 1); lcd.print(" MOVE FORWARD!  ");
}

void showRedLight()
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("*** RED BIRD **");
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

    // Init all LED pins as OUTPUT and turn off
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        pinMode(LED_PINS[i], OUTPUT);
        digitalWrite(LED_PINS[i], LOW);
    }

    Wire.begin();
    lcd.init();
    lcd.backlight();

    randomSeed(analogRead(0));

    showTitle();
    gState = State::TITLE;
}

// ---------------- Loop ----------------

void redGreenUpdate()
{
    switch (gState) {

    case State::TITLE:
        ledsOff();
        delay(TITLE_DISPLAY_MS);
        showWaiting();
        gState = State::WAITING;
        break;

    case State::WAITING:
    // Light up the start indicator
    pinMode(PIN_BUTTON, OUTPUT);
    digitalWrite(PIN_BUTTON, HIGH);
    delay(100);
    
    // Switch back to input to read button
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    break;

    case State::GREEN_LIGHT:
        if (buttonJustPressed()) {
            uint32_t elapsed = millis() - gGameStart;
            int score = calculateScore(elapsed);
            ledsOff();
            showWin(score);
            Serial.printf("[WIN] elapsed=%lu ms  score=%d\n", elapsed, score);
            publishScore(playerName, score);
            gState = State::WIN;
            break;
        }
        if (millis() - gPhaseStart >= gPhaseDuration) {
            ledsOff();  // RED phase starts → LEDs off
            gPirReady     = false;
            gRedEntryTime = 0;
            gPhaseStart    = millis();
            gPhaseDuration = random(RED_MIN_MS, RED_MAX_MS);
            showRedLight();
            gState = State::RED_LIGHT;
        }
        break;

    case State::RED_LIGHT:
        if (!gPirReady) {
            if (gRedEntryTime == 0) gRedEntryTime = millis();
            if (millis() - gRedEntryTime < PIR_SETTLE_MS) break;
            gPirReady = true;
        }

        if (movementDetected()) {
            ledsOff();
            showCaught();
            Serial.println("[CAUGHT] movement on red");
            publishScore(playerName, 0);
            gState = State::CAUGHT;
            break;
        }
        if (buttonJustPressed()) {
            uint32_t elapsed = millis() - gGameStart;
            int score = calculateScore(elapsed);
            ledsOff();
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
            ledsOn();   // Back to GREEN → LEDs on
            gState = State::GREEN_LIGHT;
        }
        break;

    case State::CAUGHT:
    // Blink all LEDs 5 times
    for (int i = 0; i < 5; i++) {
        ledsOn();
        delay(200);
        ledsOff();
        delay(200);
    }
    delay(RESULT_HOLD_MS);
    showWaiting();
    gState = State::WAITING;
    break;

    case State::WIN:
    // Wave on
    for (int i = 0; i < LED_COUNT; i++) {
        digitalWrite(LED_PINS[i], HIGH);
        delay(80);
    }
    // Wave off
    for (int i = 0; i < LED_COUNT; i++) {
        digitalWrite(LED_PINS[i], LOW);
        delay(80);
    }
    // Repeat 3 times total
    for (int repeat = 0; repeat < 2; repeat++) {
        for (int i = 0; i < LED_COUNT; i++) {
            digitalWrite(LED_PINS[i], HIGH);
            delay(80);
        }
        for (int i = 0; i < LED_COUNT; i++) {
            digitalWrite(LED_PINS[i], LOW);
            delay(80);
        }
    }
    delay(RESULT_HOLD_MS);
    showWaiting();
    gState = State::WAITING;
    break;
    }
}