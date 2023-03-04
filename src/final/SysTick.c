
#include "SysTick.h"

volatile uint32_t current_ms_elapsed;


//******************************************************************************************
// Initialize SysTick	
//******************************************************************************************	
void init_SysTick(void){
	// generate 1ms interrupt
	// SysTick Control & Status Register
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // disable SysTick IRQ and SysTick Counter
	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // set to 1

	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; // set to 0 (80Mhz/8 = 10Mhz)

	SysTick->LOAD = 9999; // set auto reload value to (10Mhz * 1ms) - 1

	SysTick->VAL = 0; // set starting value to 0

	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1

	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	// set the starting ms elapsed to 0
	current_ms_elapsed = 0;
}

//******************************************************************************************
// SysTick Interrupt Handler
//******************************************************************************************
void SysTick_Handler(void){
	current_ms_elapsed++;
}
	
//******************************************************************************************
// Delay in ms
//******************************************************************************************
void delay(uint32_t ms_wait_duration){
	uint32_t future_ms_elapsed;

	future_ms_elapsed = current_ms_elapsed + ms_wait_duration;
	while (current_ms_elapsed < future_ms_elapsed);
}

uint32_t micros(void){
	return current_ms_elapsed;
}