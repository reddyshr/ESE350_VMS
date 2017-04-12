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
	void forward(Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag);
	void reverse(Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag);
  void turnLeft(float angle, Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag);
  void turnRight(float angle, Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag);
  void calibrate(Adafruit_9DOF dof, Adafruit_LSM303_Mag_Unified mag);
	void brake();
	Car(int leftCWPin, int leftCCWPin, int rightCWPin, int rightCCWPin) : leftMotor(leftCWPin, leftCCWPin), rightMotor(rightCWPin, rightCCWPin){}
	private:
	Motor leftMotor;
	Motor rightMotor;
};





#endif /* CAR_H_ */