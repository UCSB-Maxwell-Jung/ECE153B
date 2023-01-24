/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Baron Young, Maxwell Jung
 * Section: Wed 7pm
 * Lab: 2B
 */

#include "stm32l476xx.h"
#include "LED.h"
#include "SysTimer.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source
	RCC->CFGR |= RCC_CFGR_SW_MSI; // - Replace with value that will make MSI the system clock source
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7; // Replace with value that will make range 8 MHz
	
	// Use the MSI clock range that is defined in RCC_CR
	RCC->CR |= RCC_CR_MSIRGSEL; //  - Replace with value that will select range in RCC->CR
	
	// Enable MSI oscillator
	RCC->CR |= RCC_CR_MSION; //  - Replace with value that will enable MSI
	
	// Wait until MSI is ready
	while((RCC->CR & RCC_CR_MSIRDY/* - Replace with value that checks whether MSI is ready */) == 0);
}

int main() {
	// Initialization 
	System_Clock_Init();
	SysTick_Init();
	LED_Init();
	
	while(1) {
		// [TODO] Write Simple Program here...
	}
}
