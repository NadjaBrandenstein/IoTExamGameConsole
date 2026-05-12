#include <Arduino.h>

// Pin-mapping for knap 1-9
const int knapper[7] = {
  2, // knap 1
  13,  // knap 2
  14, // knap 3
  26, // knap 4
  25, // knap 5
  4, // knap 6
  12,  // knap 7
};

void setup() {

  // Sæt alle pins som output
  for (int i = 0; i < 7; i++) {
    pinMode(knapper[i], OUTPUT);
    digitalWrite(knapper[i], LOW); // sluk alle
  }

}

void loop() {

  // Tænd knapper en ad gangen
  for (int i = 0; i < 7; i++) {

    digitalWrite(knapper[i], HIGH); // tænd LED
    delay(500);

    digitalWrite(knapper[i], LOW);  // sluk igen
  }

}