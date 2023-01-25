/*
 * ECE 153B - Winter 2021
 *
 * Name(s):
 * Section:
 * Lab: 2B
 */

#include "SysTimer.h"

uint32_t static volatile msTicks;

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter
	
	// SysTick Reload Value Register
	SysTick->LOAD = (1*1000000)*0.001-1; // (clock freq * desired period) - 1 = number of ticks needed
	
	// SysTick Current Value Register
	SysTick->VAL = 0;
	
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1
	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // set to 1

	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; // reset to 0

	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	// set the starting tick count as 0
	msTicks = 0;
}

void SysTick_Handler(void) {
	++msTicks;
}

void delay(uint32_t T) {
	uint32_t startingTicks = msTicks; // Hint: It may be helpful to keep track of what the current tick count is

	// [TODO] - Implement function that waits until a time specified by argument T
	while (msTicks-startingTicks < T);
}
