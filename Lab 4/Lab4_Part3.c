#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define F_CPU 16000000
#define BAUD 9600
#include "uart.h"


unsigned int period = 7999;
unsigned int currRow = 1;
unsigned int currColumn = 0;


void findColumn(void) {
	if (((PIND & (1 << PD2)) >> PD2) == 0) {
		currColumn = 1;
	} else if (((PIND & (1 << PD3)) >> PD3) == 0) {
		currColumn = 2;
	} else if (((PIND & (1 << PD4)) >> PD4) == 0) {
		currColumn = 3;
	} else if (((PIND & (1 << PD5)) >> PD5) == 0) {
		currColumn = 4;
	} else {
		currColumn = 0;
	}
}

char getKey(int row, int column) {	
	if (row == 1) {
		if (column == 1) {
			return '1';
		} else if (column == 2) {
			return '2';
		} else if (column == 3) {
			return '3';
		} else if (column == 4) {
			return 'A';
		}
	} else if (row == 2) {
		if (column == 1) {
			return '4';
		} else if (column == 2) {
			return '5';
		} else if (column == 3) {
			return '6';
		} else if (column == 4) {
			return 'B';
		}
	} else if (row == 3) {
		if (column == 1) {
			return '7';
		} else if (column == 2) {
			return '8';
		} else if (column == 3) {
			return '9';
		} else if (column == 4) {
			return 'C';
		}		
	} else if (row == 4) {
		if (column == 1) {
			return '*';
		} else if (column == 2) {
			return '0';
		} else if (column == 3) {
			return '#';
		} else if (column == 4) {
			return 'D';
		}		
	} 
}
//For Generating Frequency of Tone
ISR (TIMER2_COMPA_vect) {
	if (currRow == 1) {
		PORTC = 0;
		PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);
		findColumn();
		if (currColumn != 0) {
			putchar(getKey(currRow, currColumn));
		}		
		currRow = 2;
	} else if (currRow == 2) {
		PORTC = 0;
		PORTC |= (1 << PC2) | (1 << PC4) | (1 << PC5);
		findColumn();
		if (currColumn != 0) {
			putchar(getKey(currRow, currColumn));
		}
		currRow = 3;
	} else if (currRow == 3) {
		PORTC = 0;
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC5);
		findColumn();
		if (currColumn != 0) {
			putchar(getKey(currRow, currColumn));
		}
		currRow = 4;
	} else if (currRow == 4) {
		PORTC = 0;
		PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC4);
		findColumn();
		if (currColumn != 0) {
			putchar(getKey(currRow, currColumn));
		}
		currRow = 1;
	}
}

int main(void)
{
	uart_init();
	DDRC |= (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5); //Set rows to output	
	DDRD = 0;
	TCCR2A |= (1 << WGM21); //Sets to CTC mode (clears TCNT2 when OCR2A == TCNT2)
	TCCR2B |= (1 << 2); //Select prescaler to 8 
	TIMSK2 |= 0x02;	//Enable Output Compare A Match Interrupt
	TCNT2 = 0;
	OCR2A = period; 
	sei();
	
	while(1) {
		
	}

}