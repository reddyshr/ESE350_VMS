/*
 * TWI.cpp
 *
 * Created: 4/8/2017 3:39:38 PM
 *  Author: Shreetej
 */ 

#include "TWI.h"
using namespace std;
#define START 0x08
#define MR_SLA_ACK 0x40
#define MR_DATA_NACK 0x58

void TWI::error() {
	//DO SOMETHING
	while(1);
	return;
}

void TWI::initialize() {
  PRR &= ~(1 << PRTWI); //Set PRTWI to 0
	TWBR = 0x48; // Set SCL line to 100 kHz
	TWCR = (1 << TWEN); //Enable I2C
	TWSR = 0x00; //set prescaler value to 1
}

unsigned int TWI::read(unsigned int addressRead) {
	unsigned int data;
	//Start Condition and Transmission Check
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	//Check if Status Register indicates Start
	if ((TWSR & 0xF8) != START) {
		return 1;
	}
	
	//Load Slave Address + R and Transmission Check
	TWDR = addressRead;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	//Check if ACK by slave
	if ((TWSR & 0xF8) != MR_SLA_ACK) {
		return 2;
	}
	
	//Receive Data Byte
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	//Check if NACK
	if ((TWSR & 0xF8) != MR_DATA_NACK) {
		return 3;
	}
	
	// Read Data Byte and Stop Transmission
	data = TWDR;
	TWCR = (1 << TWINT) | (1 << TWEN) || (1 << TWSTO);
	return data;
}

void TWI::write(unsigned int address, unsigned int data) {
	return;
}

