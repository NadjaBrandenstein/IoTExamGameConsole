#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MqttClient.h"

const int knapper[7] = {2, 13, 14, 26, 25, 4, 12};

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

void drawLCD(int timeLeft){

  char l1[16];
  char l2[16];

  sprintf(l1, "Score:%d", score);
  sprintf(l2, "Time:%d", timeLeft);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(l1);

  lcd.setCursor(0,1);
  lcd.print(l2);
}

// ---------------- GAME ----------------

bool checkHit(int pin, unsigned long timeoutMs) {

    unsigned long start = millis();

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    while (millis() - start < timeoutMs) {

        pinMode(pin, INPUT_PULLUP);
        delayMicroseconds(100);

        if (digitalRead(pin) == LOW) {
            delay(25);

            if (digitalRead(pin) == LOW) {

                digitalWrite(pin, LOW);
                return true;
            }
        }

        delay(5);
    }

    digitalWrite(pin, LOW);
    return false;
}

// ---------------- SETUP ----------------

void wackInit() {

    Serial.begin(115200);
    randomSeed(analogRead(34));

    Wire.begin(21, 22);

    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Whac-A-Mole");
    lcd.setCursor(0,1);
    lcd.print("Ready!");

    delay(1500);
    score = 0;
}

// ---------------- LOOP ----------------

void wackUpdate() {

    gameStart = millis();
    score = 0;

    while (millis() - gameStart < GAME_TIME) {

        int aktiv = random(0, 7);

        int timeLeft = (GAME_TIME - (millis() - gameStart)) / 1000;

        bool ramt = checkHit(knapper[aktiv], 2000);

        pinMode(knapper[aktiv], OUTPUT);
        digitalWrite(knapper[aktiv], LOW);

        if (ramt) score++;
        else score--;

        drawLCD(timeLeft);

        delay(200);
    }

    // ---------------- GAME OVER ----------------

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GAME OVER");

    lcd.setCursor(0,1);
    lcd.print("Score:");
    lcd.print(score);

    Serial.print("Final Score: ");
    Serial.println(score);

    publishScore(score);

    delay(5000);
}