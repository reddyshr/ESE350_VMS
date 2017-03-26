/*
 * Motor.c
 *
 * Created: 3/25/2017 6:36:56 PM
 *  Author: Shreetej
 *
 * Purpose: Control the speed and direction of a DC motor.
 *
 * Important Notes: This class is designed to control the motor speed and direction using the SN754410 
 *                  h-bridge. As a result, for each motor, two PWM pins on the Atmega328P need to be allocated
 *					to the h-bridge to allow for bi-directional rotation. Based on the current code, you need 
 *					to use either arduino pins 11 and 3 or pins 6 and 5. This corresponds to the output compare units of 
 *					timers 0 and 2. Speed can range from 0 to 100 inclusive.
 *
 *
 *
 */ 

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

void init_timer0() {
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1); //set on BOTTOM, clear on compare match
	TCCR0A |= (1 << WGM01) | (1 << WGM00); //mode 3 pwm, top is max
	TCCR0B |= (1 << CS00); //no prescaler
}

void init_timer2() {
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1); //set on BOTTOM, clear on compare match
	TCCR2A |= (1 << WGM21) | (1 << WGM20); //mode 3 pwm, top is max
	TCCR2B |= (1 << CS20); //no prescaler
}

Motor::Motor(int clockwisePin, int counterclockwisePin) {	
	cwPin = clockwisePin;
	ccwPin = counterclockwisePin;
	if ((cwPin == 5 && ccwPin == 6) || (cwPin == 6 && ccwPin == 5)) {
		init_timer0();
	} else if ((cwPin == 11 && ccwPin == 3) || (cwPin == 3 && ccwPin == 11)) {
		init_timer2();
	}
}

void Motor::rotateCW(double speed) {
	int ocVal;
	if (speed > 100) {
		ocVal = 254;
	} else if (speed < 0) {
		ocVal = 0;
	} else {
		ocVal = (int) ((speed / 100.0) * 254.0);
	}
	
	if (cwPin == 5) {	
		DDRD &= ~(x06); //disable OC0A output
		OCR0A = 0;
		OCR0B = ocVal;
		DDRD |= x05; //enable OC0B output		
	} else if (cwPin == 6) {
		DDRD &= ~(x05); //disable OC0B output
		OCR0B = 0;
		OCR0A = ocVal;
		DDRD |= x06; //enable OC0A output
	} else if (cwPin == 11) {
		DDRD &= ~(x03); //disable OC2B output
		OCR2B = 0;
		OCR2A = ocVal;
		DDRB |= x03;		
	} else if (cwPin == 3) {
		DDRB &= ~(x03); //disable OC2B output
		OCR2A = 0;
		OCR2B = ocVal;
		DDRD |= x03;
	}
}

void Motor::rotateCCW(double speed) {
	int ocVal;
	if (speed > 100) {
		ocVal = 254;
		} else if (speed < 0) {
		ocVal = 0;
		} else {
		ocVal = (int) ((speed / 100.0) * 254.0);
	}
	
	if (ccwPin == 5) {
		DDRD &= ~(x06); //disable OC0A output
		OCR0A = 0;
		OCR0B = ocVal;
		DDRD |= x05; //enable OC0B output
	} else if (ccwPin == 6) {
		DDRD &= ~(x05); //disable OC0B output
		OCR0B = 0;
		OCR0A = ocVal;
		DDRD |= x06; //enable OC0A output
	} else if (ccwPin == 11) {
		DDRD &= ~(x03); //disable OC2B output
		OCR2B = 0;
		OCR2A = ocVal;
		DDRB |= x03;
	} else if (ccwPin == 3) {
		DDRB &= ~(x03); //disable OC2B output
		OCR2A = 0;
		OCR2B = ocVal;
		DDRD |= x03;
	}
}

void Motor::stop() {
	if ((cwPin == 5 && ccwPin == 6) || (cwPin == 6 && ccwPin == 5)) {
		DDRD &= ~(x05);
		DDRD &= ~(x06);
	} else if ((cwPin == 11 && ccwPin == 3) || (cwPin == 3 && ccwPin == 11)) {
		DDRB &= ~(x03);
		DDRD &= ~(x03);
	}
}