#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define BAUD 9600
#define F_CPU 16000000
#include <stdio.h>
#include "uart.h"


int main() {
	uart_init();
	
	ADMUX = (1 << REFS0); //ADC0 input
	ADCSRA = (1 <<ADPS2) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN) | (1 << ADATE);
	ADCSRA |= (1 << ADSC);
	
	while (1) {
		printf("%u\n", (uint16_t) ADC);
	}
	
}