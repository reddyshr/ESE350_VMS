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
void Car::forward(Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified   mag) {
  float initHeading;
  float currHeading;
  cont = true;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
    initHeading = orientation.heading;
    Serial.print("INIT: ");
    Serial.println(initHeading);
  }

  while (cont) {
    //AN INTERRUPT SHOULD BE CALLED THAT MAKES CONT FALSE
    mag.getEvent(&mag_event);
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
      currHeading = orientation.heading;
      Serial.print("Curr: ");
      Serial.println(currHeading);
    }
    leftMotor.rotateCCW(40);
    rightMotor.rotateCW(40);
  }
	
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
  tolerance = 2.0;
  turnSpeed = 60;
  leftMotor.rotateCCW(turnSpeed);
  rightMotor.rotateCCW(turnSpeed);
  
  while(abs(error) > tolerance) {
    mag.getEvent(&mag_event);
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
      currHeading = orientation.heading;
    }
    
    error = targetHeading - currHeading; 
    Serial.println(error);
  }
  Serial.println("DONE");
  leftMotor.stop();
  rightMotor.stop();
}

void Car::turnRight(float angle, Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag) {
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

  if (angle > (360 - initHeading)) {
    targetHeading = angle - (360 - initHeading);
  } else {
    targetHeading = initHeading + angle;
  }
  tolerance = 2.0;
  turnSpeed = 60;
  leftMotor.rotateCW(turnSpeed);
  rightMotor.rotateCW(turnSpeed);
  
  while(abs(error) > tolerance) {
    mag.getEvent(&mag_event);
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
      currHeading = orientation.heading;
    }
    
    error = targetHeading - currHeading; 
    Serial.println(error);
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