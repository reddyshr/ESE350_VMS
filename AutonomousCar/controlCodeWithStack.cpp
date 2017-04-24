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
#include <PixyI2C.h>
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


PixyI2C pixy(0x55); // You can set the I2C address through PixyI2C object  
int colorSignature;
int i;


int getColor() {
  int maxBlock = -1;
  int maxArea = 0;
  int blocks = pixy.getBlocks();
  if (blocks != 0) {
    for (int i = 0; i < blocks; i++) {
      int area = pixy.blocks[i].width * pixy.blocks[i].height;
      if (area >= maxArea) {
        maxArea = area;
        maxBlock = i;
      }
    }
  }
  return maxBlock;
}


void decode_color(int signature) {
  if(signature == 1) { 
      Serial.println("Orange"); 
    }
    else if (signature == 2) {
      Serial.println("Green"); 
    }
    else if (signature == 3) { 
      Serial.println("Pink"); 
    }
    else if (signature == 4) {
      Serial.println("Yellow");
    }
    else { 
      Serial.println("No Object Detected");
    }
}


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
  /*
  Serial.print("s1: ");
  Serial.print(currDist[0]);
  Serial.print(" s2: ");
  Serial.print(currDist[1]);
  Serial.print(" s3: ");
  Serial.print(currDist[2]);
  Serial.print(" s4: ");
  Serial.print(currDist[3]);
  Serial.print(" s5: ");
  Serial.println(currDist[4]);*/
}

void dead_end() {
  if (currDist[0] <= 50 && currDist[1] <= 50 && currDist[2] <= 50 && currDist[3] <=50 && currDist[4] <= 50) {
    car.brake(); 
    if (currColor = 1) {
       turnedBack = true;
       orange = 3;
       car.turnLeft(90);
       car.turnLeft(90);
    }
  }
  else if (currDist[0] >= 30 && currDist[4] >= 30) {
     //int val = getColor(); 
     //while (val == -1) {
       //val = getColor(); 
     //}
     //in val = 1;
    //colorSignature = pixy.blocks[val].signature;
    //currColor = colorSignature;
    colorSignature = 1;
    currColor = 1;
   // decode_color(pixy.blocks[val].signature); 
    if (colorSignature == 1 && orange == 1) {
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnLeft(10);
        delay(100);
        getAverageReadings();
      }
    } else if (colorSignature == 1 && orange == 3)
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnRight(10);
        delay(100);
        getAverageReadings();
      }
    } else if (currColor == 1 && orange == 0 && !turnedBack) {
      orange = 1;
      orangeStack.pushVal(1);
      while (currDist[2] <= 80 || currDist[3] <= 40) {
        car.turnLeft(10);
        delay(100);
        getAverageReadings();
      }
  } else if (currDist[4] >= 30 || currDist[3] >= 30) {
    if (currColor = 1 && !turnedBack) {
        orangeStack.pushVal(3);
      } else if (currColor == 1 && orangeStack.getSize() >= 1 && turnedBack) {
        int val = orangeStack.popVal();
        if (val != 1) {
          while(1);
        }
      } else if (currColor == 1 && orangeStack.getSize() == 0 && turnedBack) {
        turnedBack = false;
        currColor = 0;
    }
    while (currDist[2] <= 80 || currDist[1] <= 40) {
      car.turnRight(10);
      delay(50);
      getAverageReadings();
    }
  }
  else if (currDist[0] >= 30 || currDist[1] >= 30) {
    if (currColor = 1 && !turnedBack) {
        orangeStack.pushVal(1);
      } else if (currColor == 1 && orangeStack.getSize() >= 1 && turnedBack) {
        int val = orangeStack.popVal();
        if (val != 3) {
          while(1);
        }
      } else if (currColor == 1 && orangeStack.getSize() == 0 && turnedBack) {
        turnedBack = false;
        currColor = 0;
    }
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
  Wire.begin();
  pixy.init();
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
  i = 0;
}

void loop() {
  
  getAverageReadings();
  if (currDist[2] <= 25) {
    car.brake();
    delay(50);
    dead_end();
  }
  else if (currDist[2] >= 50 && currDist[4] >= 50) {
    car.turnRight(45);
    //get color reading
  }
  else if (currDist[2] >= 50 && currDist[0] >= 50) {
    car.turnLeft(45);
    //get color reading
  }
  else if (currDist[1] <= 15) {
    car.turnRight(15);
    delay(50);
    getAverageReadings(); 
  }
  else if (currDist[3] <= 15) {
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