#include "SysTick.h"
#include <stdbool.h>

#define SYSTICK_CLK_FREQ 10000000 // unit = ticks/second = Hz
#define SYSTICK_CLK_FREQ_IN_MS 10000 // unit = ticks/ms
#define MAX_TICK_COUNT 0xffffff // counter is a 24 bit register so max value is 0xffffff

// this variable is true when system calls delay() function
volatile bool waiting;

//******************************************************************************************
// Initialize SysTick
// But don't start counting
//******************************************************************************************	
void init_SysTick(void){
	// disable SysTick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	// Select clock source as external (0) (AHB/8 = 80MHz/8 = 10MHz = SYSTICK_CLK_FREQ)
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	
	// set counter to maximum possible value
	SysTick->LOAD = MAX_TICK_COUNT;
	SysTick->VAL = MAX_TICK_COUNT;

	// Enables SysTick interrupt request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1

	waiting = false;
}

//******************************************************************************************
// SysTick Interrupt Handler
//******************************************************************************************
void SysTick_Handler(void){
	waiting = true;
}
	
//******************************************************************************************
// Delay in ms
// Max delay = 1.6777216 seconds = 1678 ms
//******************************************************************************************
void delay(uint32_t ms){
	waiting = true;

	uint32_t ticks = SYSTICK_CLK_FREQ_IN_MS * ms - 1;
	if (ticks > MAX_TICK_COUNT)
		ticks = MAX_TICK_COUNT;

	SysTick->VAL = ticks;

	// start counting down by starting systick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while (waiting);
	// stop systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
