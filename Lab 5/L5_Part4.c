/*
 * CFile1.c
 *
 * Created: 5/6/2017 7:47:49 PM
 *  Author: Shreetej
 */ 
#define BAUD 9600
#define F_CPU 16000000
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
volatile unsigned int pulseWidth;
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
	DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4);
	cli();
	uart_init();
	DDRB  |= (1 << PB1);      //set PB1(Pin9) is output
	DDRB  &= ~(1 << PB0);     //set PB0(Pin8) as input
	sei();
	init_timer1();
	init_ic();
		
	while(1) {
		if (sampleCollected == 1) {
			//printf("Pulse width: %u\n", diff);
			pulseWidth = diff;
			//reset variables
			start = 0;
			end = 0;
			diff = 0;
			sampleCollected = 0;
		}
		if (pulseWidth >= 0 && pulseWidth < 100) {
			//0
			printf("NUM: %u\n", 0);
			PORTB &= ~(1 << PB2);
			PORTB &= ~(1 << PB3);
			PORTB &= ~(1 << PB4);
		} else if (pulseWidth >= 100 && pulseWidth < 200) {
			//1
			printf("NUM: %u\n", 1);
			PORTB |= (1 << PB2);
			PORTB &= ~(1 << PB3);
			PORTB &= ~(1 << PB4);
		} else if (pulseWidth >= 200 && pulseWidth < 300) {
			//2
			printf("NUM: %u\n", 2);
			PORTB &= ~(1 << PB2);
			PORTB |= (1 << PB3);
			PORTB &= ~(1 << PB4);
		} else if (pulseWidth >= 300 && pulseWidth < 400) {
			//3
			printf("NUM: %u\n", 3);
			PORTB |= (1 << PB2);
			PORTB |= (1 << PB3);
			PORTB &= ~(1 << PB4);
		} else if (pulseWidth >= 400 && pulseWidth < 500) {
			//4
			printf("NUM: %u\n", 4);
			PORTB &= ~(1 << PB2);
			PORTB &= ~(1 << PB3);
			PORTB |= (1 << PB4);
		} else if (pulseWidth >= 500 && pulseWidth < 600) {
			//5
			printf("NUM: %u\n", 5);
			PORTB |= (1 << PB2);
			PORTB &= ~(1 << PB3);
			PORTB |= (1 << PB4);
		} else if (pulseWidth >= 600 && pulseWidth < 700) {
			//6
			printf("NUM: %u\n", 6);
			PORTB &= ~(1 << PB2);
			PORTB |= (1 << PB3);
			PORTB |= (1 << PB4);
		} else if (pulseWidth >= 700 && pulseWidth < 800) {
			//7
			printf("NUM: %u\n", 7);
			PORTB |= (1 << PB2);
			PORTB |= (1 << PB3);
			PORTB |= (1 << PB4);
		}
	}
	
}