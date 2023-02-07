/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 3B
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void Input_Capture_Setup() {
	// Enable GPIO Port A Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Enable TIM1 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM1EN;

	// Configure PA5
	uint8_t pin_number = 9;
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
	GPIOA->AFR[0] |= 0b0001 << (pin_number * 4);  // set to AF1 (TIM1_CH2)

	// Configure PWM Output for TIM1 CH 2
	// 7a
	TIM1->CR1 &= ~TIM_CR1_DIR; // set counter direction: upcounter (up = 0, down = 1)
	// 7b
	TIM1->PSC = 15; // set prescalar value
				   // Explanation: downscale 4 Mhz clock input to 1 Mhz by dividing by PSC + 1: 4 MHz/(3+1) = 1 Mhz
	// 7c
	TIM1->ARR = 0xFFFF; // set auto-reload value
					 // Explanation: reset timer every 1ms (999+1 ticks @ 1us/tick)
	// below 2 lines might be unnecessary
	TIM1->CCMR1 &= ~TIM_CCMR1_CC1S; // reset capture/compare 1 selection
	TIM1->CCMR1 |= 0b00;			// configure CC1 channel as output
	// 7d
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;						  // clear output compare mode bits
	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // set output compare mode bits to PWM mode 1 (0110)
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;						  // enable output preload

	// 8.
	TIM1->BDTR |= (0b1 << 15 | 0b1 << 11 | 0b1 << 10); // enable main output mode, off-state selection for run mode/idle mode
	// 9.
	TIM1->EGR |= TIM_EGR_UG; // Enable update generation in the event generation register

	// 10.
	TIM1->DIER |= TIM_DIER_UIE;



	//
	TIM1->CCER &= ~TIM_CCER_CC1P; // Set the output polarity for compare 1 to active high (high = 0, low = 1)
	//
	TIM1->CCER |= TIM_CCER_CC1E; // Enable the channel 1 output
	// 
	TIM1->CCR1 = 10; // 10us = 10 ticks @ 1Mhz
	// 
	TIM1->CR1 |= TIM_CR1_CEN; // enable timer
}

void TIM4_IRQHandler(void) {
	// [TODO]
}

void Trigger_Setup() {
	// [TODO]
}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();

	
	while(1) {
		// [TODO] Store your measurements on Stack
		
	}
}
