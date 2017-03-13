#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"


int main() {
	//Do we need to change the conversion rate with ADCSRA?
	DDRC   |= (1 << PC0);
	ADMUX   = 0;                           //set analog channel as PC0/ADC0  
	ADMUX  |= (1 << REFS0);                //AVCC is ref voltage
	ADCSRA |= (1 << ADEN) | (1 << ADATE);  //enable ADC and auto trigger
	ADCSRA |= (1 << ADSC);                 //start conversion
	while((ADCSRA & (1 << ADSC)) >> ADSC); //wait for conversion to finish
	printf(ADC);
}