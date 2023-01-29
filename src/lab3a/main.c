/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 3A
 */

#include "stm32l476xx.h"
 
void PWM_Init() {
	// Enable GPIO Port A Clock
	// [TODO]
	
	// Enable TIM2 Clock
	// [TODO]
	
	// Configure PA5
	// [TODO]
	
	// Configure PWM Output for TIM2 CH 1
	// [TODO]
}
 
int main() {
	// Initialization - We will use the default 4 MHz clock
	PWM_Init();
	
	// Periodic Dimming
	int i;
	while(1) {
		// [TODO] (changing duty cycle, etc.)
		
		for(i=0; i<1000; ++i); // Some Delay
	}
	
	return 0;
}
