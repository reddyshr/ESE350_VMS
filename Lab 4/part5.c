/*
 * part5.c
 *
 * Created: 3/1/2017 9:09:17 PM
 *  Author: Shreetej
 */ 

//NOTES
//For Timer 1 with no prescaler
// for 697 Hz, set OCR1A to 11477 (11477.26)
// for 770 Hz, set OCR1A to 10389 (10389.11)
// for 852 Hz, set OCR1A to 9389 (9389.17)
// for 941 Hz, set OCR1A to 8501 (8501.1)

//For Timer 0 with a 64 prescaler
// for 1209 Hz, set OCR0A to 103 (102.89)
// for 1336 Hz, set OCR0A to 93 (93.065)
// for 1477 Hz, set OCR0A to 84 (84.13)
// for 1633 Hz, set OCR0A to 76 (76.045)


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

volatile int row_to_check;

void zeroFrequency() {
	OCR1A = 0;
	OCR0A = 0;	
}
void decode() {
	if (row_to_check == 4) {
		PORTC &= ~(1 << PC2);
		PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1 -> '1'
			OCR1A = 11477;
			OCR0A = 103;			
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2 -> '2'
			OCR1A = 11477;
			OCR0A = 93;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3 - '3'
			OCR1A = 11477;
			OCR0A = 84;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4 -> 'A'
			OCR1A = 11477;
			OCR0A = 76;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			zeroFrequency();
		}
		else {
		}
		row_to_check = 3;
	}
	
	else if (row_to_check == 3) {
		PORTC &= ~(1 << PC3);
		PORTC |= (1 << PC2) | (1 << PC4) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1 -> '4'
			OCR1A = 10389;
			OCR0A = 103;
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2 -> '5'
			OCR1A = 10389;
			OCR0A = 93;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3 -> '6'
			OCR1A = 10389;
			OCR0A = 64;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4 -> 'B'
			OCR1A = 10389;
			OCR0A = 76;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			zeroFrequency();
		}
		else {
		}
		row_to_check = 2;
	}
	else if (row_to_check == 2) {
		PORTC &= ~(1 << PC4);
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC5);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1 -> '7'
			OCR1A = 9389;
			OCR0A = 103;
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2 -> '8'
			OCR1A = 9389;
			OCR0A = 93;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3 -> '9'
			OCR1A = 9389;
			OCR0A = 84;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4 -> 'C'
			OCR1A = 9389;
			OCR0A = 76;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			zeroFrequency();
		}
		else {
		}
		row_to_check = 1;
	}
	else {
		PORTC &= ~(1 << PC5);
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC4);
		if (((PIND & (1 << PD2)) >> PD2) == 0) {
			//column 1 -> '*'
			OCR1A = 8501;
			OCR0A = 103;
			while(((PIND & (1 << PD2)) >> PD2) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD3)) >> PD3) == 0) {
			//column 2 -> '0'
			OCR1A = 8501;
			OCR0A = 93;
			while(((PIND & (1 << PD3)) >> PD3) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD4)) >> PD4) == 0) {
			//column 3 -> '#'
			OCR1A = 8501;
			OCR0A = 84;
			while(((PIND & (1 << PD4)) >> PD4) == 0);
			zeroFrequency();
		}
		else if (((PIND & (1 << PD5)) >> PD5) == 0) {
			//column 4 -> 'D'
			OCR1A = 8501;
			OCR0A = 76;
			while(((PIND & (1 << PD5)) >> PD5) == 0);
			zeroFrequency();
		}
		else {
		}
		row_to_check = 4;
	}
}

void init_timer0() {
	TCCR0A |= (1 << COM0A0); //toggle OC0A on compare match
	TCCR0A |= (1 << WGM01); // enable CTC mode
	TCCR0B |= (1 << 1) | (1 << 0); //enable 64 prescaler	
}

void init_timer1() {
	TCCR1A |= (1 << COM1A0); //toggle OC1A on compare match
	TCCR1B |= (1 << WGM12); // enable CTC mode
	TCCR1B |= (1 << 0); //enable no prescaler
}

void init_timer2() {	
	TCCR2A |= (1 << WGM21); //Sets to CTC mode (clears TCNT2 when OCR2A == TCNT2)
	TCCR2B |= (1 << 2) | (1 << 1); 	//enable TIMER2 with prescaler = 256
	OCR2A = 255;
	TIMSK2 |= 0x02; 	//Enable Output Compare A Match Interrupt
}

ISR(TIMER2_COMPA_vect) {
	decode();
}

int main() {
	DDRC |= (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5); //set rows as output
	DDRB |= (1 << PB1); //set PB1 and PD6 to outputs
	DDRD |= (1 << PD6);
	init_timer0();
	init_timer1();
	init_timer2();
	zeroFrequency();	
	sei();

	row_to_check = 4;
	TCNT2 = 0;
	
	while(1);		
}