#include <Arduino.h>

/* // Pin-mapping for knap 1-9
const int knapper[9] = {
  12, // knap 1
  4,  // knap 2
  16, // knap 3
  17, // knap 4
  13, // knap 5
  14, // knap 6
  2,  // knap 7
  26, // knap 8
  25  // knap 9
};

void setup() {

  // Sæt alle pins som output
  for (int i = 0; i < 9; i++) {
    pinMode(knapper[i], OUTPUT);
    digitalWrite(knapper[i], LOW); // sluk alle
  }

}

void loop() {

  // Tænd knapper en ad gangen
  for (int i = 0; i < 9; i++) {

    digitalWrite(knapper[i], HIGH); // tænd LED
    delay(500);

    digitalWrite(knapper[i], LOW);  // sluk igen
  }

} */


