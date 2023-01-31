/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 3A
 */

#include "stm32l476xx.h"
 
void PWM_Init() {
	// Initialize Green LED
	// Green_LED_Off();
	Green_LED_Off();

	// Enable GPIO Port A Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Enable TIM2 Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

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
	GPIOA->AFR[0] |= ~0b0001 << (pin_number * 4); // set to AF1 (TIM2_CH1)

	// Configure PWM Output for TIM2 CH 1
	TIM2->CR1 |= TIM_CR1_CEN; // enable timer
	TIM2->CR1 &= ~TIM_CR1_DIR; // set counter direction: upcounter (up = 0, down = 1)
	TIM2->PSC = 3; // set prescalar value: 4 Mhz/(3 + 1) = 1 Mhz (1 tick/us)
	TIM2->ARR = 1000; // set auto-reload value: reset timer every 1000 ticks @ 1us/tick = 1000us = 1ms
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
