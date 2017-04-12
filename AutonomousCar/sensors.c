#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

const int triggerPulseHi = 2;
const int triggerPulseLo = 14999;

//BOOLEANS TO DETERMINE WHEN SAMPLES ARE READY
volatile int s1_ready;
volatile int s2_ready;
volatile int s3_ready;
volatile int s4_ready;
volatile int s5_ready;

//BOOLEANS TO DETERMINE IF SAMPEL HAS BEEN TAKEN
volatile int s1_reading;
volatile int s2_reading;
volatile int s3_reading;
volatile int s4_reading;
volatile int s5_reading;

//RISING EDGE TIMES
volatile int s1_start;
volatile int s2_start;
volatile int s3_start;
volatile int s4_start;
volatile int s5_start;

//FALLING EDGE TIMES
volatile int s1_end;
volatile int s2_end;
volatile int s3_end;
volatile int s4_end;
volatile int s5_end;

//PULSE WIDTHS
volatile int s1_pulse;
volatile int s2_pulse;
volatile int s3_pulse;
volatile int s4_pulse;
volatile int s5_pulse;

//DELAY FOR TRIGGER PULSE
volatile int delay;

int calculate_distance (int pulseWidth) {
  int time_us = 4 * pulseWidth;        //pulsewidth in us 
  int hit_time = time_us / 2;          //time until it hit in us
  return (int) (hit_time * 0.034); 
}

void print_distance() {
  int s1_dist = calculate_distance(s1_pulse);
  int s2_dist = calculate_distance(s2_pulse);
  int s3_dist = calculate_distance(s3_pulse);
  int s4_dist = calculate_distance(s4_pulse);
  int s5_dist = calculate_distance(s5_pulse);
	printf("Sensor 1: %ucm, Sensor 2: %ucm, Sensor 3: %ucm, Sensor 4: %ucm, Sensor 5: %ucm\n", s1_dist, s2_dist, s3_dist, s4_dist, s5_dist);
}

void reset_variables() {
  s1_ready = 0;
  s2_ready = 0;
  s3_ready = 0;
  s4_ready = 0;
  s5_ready = 0;
  s1_start = 0;
  s2_start = 0;
  s3_start = 0;
  s4_start = 0;
  s5_start = 0;
  s1_end = 0;
  s2_end = 0;
  s3_end = 0;
  s4_end = 0;
  s5_end = 0;
  s1_reading = 0;
  s2_reading = 0;
  s3_reading = 0;
  s4_reading = 0;
  s5_reading = 0;
  s1_pulse = 0;
  s2_pulse = 0;
  s3_pulse = 0;
  s4_pulse = 0;
  s5_pulse = 0;
}


ISR(TIMER1_COMPA_vect) {
	if (delay == triggerPulseHi) {
		delay = triggerPulseLo;
		OCR1A += delay;
	}
	else if (delay == triggerPulseLo) {
		delay = triggerPulseHi;
		OCR1A += delay;
	}
	else {
		printf("error!\n");
	}
}

void init_timer1() {
	TCCR1B |= (1 << CS11) | (1 << CS10);    //start timer, 64 prescaler
	TCCR1A |= (1 << COM1A0);                //toggle OC1A(Pin9) on compare match
	TIMSK1 |= (1 << OCIE1A);                //enable OCA interrupt
	delay = triggerPulseHi;                 //start with high pulse
	TCNT1 = 0;
  OCR1A = TCNT1 + delay;                  //2 cycles = 10us needed
}

int main() {
	cli();
	uart_init();
	DDRB  |= (1 << PB1);    //set PB1(Pin9) as output
	DDRD  &= ~(1 << PD7);   //set PD7(Pin7) as input
	DDRD  &= ~(1 << PD6);   //set PD6(Pin6) as input
	DDRD  &= ~(1 << PD5);   //set PD5(Pin5) as input
	DDRD  &= ~(1 << PD4);   //set PD4(Pin4) as input
	DDRD  &= ~(1 << PD3);   //set PD3(Pin3) as input
	PORTB = (1 << PORTB1);  //set PB1(Pin9) high
	sei();
	reset_variables();
	init_timer1();

  while(1) {	
    if (s1_ready == 1 && s2_ready == 1 && s3_ready == 1 && s4_ready && s5_ready) {
      print_distance();
      reset_variables();
    }
    if ((((PIND & (1 << PIND7)) >> PIND7) == 1) && s1_reading == 0 && s1_ready == 0) {
      //sensor 1 rising edge
      s1_start = TCNT1;
      s1_reading = 1;
    }
    if ((((PIND & (1 << PIND7)) >> PIND7) == 0) && s1_reading == 1 && s1_ready == 0) {
      //sensor 1 falling edge
      s1_end = TCNT1;
      s1_pulse = s1_end - s1_start;
      s1_ready = 1;
    }  
    if ((((PIND & (1 << PIND6)) >> PIND6) == 1) && s2_reading == 0 && s2_ready == 0) {
      //sensor 2 rising edge
      s2_start = TCNT1;
      s2_reading = 1;
    }
    if ((((PIND & (1 << PIND6)) >> PIND6) == 0) && s2_reading == 1 && s2_ready == 0) {
      //sensor 2 falling edge
      s2_end = TCNT1;
      s2_pulse = s2_end - s2_start;
      s2_ready = 1;
    }
    if ((((PIND & (1 << PIND5)) >> PIND5) == 1) && s3_reading == 0 && s3_ready == 0) {
      //sensor 3 rising edge
      s3_start = TCNT1;
      s3_reading = 1;
    }
    if ((((PIND & (1 << PIND5)) >> PIND5) == 0) && s3_reading == 1 && s3_ready == 0) {
      //sensor 3 falling edge
      s3_end = TCNT1;
      s3_pulse = s3_end - s3_start;
      s3_ready = 1;
    }
    if ((((PIND & (1 << PIND4)) >> PIND4) == 1) && s4_reading == 0 && s4_ready == 0) {
      //sensor 4 rising edge
      s4_start = TCNT1;
      s4_reading = 1;
    }
    if ((((PIND & (1 << PIND4)) >> PIND4) == 0) && s4_reading == 1 && s4_ready == 0) {
      //sensor 4 falling edge
      s4_end = TCNT1;
      s4_pulse = s4_end - s4_start;
      s4_ready = 1;
    }
    if ((((PIND & (1 << PIND3)) >> PIND3) == 1) && s5_reading == 0 && s5_ready == 0) {
      //sensor 5 rising edge
      s5_start = TCNT1;
      s5_reading = 1;
    }
    if ((((PIND & (1 << PIND3)) >> PIND3) == 0) && s5_reading == 1 && s5_ready == 0) {
      //sensor 5 falling edge
      s5_end = TCNT1;
      s5_pulse = s5_end - s5_start;
      s5_ready = 1;
    }  
  }
	return 1;
}
