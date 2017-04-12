/*
 * PID.cpp
 *
 * Created: 4/9/2017 4:51:37 PM
 *  Author: Shreetej
 */ 

#include <avr/io.h>
#include "PID.h"

//KP gain is attached to A0
//KD gain is attached to A1

float PID::getKP() {
	ADMUX = (1 << REFS0); //ADC0 input
	ADCSRA = (1 <<ADPS2) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN) | (1 << ADSC);
  while (ADCSRA & (1<<ADSC));
  float val = ADC;
	return(val);
}

float PID::getKD() {
	ADMUX = (1 << REFS0) | (1 << 0x00); //ADC1 input
  ADCSRA = (1 <<ADPS2) | (1 << ADPS0);
  ADCSRA |= (1 << ADEN) | (1 << ADSC);
  while (ADCSRA & (1<<ADSC));
  float val = ADC;
  return(val);
}