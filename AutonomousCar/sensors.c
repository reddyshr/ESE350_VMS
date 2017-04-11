#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

const int triggerPulseHi = 2;
const int triggerPulseLo = 14999;

//BOOLEANS TO DETERMINE WHEN SAMPLES ARE READY
volatile int sensor1_read;
volatile int sensor2_read;
volatile int sensor3_read;
volatile int sensor4_read;
volatile int sensor5_read;

//RISING EDGE TIMES
volatile int sensor1_start;
volatile int sensor2_start;
volatile int sensor3_start;
volatile int sensor4_start;
volatile int sensor5_start;

//FALLING EDGE TIMES
volatile int sensor1_end;
volatile int sensor2_end;
volatile int sensor3_end;
volatile int sensor4_end;
volatile int sensor5_end;

//PULSE WIDTHS
volatile int sensor1_pulse;
volatile int sensor2_pulse;
volatile int sensor3_pulse;
volatile int sensor4_pulse;
volatile int sensor5_pulse;

//DELAY FOR TRIGGER PULSE
volatile int delay;

//PD7(Pin7) - SENSOR #
//PD6(Pin6) - SENSOR #
//PD5(Pin5) - SENSOR #
//PD4(Pin4) - SENSOR #
//PD3(Pin3) - SENSOR #
//PB1(Pin9) - TRIGGER PULSE

void init_variables() {
	sensor1_read = 0;
	sensor2_read = 0;
	sensor3_read = 0;
	sensor4_read = 0;
	sensor5_read = 0;
	sensor1_start = 0;
  sensor2_start = 0;
  sensor3_start = 0;
  sensor4_start = 0;
  sensor5_start = 0;
  sensor1_end = 0;
  sensor2_end = 0;
  sensor3_end = 0;
  sensor4_end = 0;
  sensor5_end = 0;
  sensor1_pulse = 0;
  sensor2_pulse = 0;
  sensor3_pulse = 0;
  sensor4_pulse = 0;
  sensor5_pulse = 0;
}


ISR(TIMER1_COMPA_vect) {
	if (delay == triggerPulseHi) {
		delay = triggerPulseLo;
		OCR1A += delay;
	}
	else if (delay == triggerPulseLo) {
		delay = triggerPulseHi;
		OCR1A += delay;
	}
	else {
		printf("error!\n");
	}
}

void init_timer1() {
	TCCR1B |= (1 << CS11) | (1 << CS10);    //start timer, 64 prescaler
	TCCR1A |= (1 << COM1A0);                //toggle OC1A(Pin9) on compare match
	TIMSK1 |= (1 << OCIE1A);                //enable OCA interrupt
	delay = triggerPulseHi;                 //start with high pulse
	OCR1A = TCNT1 + delay;                  //2 cycles = 10us needed
}

int main() {
	cli();
	uart_init();
	DDRB  |= (1 << PB1);    //set PB1(Pin9) as output
	DDRD  &= ~(1 << PD7);   //set PD7(Pin7) as input
	DDRD  &= ~(1 << PD6);   //set PD6(Pin6) as input
	DDRD  &= ~(1 << PD5);   //set PD5(Pin5) as input
	DDRD  &= ~(1 << PD4);   //set PD4(Pin4) as input
	DDRD  &= ~(1 << PD3);   //set PD3(Pin3) as input
	PORTB = (1 << PORTB1);  //set PB1(Pin9) high
	sei();
	init_variables();
	init_timer1();

  while(1) {
  	//check for input from sensors continously
  	
  }

	return 1;
}
