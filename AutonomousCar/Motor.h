/*
 * Motor.h
 *
 * Created: 3/25/2017 11:13:38 PM
 *  Author: Shreetej
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_
#include <avr/io.h>

class Motor {
	public:
	void rotateCW(double speed);
	void rotateCCW(double speed);
	void stop();
	Motor(int clockwisePin, int counterclockwisePin);
	private:
	int cwPin;
	int ccwPin;
	
};

#endif /* MOTOR_H_ */