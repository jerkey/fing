#include "R311.h"

void setup() {
  Serial.begin(57600); // USB virtual comm port, talking to computer
  while (!Serial) {} // wait for serial port to connect. Needed for native USB port only
  Serial.println("native USB port reporting for communications");
  R311 fingReader = R311(&Serial1); // hardware serial port, talking to fingerprint reader
}

void loop() {
  if (Serial.available() > 0) {
    // get incoming byte:
    byte inByte = Serial.read();
  }
  Serial.println(millis());
  delay(random(1000));
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}
