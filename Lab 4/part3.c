#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

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

void decode() {
	if (row_to_check == 4) {
		PORTC &= ~(1 << PC2);
		PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1
			putchar('1');
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			putchar('2');
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			putchar('3');
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			putchar('A');
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
			putchar('4');
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			putchar('5');
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			putchar('6');
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			putchar('B');
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
			putchar('7');
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			putchar('8');
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			putchar('9');
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			putchar('C');
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
			putchar('*');
			while(((PIND & (1 << PD2)) >> PD2) == 0);
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2
			putchar('0');
			while(((PIND & (1 << PD3)) >> PD3) == 0);
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3
			putchar('#');
			while(((PIND & (1 << PD4)) >> PD4) == 0);
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4
			putchar('D');
			while(((PIND & (1 << PD5)) >> PD5) == 0);
		}
		else {
		}
		row_to_check = 4;
	}
}

ISR(TIMER2_COMPA_vect) {
	decode();
}

int main() {
	uart_init();
	//set rows as output
	DDRC |= (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);
	//set columns as input
	DDRD = 0;
	//Sets to CTC mode (clears TCNT2 when OCR2A == TCNT2)
	TCCR2A |= (1 << WGM21);
	//enable TIMER2 with prescaler = 256 
	TCCR2B |= (1 << 2) | (1 << 1); 
	OCR2A = 255;
	//Enable Output Compare A Match Interrupt
	TIMSK2 |= 0x02;	
	
	sei();

	row_to_check = 4;
	TCNT2 = 0;

	while(1);
}