
#include "SysTimer.h"

volatile uint32_t msTicks;


//******************************************************************************************
// Initialize SysTick	
//******************************************************************************************	
void SysTick_Init(void){
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

	SysTick->LOAD = (10*1000000)*0.001-1; // set auto reload value to (10Mhz * 1ms) - 1

	SysTick->VAL = 0; // set starting value to 0

	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1

	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	// set the starting tick count as 0
	msTicks = 0;
}

//******************************************************************************************
// SysTick Interrupt Handler
//******************************************************************************************
void SysTick_Handler(void){
	msTicks++;
}
	
//******************************************************************************************
// Delay in ms
//******************************************************************************************
void delay (uint32_t T){
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < T);
		
	msTicks = 0;
}
