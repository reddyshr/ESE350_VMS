/*
 * Car.h
 *
 * Created: 3/28/2017 10:49:33 PM
 *  Author: Shreetej
 */ 


#ifndef CAR_H_
#define CAR_H_
#include <avr/io.h>
#include "Motor.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>

class Motor;
class Car {
	public:
  bool cont;
  int goSpeed;
	void forward(int currLDist, int currRDist, int oldLDist, int oldRDist, float dt);
	void reverse();
  void turnLeft(float angle);
  void turnRight(float angle);
	void brake();
	Car(int leftCWPin, int leftCCWPin, int rightCWPin, int rightCCWPin) : leftMotor(leftCWPin, leftCCWPin), rightMotor(rightCWPin, rightCCWPin){}
	private:
	Motor leftMotor;
	Motor rightMotor;
  int leftRotPin = 7;
  int rightRotPin = 8;
};





#endif /* CAR_H_ */
