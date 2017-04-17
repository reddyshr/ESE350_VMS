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
int oldDist[5];
int currDist[5];
void initSensors()
{
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}

void dead_end() {
  if (currDist[0] >= 20 && currDist[1] >= 20 && currDist[3] >= 20 && currDist[4] >= 20) {
    //OPEN LEFT AND RIGHT TURNS
    randomSeed(currDist[2]);  //initialize to get pseudo-random number
    long rand = random(200);    //print random number from 0 to 200
    if (rand < 100L) {
      //MAKE LEFT TURN
      while(currDist[2] <= 20) {
        //TURN LEFT ONE DEGREE
      }
    }
    else {
      //MAKE RIGHT TURN
      while (currDist[2] <= 20)  {
        //TURN RIGHT ONE DEGREE
      }
    }
  }
  else if (currDist[0] >= 20 && currDist[1] >= 20) {
    //OPEN LEFT TURN
  }
  else if (currDist[4] >= 20 && currDist[3] >= 20) {
    //OPEN RIGHT TURN
  }
  else {
    //DEAD END, TURN AROUND 180 DEGREES
    car.turnLeft(180, dof, mag);
  }
}


void setup() {
  Serial.begin(9600);
  initSensors();
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
  /*Wire.requestFrom(8, 5);  
  while (Wire.available()) {
    currDist[0] = Wire.read();
    currDist[1] = Wire.read();
    currDist[2] = Wire.read();
    currDist[3] = Wire.read();
    currDist[4] = Wire.read();
  }*/
  if (currDist[0] <= 20 || currDist[1] <= 20 || currDist[2] <= 20) {
    //REACH A WALL OR DEAD END
    //dead_end();  
    car.turnLeft(90, dof, mag);
  }
  else {
    car.forward(currDist[0], currDist[4], oldDist[0], oldDist[4], 0.1, dof, mag);
  }
  //COPY DISTANCE READINGS FOR NEXT ITERATION
  oldDist[0] = currDist[0];
  oldDist[1] = currDist[1];
  oldDist[2] = currDist[2];
  oldDist[3] = currDist[3];
  oldDist[4] = currDist[4];
  delay(50);
}
