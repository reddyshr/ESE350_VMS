#include <avr/io.h>
#include <avr/interrupt.h>

volatile int start;
volatile int end; 
volatile int diff;
volatile int sampleCollected;



void init_oc_timer1() {
	TIMSK1 = 0; //disable all interrupts
	TCCR1A |= (1 << COM1A1); // clear PB1 on output compare
	OCR1A = x07; // set OC register to 7 -> 5 us
	TIMSK1 |= (1 << OCIE1A); // enable OCA interrupt
	TCCR1B |= (1 << CS10); // no prescaler/start timer
}

void init_ic_timer1() {
	TIMSK1 = 0; //disconnect output compare interrupt
	TCCR1B |= (1 << ICES1); // trigger is rising edge	
	TIMSK1 |= (1 << ICIE1); //enable input capt interrupt
}

ISR(TIMER1_CAPT_vect) {
	if ((TCCR1B & (1 << ICES1)) == 1) { //trigger is rising edge
		start = ICR1; //rising edge input capture value
		TCCR1B &= ~(1 << ICES1)// trigger is falling edge
	} else {
		end = ICR1; //falling edge input capture value
		diff = start - end;
		sampleCollected = 1;
	}
}
ISR(TIMER1_COMPA_vect) {
	DDRB &= ~(1 << PB1); //Set PB1 to input
	init_ic_timer1(); // set timer1 for input capture
	TCNT1 = 0; //reset timer count
}


int main(void)
{
	DDRB |= (1 << PB1); //PB1 is output
	PORTB |= (1 << PORTB1); //Set PB1 to high
	init_oc_timer1();
	sei();
	
    while(1)
    {    
		if (sampleCollected) {
			//PRINT DIFF
			TIMSK1 = 0; //disable interrupts
			TCNT1 = 0; // clear timer count
			TIMSK1 |= (1 << OCIE1A); // enable OCA interrupt
			sampleCollected = 0;
		}
    }
}