/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Baron Young, Maxwell Jung
 * Section: Wed 7pm
 * Lab: 4B
 */
#define RTR 0x00

#include "stm32l476xx.h"
#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

// Note the "<< 1" must be present because bit 0 is treated as a don't care in 7-bit addressing mode
static uint8_t SlaveAddress = 0x48 << 1; // A0 sensor (0b1001000);
static uint8_t Data_Receive;
static uint8_t command = RTR; // Read Temperature Register
static int temperature = 0x80; // lowest possible temperature

void Init_USARTx(int x);

// Initializes USARTx
// USART2: UART Communication with Termite
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

int main(void) {
	int i;

	System_Clock_Init(); // System Clock = 80 MHz
	
	// Initialize I2C
	I2C_GPIO_Init();
	I2C_Initialization();

	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(2);
	printf("USART initialized\n");

	while(1) {	
		// Get Temperature
		// First, send a command to the sensor for reading the temperature
		I2C_SendData(I2C1, SlaveAddress, &command, 1);
		// Next, get the measurement
		I2C_ReceiveData(I2C1, SlaveAddress, &Data_Receive, 1);
		// Print Temperature to Termite
		// convert unsigned 8 bit to 2's complement signed 8 bit
		// for 8 bit binary, 2's complement is equal to
		// value of magnitude bits - 2^(n-1) * sign bit
		temperature = (int)(0x7f & Data_Receive) - (int)(0x80 & Data_Receive);
		printf("Temperature: %dC\n", temperature);
		
		// Some delay
		for(i = 0; i < 50000; ++i); 
	}
}
