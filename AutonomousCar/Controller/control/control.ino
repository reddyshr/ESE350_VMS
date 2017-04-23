#include <avr/io.h>
#include <stdio.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include "Motor.h"
#include "Car.h"
#include "PID.h"
/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
bool cont;
Car car(3, 11, 5, 6);
//int oldLeftDist = 0;
//int oldRightDist = 0;
//int currLeftDist;
//int currRightDist;
//int frontDist;
int oldDist[5];
int currDist[5];

void getAverageReadings() {
  currDist[0] = 0;
  currDist[1] = 0;
  currDist[2] = 0;
  currDist[3] = 0;
  currDist[4] = 0;
  int i = 0;
  while (i < 1) {
    Wire.requestFrom(8, 5);
    if (Wire.available()) {
      currDist[0] = currDist[0] + Wire.read();
      currDist[1] = currDist[1] + Wire.read();
      currDist[2] = currDist[2] + Wire.read();
      currDist[3] = currDist[3] + Wire.read();
      currDist[4] = currDist[4] + Wire.read(); 
    }
    i++;
  }
  /*currDist[0] = currDist[0] / 3;
  currDist[1] = currDist[1] / 3;
  currDist[2] = currDist[2] / 3; 
  currDist[3] = currDist[3] / 3;
  currDist[4] = currDist[4] / 3; */

  Serial.print("s1: ");
  Serial.print(currDist[0]);
  Serial.print(" s2: ");
  Serial.print(currDist[1]);
  Serial.print(" s3: ");
  Serial.print(currDist[2]);
  Serial.print(" s4: ");
  Serial.print(currDist[3]);
  Serial.print(" s5: ");
  Serial.println(currDist[4]);
}

void dead_end() { 
}


void setup() {
  Serial.begin(9600);
  Wire.begin();
  oldDist[0] = 0;
  oldDist[1] = 0;
  oldDist[2] = 0;
  oldDist[3] = 0;
  oldDist[4] = 0;
  currDist[0] = 0;
  currDist[1] = 0;
  currDist[2] = 0;
  currDist[3] = 0;
  currDist[4] = 0;
}

void loop() {
  getAverageReadings();
  /*if (currDist[2] <= 30) {
    car.turnRight(100);
    delay(1000);
  }
  else {
     car.forward(currDist[0], currDist[4], oldDist[0], oldDist[4], 0.1);
     delay(100);
  }*/
  if (currDist[2] <= 40 || currDist[1] <= 30) {
    car.brake();
    delay(1000);
    while (currDist[2] <= 100 || currDist[1] <= 40) {
      car.turnRight(15);
      getAverageReadings();
      delay(500);
    }
  }
  else {
    car.forward(currDist[0], currDist[4], oldDist[0], oldDist[4], 0.1);
  }
  //COPY DISTANCE READINGS FOR NEXT ITERATION
  oldDist[0] = currDist[0];
  oldDist[1] = currDist[1];
  oldDist[2] = currDist[2];
  oldDist[3] = currDist[3];
  oldDist[4] = currDist[4];
  delay(1000);
}
