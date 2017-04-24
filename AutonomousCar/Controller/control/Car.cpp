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
void Car::forward(int currLDist, int currRDist, int oldLDist, int oldRDist, float dt) {
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

  int leftSpeed = baseSpeed - output;
  int rightSpeed = baseSpeed + output;

  if (leftSpeed > 100) {
    leftSpeed = 97;
  } else if (leftSpeed < 40) {
    leftSpeed = 55;
  } 
  if (rightSpeed > 100) {
    rightSpeed = 97;
  } else if (rightSpeed < 40) {
    rightSpeed = 55; 
  }

  leftMotor.rotateCCW(leftSpeed);
  rightMotor.rotateCCW(rightSpeed); 
  /*Serial.print("KP:  ");
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
  Serial.println("   ");*/
	
}

void Car::reverse() {
	//leftMotor.rotateCW(speed);
	//rightMotor.rotateCCW(speed);
}

void Car::turnLeft(float angle) {
  float prop = angle / 360.0;
  int num = (int) (prop * 55); //NEED TO CALCULATE NUM
  int cntLeft = 0;
  int prevSampleLeft;
  int currSampleLeft;
  int cntRight = 0;
  int prevSampleRight;
  int currSampleRight;
  int turnSpeed = 75;
  leftMotor.rotateCW(turnSpeed);
  rightMotor.rotateCCW(turnSpeed);


    DDRD &= ~(1 << PD7);
    DDRB &= ~(1 << PB0);
    while ((cntRight < num)) {
      if(PIND & (1 << PD7)) {
        currSampleLeft = 1;
      } else {
        currSampleLeft = 0;
      }
      if(PINB & (1 << PB0)) {
        currSampleRight = 1;
      } else {
        currSampleRight = 0;
      }
      
      if (currSampleLeft == 1 && prevSampleLeft == 0) {
        cntLeft++;
      }
      if (currSampleRight == 1 && prevSampleRight == 0) {
        cntRight++;
      }
      
      prevSampleLeft = currSampleLeft;
      prevSampleRight = currSampleRight;

      if (cntLeft >= num) {
        leftMotor.stop();
      }
      if (cntRight >= num) {
        leftMotor.stop();
        rightMotor.stop();
      }
    }
 
  leftMotor.stop();
  rightMotor.stop();
}

void Car::turnRight(float angle) {
  float prop = angle / 360.0;
  int num = (int) (prop * 56);
  int cntLeft = 0;
  int prevSampleLeft;
  int currSampleLeft;
  int cntRight = 0;
  int prevSampleRight;
  int currSampleRight;
  int turnSpeed = 75;
  leftMotor.rotateCCW(turnSpeed);
  rightMotor.rotateCW(turnSpeed);


    DDRD &= ~(1 << PD7);
    DDRB &= ~(1 << PB0);
    while ((cntLeft < num) ) {
      if(PIND & (1 << PD7)) {
        currSampleLeft = 1;
      } else {
        currSampleLeft = 0;
      }
      if(PINB & (1 << PB0)) {
        currSampleRight = 1;
      } else {
        currSampleRight = 0;
      }
      
      if (currSampleLeft == 1 && prevSampleLeft == 0) {
        cntLeft++;
      }
      if (currSampleRight == 1 && prevSampleRight == 0) {
        cntRight++;
      }
      
      prevSampleLeft = currSampleLeft;
      prevSampleRight = currSampleRight;

      if (cntLeft >= num) {
        leftMotor.stop();
        rightMotor.stop();
      }
      if (cntRight >= num) {
        rightMotor.stop();
      }
    }
 
  leftMotor.stop();
  rightMotor.stop();
}

void Car::brake() {
	leftMotor.stop();
	rightMotor.stop();
}
