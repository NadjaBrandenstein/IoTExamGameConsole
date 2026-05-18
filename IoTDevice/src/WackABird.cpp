#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MqttClient.h"

const int pins[7] = {2, 13, 14, 26, 25, 4, 12};

static int score = 0;

// GAME TIMER
const unsigned long GAME_TIME = 30000;
unsigned long gameStart = 0;

// LCD
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

static LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// ---------------- LCD ----------------

void drawLCD(int timeLeft)
{
    char l1[16];
    char l2[16];

    sprintf(l1, "Score:%d", score);
    sprintf(l2, "Time:%d", timeLeft);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(l1);

    lcd.setCursor(0, 1);
    lcd.print(l2);
}

// ---------------- LED CONTROL ----------------

void setAllOff()
{
    for (int i = 0; i < 7; i++)
    {
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
    }
}

void showTarget(int pin)
{
    setAllOff();

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

// ---------------- BUTTON CHECK ----------------

bool checkButton(int pin, unsigned long timeoutMs)
{
    unsigned long start = millis();

    while (millis() - start < timeoutMs)
    {
        if (digitalRead(pin) == LOW)
        {
            delay(25); // debounce

            if (digitalRead(pin) == LOW)
            {
                return true;
            }
        }

        delay(5);
    }

    return false;
}

// ---------------- SETUP ----------------

void whackInit()
{
    Serial.begin(115200);
    randomSeed(analogRead(34));

    Wire.begin(21, 22);

    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Whac-A-Bird");
    lcd.setCursor(0, 1);
    lcd.print("Ready!");

    delay(1500);

    score = 0;

    // IMPORTANT: set buttons as inputs ONCE
    for (int i = 0; i < 7; i++)
    {
        pinMode(pins[i], INPUT_PULLUP);
    }

    setAllOff();
}

// ---------------- LOOP ----------------

void whackUpdate()
{
    gameStart = millis();
    score = 0;

    while (millis() - gameStart < GAME_TIME)
    {
        int idx = random(0, 7);
        int pin = pins[idx];

        int timeLeft = (GAME_TIME - (millis() - gameStart)) / 1000;

        // show target
        showTarget(pin);

        bool hit = checkButton(pin, 1200);

        // turn off LED
        digitalWrite(pin, LOW);

        if (hit)
        {
            score++;
        }
        else
        {
            score--;
        }

        drawLCD(timeLeft);

        delay(150);
    }

    // ---------------- GAME OVER ----------------

    setAllOff();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GAME OVER");

    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(score);

    Serial.print("Final Score: ");
    Serial.println(score);

    publishScore(playerName, score);

    delay(5000);
}