#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

volatile int delay;
//sets timer to CTC mode
void enable_timer() {
	//set timer to CTC mode, top OCRA
	TCCR0A &= ~(1 << WGM00);  
	TCCR0A |= (1 << WGM01);
	TCCR0A |= (1 << COM0A0);  //toggle OC0A on compare match, PIN6
	TIMSK0 |= (1 << 1);   //enable output compare interrupt
	OCR0A = delay;
}

//temp function to test if we can output the correct frequencies
void decode(int freq) {
	if (freq == 1000) {
		delay = 125;
		enable_timer();
	}
	else if (freq == 1122) {
		delay = 111;
		enable_timer();
	}
	else if (freq == 1260) {
		delay = 99;
		enable_timer(); 
	}
	else if (freq == 1335) {
		delay = 94;
		enable_timer();
	}
	else if (freq == 1498) {
		delay = 83;
		enable_timer();
	}
	else if (freq == 1682) {
		delay = 74;
		enable_timer();
	}
	else if (freq == 1888) {
		delay = 66;
		enable_timer();
	}
	else if (freq == 2000) {
		delay = 63;
		enable_timer();
	}
	else {
		printf("could not find freq\n");
	}
}

ISR(TIMER0_COMPA_vect) {
	OCR0A = delay;
}

int main() {
	//start timer, 64 prescaler
	TCCR0B |= (1 << CS01) | (1 << CS00); 
	DDRD |= (1 << PD6);
	uart_init(); 
	while(1) {
		char input[100];
		scanf("%s", &input);
		if (input[0] != '\0') {
			//user has put in input
			int freq = atoi(input);
			if (freq == 0) {
				printf("invalid frequency\n");
				continue;
			}
			printf("frequency is: %u\n", freq);
			decode(freq);
			printf("Enter frequency: \n");
		}
	}
	return 1;
}
