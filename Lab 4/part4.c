#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

const uint32_t TIMER = 2000000; //prescaler is 8

volatile int row_to_check;
volatile unsigned int row_delay;
volatile unsigned int col_delay;

void toggle_on_tone() {
	//toggle compare match for OC1A (pin9) and OC1B (pin10)
	TCCR1A |= (1 << COM1A0) | (1 << COM1B0);
	//enable TIMER1 with prescaler 8
	TCCR1B |= (1 << CS11); 
	//enable OCA, OCB interrupts
	TIMSK1 |= (1 << 2) | (1 << 1);	
	TCNT1 = 0;
	OCR1A = TCNT1 + row_delay;
	OCR1B = TCNT1 + col_delay;	
}

void toggle_off_tone() {
	TCCR1B = 0;            //turn clock off
	TCCR1A &= ~(1 << COM1A0);   //toggle off compare match OC1A
	TCCR1A &= ~(1 << COM1B0);   //toggle off compare match OC1B
	TIMSK1 &= ~(1 << 1);   //disable OCA interrupt
	TIMSK1 &= ~(1 << 2);   //disable OCB interrupt
	OCR1A = 0;
	OCR1B = 0;
}

void decode() {
if (row_to_check == 4) {
		PORTC &= ~(1 << PC2);
		PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, number 1, PB1 = 697Hz, PB2 = 1209Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1209) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 2, PB1 = 697Hz, PB2 = 1336Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1336) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, number 3, PB1 = 697Hz, PB2 = 1477Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1477) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter A, PB1 = 697Hz, PB2 = 1633Hz
			row_delay = (TIMER / 697) / 2;
			col_delay = (TIMER / 1633) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			toggle_off_tone();
		}
		else {
		}
	}
	else if (row_to_check == 3) {
		PORTC &= ~(1 << PC3);
		PORTC |= (1 << PC2) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, number 4, PB1 = 770Hz, PB2 = 1209Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1209) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 5, PB1 = 770Hz, PB2 = 1336Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1336) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, number 6, PB1 = 770Hz, PB2 = 1477Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1477) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter B, PB1 = 770Hz, PB2 = 1633Hz
			row_delay = (TIMER / 770) / 2;
			col_delay = (TIMER / 1633) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			toggle_off_tone();
		}
		else {
		}
	}
	else if (row_to_check == 2) {
		PORTC &= ~(1 << PC4);
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, number 7, PB1 = 852Hz, PB2 = 1209Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1209) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 8, PB1 = 852Hz, PB2 = 1336Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1336) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, number 9, PB1 = 852Hz, PB2 = 1477Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1477) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter C, PB1 = 852Hz, PB2 = 1633Hz
			row_delay = (TIMER / 852) / 2;
			col_delay = (TIMER / 1633) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			toggle_off_tone();
		}
		else {
		}
	}
	else if (row_to_check == 1) {
		PORTC &= ~(1 << PC5);
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC4);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1, letter *, PB1 = 941Hz, PB2 = 1209Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1209) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2, number 0, PB1 = 941Hz, PB2 = 1336Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1336) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3, letter #, PB1 = 941Hz, PB2 = 1477Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1477) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			toggle_off_tone();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4, letter D, PB1 = 941Hz, PB2 = 1633Hz
			row_delay = (TIMER / 941) / 2;
			col_delay = (TIMER / 1633) / 2;
			toggle_on_tone();
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			toggle_off_tone();
		}
		else {
		}
	}
	else {
	}
}

//overflows every 4ms, changes row to check
ISR(TIMER2_OVF_vect) {
	if (row_to_check == 4) {
		row_to_check = 3;
	}
	else if (row_to_check == 3) {
		row_to_check = 2;
	}
	else if (row_to_check == 2) {
		row_to_check = 1;
	}
	else if (row_to_check == 1) {
		row_to_check = 4;
	}
	else { printf("ERROR: invalid row\n"); }
}

//set row frequency
ISR(TIMER1_COMPA_vect) {
	OCR1A += row_delay;
}

//set col frequency
ISR(TIMER1_COMPB_vect) {
	OCR1B += col_delay;
}

int main() {
	uart_init();
	cli();
	//set rows as output
	DDRC |= (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);
	//set columns as input
	DDRD = 0;
	//set pin9 and pin10 as outputs
	DDRB |= (1 << PB1) | (1 << PB2);   
	row_to_check = 4;
	//enable overflow interrupt
	TIMSK2 |= (1 << TOIE2);
	//enable timer2, prescaler 256
	TCCR2B |= (1 << CS22) | (1 << CS21); 

	sei();

	while(1) {
		decode();
	}
}
