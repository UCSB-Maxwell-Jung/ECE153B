
#include "SysTick.h"

#include "stm32l476xx.h"

volatile uint32_t us_elapsed;


//******************************************************************************************
// Initialize SysTick
// generate 1 tick/interrupt every 1us
//******************************************************************************************	
void initSysTick(void){
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

	SysTick->LOAD = 9; // set auto reload value to (10Mhz * 1us) - 1

	SysTick->VAL = 0; // set starting value to 0

	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1

	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	// set the starting us elapsed to 0
	us_elapsed = 0;
}

//******************************************************************************************
// SysTick Interrupt Handler
//******************************************************************************************
void SysTick_Handler(void){
	us_elapsed++;
}
	
//******************************************************************************************
// Delay in ms
//******************************************************************************************
void delay(uint32_t ms){
	uint32_t future_time;

	future_time = us_elapsed + 1000*ms;
	while (us_elapsed < future_time);
}

//******************************************************************************************
// Delay in us
//******************************************************************************************
void delayMicroseconds(uint32_t us){
	uint32_t future_time;

	future_time = us_elapsed + us;
	while (us_elapsed < future_time);
}

uint32_t millis(void){
	return us_elapsed/1000;
}

uint32_t micros(void){
	return us_elapsed;
}