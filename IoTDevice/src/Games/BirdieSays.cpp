#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "System/MqttClient.h"

extern String playerName;

const int N = 7;

// Shared GPIO pins (LED + button on same pin)
const int pins[N] = {2, 13, 14, 26, 25, 4, 12};

int sequence[50];
int seqLen = 2;
static int score = 0;
bool gameRunning = false;

// LCD
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

static LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// ---------------- LCD HELPERS ----------------

void drawLCD(const char* line1, const char* line2){

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(line1);

  lcd.setCursor(0,1);
  lcd.print(line2);
}

void updateLCD(int timeLeft){

  char l1[16];
  char l2[16];

  sprintf(l1, "Score:%d", score);
  sprintf(l2, "Time:%d", timeLeft);

  drawLCD(l1, l2);
}

void gameOverScreen(){

  char l1[16];
  char l2[16];

  sprintf(l1, "GAME OVER");
  sprintf(l2, "Score:%d", score);

  drawLCD(l1, l2);
}

// ---------------- GAME CONTROL ----------------

void birdieStartGame() {

    score = 0;
    seqLen = 2;

    for(int i = 0; i < seqLen; i++){
        sequence[i] = random(0, N);
    }

    gameRunning = true;

    Serial.println("Game started!");
}

// ---------------- LED ----------------

void showLED(int pin){

  pinMode(pin, OUTPUT);

  digitalWrite(pin, HIGH);
  delay(300);

  digitalWrite(pin, LOW);
  delay(80);

  pinMode(pin, INPUT_PULLUP);
}

// ---------------- SEQUENCE ----------------

void showSequence(){

  Serial.println("\n--- SHOW SEQUENCE ---");

  drawLCD("WATCH!", "Memorize");
  delay(800);

  for(int i = 0; i < seqLen ; i++){

    int idx = sequence[i];

    Serial.print("Step ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(idx);

    showLED(pins[idx]);
  }

  for(int i = 0 ; i < N; i++){
    pinMode(pins[i], INPUT_PULLUP);
  }

  Serial.println("--- END SEQUENCE ---\n");
}

// ---------------- PLAYER ----------------

bool playerTurn(){

  Serial.println("--- PLAYER TURN ---");

  for(int i = 0; i < seqLen; i++){

    bool correct = false;
    unsigned long start = millis();
    int lastTime = -1;

    while(millis() - start < 5000){

      int timeLeft = 5 - ((millis() - start) / 1000);

      if(timeLeft != lastTime){
        updateLCD(timeLeft);
        lastTime = timeLeft;
      }

      for(int b = 0; b < N; b++){

        pinMode(pins[b], INPUT_PULLUP);

        if(digitalRead(pins[b]) == LOW){

          delay(25);

          if(digitalRead(pins[b]) == LOW){

            Serial.print("Pressed: ");
            Serial.println(b);

            if(b == sequence[i]){
              score++;
              correct = true;
              delay(200);
              break;
            } else {
              return false;
            }
          }
        }
      }

      if(correct) break;
    }

    if(!correct) return false;
  }

  return true;
}

// ---------------- GAME LOGIC ----------------

void addStep(){
  sequence[seqLen] = random(0, N);
  seqLen++;
}

// ---------------- SETUP ----------------

void birdieInit(){

  Serial.begin(115200);
  randomSeed(millis());

  Wire.begin(21, 22);   // ESP32 I2C pins

  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();

  drawLCD("Birdie Says", "Ready!");
  delay(1500);

  for(int i = 0; i < N; i++){
    pinMode(pins[i], INPUT_PULLUP);
  }

  Serial.println("Birdie Says Ready!");
}

// ---------------- LOOP ----------------

void birdieUpdate(){

  if (!gameRunning) return;

  showSequence();

  if (!gameRunning) return;

  bool ok = playerTurn();

  if (!gameRunning) return;

  if(ok){

    Serial.println("Round cleared!");
    addStep();

    Serial.print("Score: ");
    Serial.println(score);

    delay(500);

  } else {

    Serial.println("GAME OVER");
    Serial.print("Final Score: ");
    Serial.println(score);

    publishScore(playerName, score);

    gameOverScreen();
    delay(3000);

    gameRunning = false;

    return;
  }
}