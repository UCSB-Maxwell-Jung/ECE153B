/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 4A
 */


#include "stm32l476xx.h"
#include "LED.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

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
	System_Clock_Init(); // Switch System Clock = 80 MHz
	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(1);
	LED_Init();
	char command[64]; // assume command typed by the user is at most 63 characters (63 + 1 null character)
	while(1) {
		printf("Please enter a command:");
		scanf("%s", command);
		
		if((strcmp(command, "Y") == 0) || (strcmp(command, "y") == 0)){
			printf("LED ON \n");
			Green_LED_On();
		}
		else if((strcmp(command, "N") == 0) || (strcmp(command, "n") == 0)){
			printf("LED OFF \n");
			Green_LED_Off();
		}
		else {
			printf("Please input valid character: 'y'/'Y' or 'n'/'N' \n");
		}
	}
}
