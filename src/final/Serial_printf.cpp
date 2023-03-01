#include "Serial.h"
#include <stdio.h>

// Implement a dummy __FILE struct, which is called with the FILE structure.
struct _FILE {
	int dummy;
};

// We have to define FILE if prinf is used
FILE __stdout;
FILE __stdin;
 
// Retarget printf() to USARTx
int fputc(int ch, FILE *f) { 
	uint8_t c;
	c = ch & 0x00FF;
	Serial::write((uint8_t *)&c, 1);
	return(ch);
}

// Retarget scanf() to USARTx
int fgetc(FILE *f) {  
	uint8_t rxByte;
	rxByte = Serial::read();
	return rxByte;
}
