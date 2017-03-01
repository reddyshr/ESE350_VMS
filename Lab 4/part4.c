#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

const int TIMER = 62500; //prescaler is 256

volatile int row_to_check;
volatile int row_delay;
volatile int col_delay;

/*
PC2 - pin8/row4
PC3 - pin1/row3
PC4 - pin2/row2
PC5 - pin4/row1

PD2 - pin3/col1
PD3 - pin5/col2
PD4 - pin6/col3
PD5 - pin7/col4
*/

void decode() {
	if (row_to_check == 4) {
		PORTC &= ~(1 << PC2);
		PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, number 1, PB1 = 697Hz, PB2 = 1209Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1209) / 2;
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 2, PB1 = 697Hz, PB2 = 1336Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1336) / 2;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, number 3, PB1 = 697Hz, PB2 = 1477Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1477) / 2;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter A, PB1 = 697Hz, PB2 = 1633Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1633) / 2;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
		}
		else {
		}
		row_to_check = 3;
	}
	else if (row_to_check == 3) {
		PORTC &= ~(1 << PC3);
		PORTC |= (1 << PC2) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, number 4, PB1 = 770Hz, PB2 = 1209Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1209) / 2;
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 5, PB1 = 770Hz, PB2 = 1336Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1336) / 2;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, number 6, PB1 = 770Hz, PB2 = 1477Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1477) / 2;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter B, PB1 = 770Hz, PB2 = 1633Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1633) / 2;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
		}
		else {
		}
		row_to_check = 2;
	}
	else if (row_to_check == 2) {
		PORTC &= ~(1 << PC4);
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, number 7, PB1 = 852Hz, PB2 = 1209Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1209) / 2;
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 8, PB1 = 852Hz, PB2 = 1336Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1336) / 2;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, number 9, PB1 = 852Hz, PB2 = 1477Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1477) / 2;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter C, PB1 = 852Hz, PB2 = 1633Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1633) / 2;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
		}
		else {
		}
		row_to_check = 1;
	}
	else {
		PORTC &= ~(1 << PC5);
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC4);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, letter *, PB1 = 941Hz, PB2 = 1209Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1209) / 2;
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 0, PB1 = 941Hz, PB2 = 1336Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1336) / 2;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, letter #, PB1 = 941Hz, PB2 = 1477Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1477) / 2;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter D, PB1 = 941Hz, PB2 = 1633Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1633) / 2;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
		}
		else {
		}
		row_to_check = 4;
	}
}

//overflows every 4ms
ISR(TIMER2_OVF_vect) {
	decode();
}

//set row frequency
ISR(TIMER2_COMPA_vect) {
	OCR1A += row_delay;
}

//set col frequency
ISR(TIMER2_COMPB_vect) {
	OCR1B += col_delay;
}

void toggle_on_tone() {
	//enable TIMER1 with prescaler 256
	TCCR1B |= (1 << CS12); 
	//toggle compare match for OC1A (pin9) and OC1B (pin10)
	TCCR1A |= (1 << 4) | (1 << 6);
	//enable OCA, OCB, timer2 overflow interrupts
	TIMSK1 |= (1 << TOIE) | (1 << OCIEA) | (1 << OCIEB);	

	OCR1A = TCNT1 + row_delay;
	OCR1B = TCNT1 + col_delay;
}

void toggle_off_tone() {
	TCCR1B = 0;                //turn clock off
	TCCR1A &= ~(1 << 4);       //toggle off compare match OC1A
	TCCR1A &= ~(1 << 6);       //toggle off compare match OC1B
	TIMSK1 &= ~(1 << OCIEA);   //disable OCA interrupt
	TIMSK1 &= ~(1 << OCIEB);   //disable OCB interrupt
}

int wave_1633Hz() {
	return ((TIMER / 1633) / 2);
}

int main() {
	uart_init();
	//set rows as output
	DDRC |= (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);
	//set columns as input
	DDRD = 0;
	row_to_check = 4;
	sei();

	while(1);
}