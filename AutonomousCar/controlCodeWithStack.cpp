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
int oldDist[5];
int currDist[5];

void getAverageReadings() {
  currDist[0] = 0;
  currDist[1] = 0;
  currDist[2] = 0;
  currDist[3] = 0;
  currDist[4] = 0;
  int i = 0;
  while (i < 3) {
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
  currDist[0] = currDist[0] / 3;
  currDist[1] = currDist[1] / 3;
  currDist[2] = currDist[2] / 3; 
  currDist[3] = currDist[3] / 3;
  currDist[4] = currDist[4] / 3; 
  
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
  if (currDist[0] <= 40 && currDist[1] <= 40 && currDist[2] <= 40 && currDist[3] <= 40 && currDist[4] <= 40) {
    //car.turnRight(160); 
    car.brake();
  }
  else if (currDist[0] >= 30 && currDist[4] >= 30) {
    //randomSeed(currDist[4]);  //initialize to get pseudo-random number
    long rand = random(200);  //get random number from 0 to 200
    if (rand < 55L) {
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnLeft(10);
        delay(50);
        getAverageReadings();
      }
    }
    else {
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnRight(10);
        delay(50);
        getAverageReadings();
      }
    }
  }
  else if (currDist[4] >= 30 || currDist[3] >= 30) {
    while (currDist[2] <= 80 || currDist[1] <= 40) {
      car.turnRight(10);
      delay(50);
      getAverageReadings();
    }
  }
  else if (currDist[0] >= 30 || currDist[1] >= 30) {
    while (currDist[2] <= 80 || currDist[3] <= 40) {
      car.turnLeft(10);
      delay(100);
      getAverageReadings();
    }
  }
  else {
  }
}


void setup() {
  Serial.begin(9600);
  randomSeed(356);
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
  random(200);
  getAverageReadings();
  if (currDist[2] <= 25) {
    car.brake();
    delay(50);
    dead_end();
  }
  else if (currDist[1] <= 10) {
    car.turnRight(15);
    delay(50);
    getAverageReadings(); 
  }
  else if (currDist[3] <= 10) {
    car.turnLeft(15);
    delay(50);
    getAverageReadings(); 
  }
  else {
    car.forward(currDist[0], currDist[4], oldDist[0], oldDist[4], 0.1);
  }
  oldDist[0] = currDist[0];
  oldDist[1] = currDist[1];
  oldDist[2] = currDist[2];
  oldDist[3] = currDist[3];
  oldDist[4] = currDist[4];
  delay(100);
}