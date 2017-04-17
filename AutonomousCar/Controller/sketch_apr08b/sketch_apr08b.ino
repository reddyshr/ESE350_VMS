#include "Wire.h"

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("asdf");
}

void loop() {
  Wire.requestFrom(8, 1);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    Serial.print("SensorReading: ");   
    Serial.println(c);         // print the character
  }

  delay(10);
}

