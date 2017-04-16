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
Car car(5, 6, 11, 3);
int oldLeftDist = 0;
int oldRightDist = 0;
int currLeftDist;
int currRightDist;
int frontDist;

void initSensors()
{
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}


void setup() {
  Serial.begin(9600);
  initSensors();
  Wire.begin();
  Serial.println("asdf");
}

void loop() {
  Wire.requestFrom(8, 3);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    currLeftDist = Wire.read(); // receive a byte as character
    currRightDist = Wire.read();
    frontDist = Wire.read();
  }
 // car.forward(currLeftDist, currRightDist, oldLeftDist, oldRightDist, 0.1, dof, mag);
  oldLeftDist = currLeftDist;
  oldRightDist = currRightDist;
  delay(50);
}
