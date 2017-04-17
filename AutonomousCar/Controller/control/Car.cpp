/*
 * Car.cpp
 *
 * Created: 3/28/2017 10:49:13 PM
 *  Author: Shreetej
 */ 

#include "Car.h"
#include "Motor.h"
#include "PID.h"
#include <stdio.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>


/*Car::Car(int leftCWPin, int leftCCWPin, int rightCWPin, int rightCCWPin) {
	//Motor leftMotor(leftCWPin, leftCCWPin);
	//Motor rightMotor(rightCWPin, rightCCWPin);
	//Motor leftMotor(leftCWPin, leftCCWPin);
	//Motor rightMotor(rightCWPin, rightCCWPin);
}*/
//AS OF NOW, A BASE SPEED OF 60 WILL BE USED

//YOU MAY NEED TO ADD DIFFERENT PID CONTROLLERS BASED ON SOURCE ERROR
void Car::forward(int currLDist, int currRDist, int oldLDist, int oldRDist, float dt, Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag) {
  //PID Control for if car is not in open space. Left and RIght DIstances are not big. 
  int baseSpeed = 60; 
  float kp = PID::getKP();
  float kd = PID::getKD();
  //float kp = 10.5;
  //float kd = 5.5;
  int old_error = oldLDist - oldRDist;
  int error = currLDist - currRDist;
  int derivative = (float) (error - old_error) / dt;
  float output = kp * (float) error + kd * (float) derivative;
  if (currLDist < currRDist) {
    output = output / currLDist;
  } else {
    output = output / currRDist;
  }

  int leftSpeed = baseSpeed + output;
  int rightSpeed = baseSpeed - output;

  if (leftSpeed > 100) {
    leftSpeed = 90;
  } else if (leftSpeed < 40) {
    leftSpeed = 60;
  } 
  if (rightSpeed > 100) {
    rightSpeed = 90;
  } else if (rightSpeed < 40) {
    rightSpeed = 60; 
  }

  leftMotor.rotateCCW(leftSpeed);
  rightMotor.rotateCW(rightSpeed); 
  Serial.print("KP:  ");
  Serial.print(kp);
  Serial.print("   ");
  Serial.print("KD:  ");
  Serial.print(kd);
  Serial.print("   ");
   Serial.print("LDist:  ");
  Serial.print(currLDist);
  Serial.print("   ");
   Serial.print("RDist:  ");
  Serial.print(currRDist);
  Serial.print("   ");
  Serial.print("leftSpeed:  ");
  Serial.print(leftSpeed);
  Serial.print("   ");
  Serial.print("rightSpeed:  ");
  Serial.print(rightSpeed);
  Serial.println("   ");
	
}

void Car::reverse(Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified   mag) {
	//leftMotor.rotateCW(speed);
	//rightMotor.rotateCCW(speed);
}

void Car::turnLeft(float angle, Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag) {
  float initHeading;
  float currHeading;
  float targetHeading;
  float error;
  float tolerance;
  int turnSpeed;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
    initHeading = orientation.heading;
    Serial.print("INIT: ");
    Serial.println(initHeading);
  }

  if (angle > initHeading) {
    targetHeading = 360.0 - (angle - initHeading);
  } else {
    targetHeading = initHeading - angle;
  }
  tolerance = 3.0;
  turnSpeed = 65;
  leftMotor.rotateCCW(turnSpeed);
  rightMotor.rotateCCW(turnSpeed);
      Serial.print("TARGET: ");
    Serial.println(targetHeading);
  error = targetHeading - currHeading;
  while(abs(error) > tolerance) {
    mag.getEvent(&mag_event);
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
      currHeading = orientation.heading;
    }
    
    error = targetHeading - currHeading; 
    //Serial.println(error);
  }
  Serial.println("DONE");
  leftMotor.stop();
  rightMotor.stop();
}

void Car::turnRight(float angle, Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag) {
  float initHeading;
  float heading;
  float currHeading;
  float targetHeading;
  float error;
  float tolerance;
  int turnSpeed;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  int numOfSamples = 20;
  
  int i = 0;
  initHeading = 0;
  while (i < numOfSamples) {
     mag.getEvent(&mag_event);
     if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
         initHeading = initHeading + orientation.heading;
     }
     i++;
  }
  initHeading = initHeading / numOfSamples;

  if (angle > (360 - initHeading)) {
    targetHeading = angle - (360 - initHeading);
  } else {
    targetHeading = initHeading + angle;
  }
  tolerance = 2.0;
  turnSpeed = 55;
  leftMotor.rotateCW(turnSpeed);
  rightMotor.rotateCW(turnSpeed);
  currHeading = 0;
  error = targetHeading - currHeading;
  while(abs(error) > tolerance) {
    int j = 0;
    while (j < 10) {
      mag.getEvent(&mag_event);
      if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
        currHeading = currHeading + orientation.heading;
      }
    }

    currHeading = currHeading / 10;
    
    error = targetHeading - currHeading; 
  }
  Serial.println("DONE");
  leftMotor.stop();
  rightMotor.stop();
}

void Car::calibrate(Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag) {
  //FIND MOTOR SPEED TO USE
  boolean done = false; 
  while(!done) {
    
  }
  
}

void Car::brake() {
	leftMotor.stop();
	rightMotor.stop();
}
