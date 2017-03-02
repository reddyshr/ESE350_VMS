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

void init_timer0() {
	TCCR0A |= (1 << COM0A0); //toggle OC0A on compare match
	TCCR0A |= (1 << WGM01); // enable CTC mode
	TCCR0B |= (1 << x01) | (1 << x00); //enable 64 prescaler	
}

void init_timer1() {
	TCCR1A |= (1 << COM1A0); //toggle OC1A on compare match
	TCCR1B |= (1 << WGM12); // enable CTC mode
	TCCR1B |= (1 << x00); //enable no prescaler
}

int main() {
	DDRB |= (1 << PB1); //set PB1 and PD6 to outputs
	DDRD |= (1 << PD6);
	init_timer0();
	init_timer1();
	OCR1A = 11477;
	OCR0A = 103;
	
	while(1);		
}