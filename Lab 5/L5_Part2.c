#include <avr/io.h>
#include <avr/interrupt.h>


int main() {
	DDRC |= (1 << PC0);
	ADMUX |= (1 << REFS0); //AVCC is ref voltage
	ADCSRA |= (1 << ADEN) | (1 << ADATE);// enable ADC and auto trigger
	ADCSRA |= (1 << ADSC); //start conversion
	while(1) {
		//PRINT ADC value from ADC reg
	}
}