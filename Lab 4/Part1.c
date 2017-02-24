

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int half_period = 18181;

ISR (TIMER2_COMPA_vect) {
	OCR1A += half_period;
}
//adding a comment
int main(void)
{
	DDRB |= (1 << PB1);
	TCCR1A |= 0x40;
	TCCR1B |= 0x01;
	TIMSK1 = 0x02;		
	OCR1A += TCNT1 + half_period;
	sei();
	
    while(1)
    {        
    }
}