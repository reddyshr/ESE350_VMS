#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

const int triggerPulseHi = 2;
const int triggerPulseLo = 14999;

volatile int delay;

ISR(TIMER1_COMPA_vect) {
	if (delay == triggerPulseHi) {
		//PORTB &= ~(1 << PORTB1); //set pin9 low
		delay = triggerPulseLo;
		OCR1A += delay;
	}
	else if (delay == triggerPulseLo) {
		//PORTB |= (1 << PORTB1);  //set pin9 high
		delay = triggerPulseHi;
		OCR1A += delay;
	}
	else {
		printf("error!\n");
	}
}

ISR(TIMER1_CAPT_vect) {
}

void init_timer1() {
	TCCR1B |= (1 << CS11) | (1 << CS10);   //start timer, 64 prescaler
	TCCR1A |= (1 << COM1A0);   //toggle OC1A(Pin9) on compare match
	TIMSK1 |= (1 << OCIE1A);   //enable OCA interrupt
	delay = triggerPulseHi;
	OCR1A = TCNT1 + delay;       //2 cycles = 10us needed
}

int main() {
	cli();
	uart_init();
	DDRB  |= (1 << PB1);       //set PB1(Pin9) as output
	DDRB  &= ~(1 << PB0);      //set PB0(Pin8) as input
	PORTB = (1 << PORTB1);     //set PB1(Pin9) high
	sei();
	init_timer1();

  while(1);

	return 1;
}
