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
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Enable TIM2 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// Configure PA5
	uint8_t pin_number = 5;
	// GPIO Mode: Input(00), Output (01),
	// AF(10), Analog (11)
	GPIOA->MODER &= ~0b11U << (pin_number * 2); // reset to 00
	GPIOA->MODER |= 0b10U << (pin_number * 2);	// set to AF

	// GPIO Output Speed: Low (00), Medium (01), 
	// High (10), Very High (11)
	GPIOA->OSPEEDR &= ~0b11U << (pin_number * 2); // reset to 00
	GPIOA->OSPEEDR |= 0b11u << (pin_number * 2);  // set to Very High

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~0b11U << (pin_number * 2); // reset to 00
	GPIOA->PUPDR |= 0b00U << (pin_number * 2);	// set to No Pull-Up, No Pull-Down

	// Write alternate function 1 (Timer 2 Channel 1) into AFR
	GPIOA->AFR[0] &= ~0b1111 << (pin_number * 4); // reset to 0000
	GPIOA->AFR[0] |= 0b0001 << (pin_number * 4); // set to AF1 (TIM2_CH1)

	// Configure PWM Output for TIM2 CH 1
	// 7a
	TIM2->CR1 &= ~TIM_CR1_DIR; // set counter direction: upcounter (up = 0, down = 1)
	// 7b
	TIM2->PSC = 3; // set prescalar value
				   // Explanation: downscale 4 Mhz clock input to 1 Mhz by dividing by PSC + 1: 4 MHz/(3+1) = 1 Mhz
	// 7c
	TIM2->ARR = 999; // set auto-reload value
					  // Explanation: reset timer every 1ms (1000 ticks @ 1us/tick)
	// below 2 lines might be unnecessary
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S; // reset capture/compare 1 selection
	TIM2->CCMR1 |= 0b00;			// configure CC1 channel as output
	// 7d
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M; // clear output compare mode bits
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // set output compare mode bits to PWM mode 1 (0110)
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // enable output preload
	// 7e
	TIM2->CCER |= TIM_CCER_CC1P; // Set the output polarity for compare 1 to active high
	// 7f
	TIM2->CCER |= TIM_CCER_CC1E; // Enable the channel 1 output
	// 7g
	float duty_cycle = 0.5;
	TIM2->CCR1 = (uint32_t)(duty_cycle * (TIM2->ARR+1));
	// 7h
	TIM2->CR1 |= TIM_CR1_CEN; // enable timer
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
