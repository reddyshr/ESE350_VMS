/*
 * TWI.h
 *
 * Created: 4/8/2017 3:32:51 PM
 *  Author: Shreetej
 */ 


#ifndef TWI_H_
#define TWI_H_
#include <avr/io.h>

class TWI {
	public:
	static void initialize();
	static unsigned int read(unsigned int addressRead);
	static void write(unsigned int addressWrite, unsigned int data);
	
	private:
	static void error();
};




#endif /* TWI_H_ */