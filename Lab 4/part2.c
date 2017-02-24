#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/delay.h"
#include "uart.h"

const uint32_t TIMER = 2000000; 

volatile int delay;
volatile int tot_overflows;

//interrupt triggered when OCR1A == TNCT1
//interrupt toggles OC1A every time

ISR(TIMER1_OVF_vect) {
	tot_overflows++;
}

ISR(TIMER1_COMPA_vect) {
	//interrupt toggles OC1A in 'delay' cycles
	OCR1A += delay;    
}

void toggle_on_compare() {
	TCCR1B |= 0x02;          //start TIMER1, 8 prescaler
	TCCR1A |= 0x40;          //toggle on compare match for OC1A (pin9)
	TIMSK1 |= 0x02 | 0x01;   //output compare A interrupt enabled 
	                         //and TIMER1 overflow enabled
	OCR1A = TCNT1 + delay;   //interrupt toggles in 'delay' cycles
	TCNT1 = 0;               //restart timer counter
	tot_overflows = 0;       //initialize overflow counter
}

void toggle_off_compare() {
	TCCR1A &= ~(0x40);       //toggle off compare match for OC1A (pin9)
	OCR1A   = 0;             //clear output compare register
	TIMSK1 &= ~(0x02);       //output compare A interrupt disabled
	TCCR1B = 0;              //turn the clock off
}

int main() {
	uart_init(); 
	cli();
	char freq[200];
	char dur[200];
	DDRB |= (1 << PB1);      //set pin9 as the output
	sei();

	printf("Enter frequency and duration:\r\n");

	while(1) {
		scanf("%s %s", &freq, &dur);
		if (freq[0] != '\0' && dur[0] != '\0') {

			int int_freq = atoi(freq);
			int int_dur = atoi(dur);

			printf("frequency is: %u\r\n", int_freq);
			printf("duration is: %u\r\n", int_dur);

			if (int_freq == 0 || int_dur == 0 || int_freq <= 0 || int_dur <= 0) {
				printf("Invalid frequency or duration. Please try again.\r\n");
				printf("Enter frequency and duration:\r\n");
			}

			else {
				//calculate delay and duration
				delay = (TIMER / int_freq) / 2;
				int max_overflows = (int) ((TIMER / 65536) * (int_dur / 1000.0));
				int extra_cycles = (int) (TIMER * (int_dur / 1000.0)) % 65536;
				toggle_on_compare();
				//wait for length of duration
				while (tot_overflows <= max_overflows);
				while (TCNT1 <= extra_cycles);
				toggle_off_compare();
				freq[0] = '\0';
				dur[0] = '\0';
				//restart
				printf("Enter frequency and duration:\r\n");
			}

		}
	
	}
}