#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MqttClient.h"

extern String playerName;

// ---------------- PINS ----------------

const int N = 7;
const int pins[N] = {2, 13, 14, 26, 25, 4, 12};

// ---------------- GAME ----------------

static int score = 0;

bool whackGameRunning = false;

const unsigned long GAME_TIME = 30000;
unsigned long gameStart = 0;

// ---------------- LCD ----------------

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

static LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// ---------------- LCD HELPERS ----------------

void whackDrawLCD(const char* line1, const char* line2)
{
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print(line1);

    lcd.setCursor(0,1);
    lcd.print(line2);
}

void whackUpdateLCD(int timeLeft)
{
    char l1[16];
    char l2[16];

    sprintf(l1, "Score:%d", score);
    sprintf(l2, "Time:%d", timeLeft);

    whackDrawLCD(l1, l2);
}

void whackGameOverScreen()
{
    char l1[16];
    char l2[16];

    sprintf(l1, "GAME OVER");
    sprintf(l2, "Score:%d", score);

    whackDrawLCD(l1, l2);
}

// ---------------- GAME CONTROL ----------------

void whackStartGame()
{
    Serial.println("Whack-A-Bird START");

    score = 0;

    gameStart = millis();

    whackGameRunning = true;

    whackDrawLCD("Whack-A-Bird", "START!");
    delay(1000);
}

// ---------------- LED HELPERS ----------------

void allPinsInput()
{
    for(int i = 0; i < N; i++)
    {
        pinMode(pins[i], INPUT_PULLUP);
    }
}

void allLEDsOff()
{
    for(int i = 0; i < N; i++)
    {
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
    }
}

// ---------------- TARGET ----------------

bool targetRound(int pin, unsigned long timeoutMs)
{
    // LIGHT LED
    pinMode(pin, OUTPUT);

    digitalWrite(pin, HIGH);

    delay(50);

    // IMPORTANT:
    // switch ALL back to buttons
    allPinsInput();

    unsigned long start = millis();

    while(millis() - start < timeoutMs)
    {
        if(digitalRead(pin) == LOW)
        {
            delay(25);

            if(digitalRead(pin) == LOW)
            {
                Serial.println("HIT");

                allLEDsOff();

                return true;
            }
        }

        delay(5);
    }

    Serial.println("MISS");

    allLEDsOff();

    return false;
}

// ---------------- SETUP ----------------

void whackInit()
{
    Serial.begin(115200);

    randomSeed(millis());

    Wire.begin(21, 22);

    lcd.init();
    lcd.begin(16,2);
    lcd.backlight();

    whackDrawLCD("Whack-A-Bird", "Ready!");

    delay(1500);

    allPinsInput();

    Serial.println("Whack-A-Bird Ready!");
}

// ---------------- LOOP ----------------

void whackUpdate()
{
    // GAME NOT RUNNING
    if(!whackGameRunning)
        return;

    // ---------------- GAME OVER ----------------

    if(millis() - gameStart >= GAME_TIME)
    {
        Serial.println("GAME OVER");

        Serial.print("Final Score: ");
        Serial.println(score);

        publishScore(playerName, score);

        whackGameOverScreen();

        delay(5000);

        whackDrawLCD("Whack-A-Bird", "Ready!");

        whackGameRunning = false;

        return;
    }

    // ---------------- ROUND ----------------

    int idx = random(0, N);

    int pin = pins[idx];

    int timeLeft =
        (GAME_TIME - (millis() - gameStart)) / 1000;

    bool hit = targetRound(pin, 2000);

    if(hit)
    {
        score++;
    }
    else
    {
        score--;
    }

    whackUpdateLCD(timeLeft);

    delay(1000);
}