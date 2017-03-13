#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

volatile unsigned int start;
volatile unsigned int end; 
volatile unsigned int diff;
volatile int sampleCollected;



void init_oc_timer1() {
	TIMSK1  = 0;               //disable all interrupts
	TCCR1A |= (1 << COM1A1);   //clear PB1/OC1A on output compare
	TIMSK1 |= (1 << OCIE1A);   //enable OCA interrupt
	TCCR1B |= (1 << CS10);     //no prescaler/start timer
	OCR1A = TCNT1 + 79;        //79 cycles = 5us, trigger in 5 us
}

void init_ic_timer1() {
	TIMSK1  = 0;               //disable output compare interrupt
	TIMSK1 |= (1 << ICIE1);    //enable input capt interrupt
	TCCR1B |= (1 << ICES1);    //trigger is rising edge	
	TIFR1  |= 0x20;            //clear the input capture flag
}

ISR(TIMER1_CAPT_vect) {
	//trigger is rising edge
	if (((TCCR1B & (1 << ICES1)) >> ICES1) == 1) {
		start   = ICR1;           //rising edge input capture value
		TCCR1B &= ~(1 << ICES1)   //set trigger to falling edge
		TIFR1  |= 0x20;           //clear the input capture flag
	}
  //trigger is falling edge
	else {
		end = ICR1;               //falling edge input capture value       
		diff = end - start;       //calculate pulse width  
		sampleCollected = 1;
	}
}

ISR(TIMER1_COMPA_vect) {
	DDRB &= ~(1 << PB1);       //Set PB1 to input
	init_ic_timer1();          //set timer1 for input capture
	TCNT1 = 0;                 //reset timer count
}


int main(void) {
	DDRB  |= (1 << PB1);      //PB1 is output
	PORTB |= (1 << PORTB1);   //Set PB1 to high
	sampleCollected = 0;
	init_oc_timer1();
	sei();
	
  while(1) {
		if (sampleCollected) {
			printf("Pulse width: %u\n", diff);
			TIMSK1  = 0;             //disable interrupts
			DDRB   |= (1 << PB1);    //PB1 is output
			PORTB  |= (1 << PORTB1); //Set PB1 to high
			TCNT1   = 0;             //clear timer count
			init_oc_timer1();        //reset so process repeats
			sampleCollected = 0;
		}
  }
}