#include "SysClock.h"

void System_Clock_Init(void) {
	// // Enable HSI
	// RCC->CR |= RCC_CR_HSION;
	// while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	// // Select HSI as system clock source
	// RCC->CFGR &= ~RCC_CFGR_SW;
	// RCC->CFGR |= RCC_CFGR_SW_HSI;
	// while((RCC->CFGR & RCC_CFGR_SWS) == 0);

	RCC->CR |= RCC_CR_MSION; // enable MSI oscillator
	while((RCC->CR & RCC_CR_MSIRDY) == 0); // Wait until MSI is ready

	RCC->CFGR &= ~RCC_CFGR_SW; // reset clock source
	RCC->CFGR |= RCC_CFGR_SW_MSI; // set MSI as system clock source
	RCC->CR &= ~RCC_CR_MSIRANGE; // reset MSI clock range
	RCC->CR |= RCC_CR_MSIRANGE_7; // set MSI clock range as 8 MHz
	RCC->CR |= RCC_CR_MSIRGSEL; // use the MSI clock range that is defined in RCC_CR
}
