/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 4C
 */


#include "stm32l476xx.h"
#include "LED.h"
#include "SPI.h"
#include "SysClock.h"
#include "SysTimer.h"
#include <stdio.h>

#define SPI_MASTER SPI2
#define SPI_SLAVE SPI3

uint8_t transmit = 0;
uint8_t receive = 0;

int main(void){
	System_Clock_Init();   // System Clock = 80 MHz
	SysTick_Init();
	
	LED_Init();
	
	SPI_GPIO_Init();
	SPI_Init();

	while(1) {
		SPI_Transfer_Byte(SPI1, transmit, &receive); // Transmit message from SPI1 and receive message from SPI2
		// delay between SPI transfers to observe LED behavior
	    delay(1000);
		if (receive == transmit-1) // SPI1 received value == SPI2 transmitted value
			Green_LED_Toggle();

		transmit++;
	}
}
