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
#include "Stack.h"

/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
bool cont;
Car car(3, 11, 5, 6);
int oldDist[5];
int currDist[5];

//0 is not looked at/none
//1 is left turn
//2 is straight
//3 is right turn
bool turnedBack = false;
int currColor = 0;
int orange = 0;
Stack orangeStack;


/* The function below gets the distance readings from the sensors
 * and stores them in the currDist[] array. It retrieves several
 * values and averages them to help reduce the error
 */
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
}

/* The function below determines the appropriate movement based on the 
 * readings from the sensors
 */
void dead_end() {
  if (currDist[0] <= 40 && currDist[1] <= 40 && currDist[2] <= 40 && currDist[3] <= 40 && currDist[4] <= 40) {
    //encountered a dead end, get color, turn around
    if (currColor == 0) {
      car.brake();
    } else if (currColor = 1) {
      car.turnLeft(180);
      turnedBack = true;
      orange = 3;
    }
    
  }
  else if (currDist[0] >= 30 && currDist[4] >= 30) {
    if (orange == 0) {
      //no color association, turn left as normal
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnLeft(10);
        getAverageReadings();
      }
      orange = 1;
      currColor = 1;
    } else if (orange == 1) {
      //orange means left, turn left
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnLeft(10);
        getAverageReadings();
      }
    } else if (orange == 3) {
      //orange means right, turn right
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnRight(10);
        getAverageReadings();
      }
    }
  }
  else if (currDist[4] >= 30 || currDist[3] >= 30) {
    //open right turn, turn right
    while (currDist[2] <= 80 || currDist[1] <= 40) {
      car.turnRight(10);
      getAverageReadings();
    }
  }
  else if (currDist[0] >= 30 || currDist[1] >= 30) {
    //open left turn, turn left
    while (currDist[2] <= 80 || currDist[3] <= 40) {
      car.turnLeft(10);
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
//get initial readings from the sensors
  getAverageReadings();
  if (currDist[2] <= 25) {
    //obstacle encountered
    car.brake();
    dead_end();
  }
  else if (currDist[1] <= 10) {
    //bear right
    car.turnRight(15);
    getAverageReadings(); 
  }
  else if (currDist[3] <= 10) {
    //bear left
    car.turnLeft(15);
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
