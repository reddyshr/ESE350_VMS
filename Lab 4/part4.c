#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

const int TIMER = 62500;
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

volatile int row_to_check;
volatile int row_delay;
volatile int col_delay;

void decode() {
	if (row_to_check == 4) {
		PORTC &= ~(1 << PC2);
		PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1
			//putchar('1');
			//PB1 = 697Hz, PB2 = 1209Hz
			row_delay = wave_697Hz();
			col_delay = wave_1209Hz();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			//putchar('2');
			//PB1 = 697Hz, PB2 = 1336Hz
			row_delay = wave_697Hz();
			col_delay = wave_1336Hz();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			//putchar('3');
			//PB1 = 697Hz, PB2 = 1477Hz
			row_delay = wave_697Hz();
			col_delay = wave_1477Hz();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			//putchar('A');
			//PB1 = 697Hz, PB2 = 1633Hz
			row_delay = wave_697Hz();
			col_delay = wave_1633Hz();
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
			//column 1;
			//putchar('4');
			//PB1 = 770Hz, PB2 = 1209Hz
			row_delay = wave_770Hz();
			col_delay = wave_1209Hz();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			//putchar('5');
			//PB1 = 770Hz, PB2 = 1336Hz
			row_delay = wave_770Hz();
			col_delay = wave_1336Hz();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			//putchar('6');
			//PB1 = 770Hz, PB2 = 1477Hz
			row_delay = wave_770Hz();
			col_delay = wave_1477Hz();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			//putchar('B');
			//PB1 = 770Hz, PB2 = 1633Hz
			row_delay = wave_770Hz();
			col_delay = wave_1633hz();
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
			//column 1;
			//putchar('7');
			//PB1 = 852Hz, PB2 = 1209Hz
			row_delay = wave_852Hz();
			col_delay = wave_1209Hz();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			//putchar('8');
			//PB1 = 852Hz, PB2 = 1336Hz
			row_delay = wave_852Hz();
			col_delay = wave_1336Hz();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			//putchar('9');
			//PB1 = 852Hz, PB2 = 1477Hz
			row_delay = wave_852Hz();
			col_delay = wave_1477Hz();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			//putchar('C');
			//PB1 = 852Hz, PB2 = 1633Hz
			row_delay = wave_852Hz();
			col_delay = wave_1633Hz();
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
			//column 1;
			//putchar('*');
			//PB1 = 941Hz, PB2 = 1209Hz
			row_delay = wave_941Hz();
			col_delay = wave_1209Hz();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			//putchar('0');
			//PB1 = 941Hz, PB2 = 1336Hz
			row_delay = wave_941Hz();
			col_delay = wave_1336Hz();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			//putchar('#');
			//PB1 = 941Hz, PB2 = 1477Hz
			row_delay = wave_941Hz();
			col_delay = wave_1477Hz();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			//putchar('D');
			//PB1 = 941Hz, PB2 = 1633Hz
			row_delay = wave_941Hz();
			col_delay = wave_1633Hz();
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

int wave_697Hz() {
	return ((TIMER / 697) / 2);
}

int wave_770Hz() {
	return ((TIMER / 770) / 2);
}

int wave_852Hz() {
	return ((TIMER / 852) / 2);
}

int wave_941Hz() {
	return ((TIMER / 941) / 2);
}

int wave_1209Hz() {
	return ((TIMER / 1209) / 2);
}

int wave_1336Hz() {
	return ((TIMER / 1336) / 2);
}

int wave_1477Hz() {
	return ((TIMER / 1477) / 2);
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
	
	sei();

	row_to_check = 4;

	while(1);
}