#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

const int triggerPulseHi = 2;
const int triggerPulseLo = 14999;

volatile int delay;
volatile unsigned int start;
volatile unsigned int end; 
volatile unsigned int diff;
volatile int sampleCollected;



void init_timer1() {
	//for trigger pin
	TCCR1B |= (1 << CS10) | (1 << CS11);     //start timer, 64 prescaler
	TCCR1A |= (1 << COM1A0);   //toggle OC1A(Pin9) compare match
	TIMSK1 |= (1 << OCIE1A);   //enable OCA interrupt
	delay = triggerPulseHi;
	OCR1A = TCNT1 + delay;    //2 cycles = 10us
	PORTB |= (1 << PORTB1);   //Set PB1 to high
	start = 0;                //initialize variables
	end = 0;
	diff = 0;
	sampleCollected = 0;
}

void init_ic() {
	//for echo pin
	TIMSK1 |= (1 << ICIE1);    //enable input capture
	TCCR1B |= (1 << ICES1);    //rising edge
	TIFR1  |= 0x20;            //clear input capture flag
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
		printf("error\n");
	}
}

ISR(TIMER1_CAPT_vect) {
	if (((TCCR1B & (1 << ICES1)) >> ICES1) == 1) {
		//triggered on rising edge, set to falling edge
		start = TCNT1;
		TCCR1B &= ~(1 << ICES1); 
		TIFR1  |= 0x20; 
	}
	else if (((TCCR1B & (1 << ICES1)) >> ICES1) == 0) {
		//triggered on falling edge, find pulse width
		end = TCNT1;
		diff = end - start;
		sampleCollected = 1;
		TCCR1B |= (1 << ICES1); 
		TIFR1  |= 0x20; 
	}
	else {
		printf("error in input capture!\n");
	}
}


int main(void) {
	cli();
	uart_init();
	DDRB  |= (1 << PB1);      //set PB1(Pin9) is output
	DDRB  &= ~(1 << PB0);     //set PB0(Pin8) as input
	sei();
	init_timer1();
	init_ic();
	
  while(1) {
		if (sampleCollected == 1) {
			printf("Pulse width: %u\n", diff);
			//reset variables
			start = 0;                
			end = 0;
			diff = 0;
			sampleCollected = 0;
		}
  }
}