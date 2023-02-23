#include "PWM.h"

void PWM_Init() {
	// Enable GPIO Port A Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Enable TIM2 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// Configure PA5 (on-board LED)
	// GPIO Mode: Input(00), Output (01),
	// AF(10), Analog (11)
	GPIOA->MODER &= ~GPIO_MODER_MODE5; // reset to 00
	GPIOA->MODER |= GPIO_MODER_MODE5_1;	// set to AF

	// GPIO Output Speed: Low (00), Medium (01), 
	// High (10), Very High (11)
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5; // reset to 00
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;  // set to Very High

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5; // set to No Pull-Up, No Pull-Down

	// Write alternate function 1 (Timer 2 Channel 1) into AFR
	GPIOA->AFR[0] &= GPIO_AFRL_AFRL5; // reset to 0000
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0; // set to AF1 (TIM2_CH1)

	// Configure PWM Output for TIM2 CH1
	// 7a
	TIM2->CR1 &= ~TIM_CR1_DIR; // set counter direction: upcounter (up = 0, down = 1)
	// 7b
	TIM2->PSC = 7; // set prescalar value
				   // Explanation: downscale 8 Mhz clock input to 1 Mhz by dividing by PSC + 1: 8 MHz/(7+1) = 1 Mhz
	// 7c
	TIM2->ARR = 999; // set auto-reload value
					  // Explanation: reset timer every 1ms (999+1 ticks @ 1us/tick)
	// below 2 lines might be unnecessary
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S; // reset capture/compare 1 selection
	TIM2->CCMR1 |= 0b00;			// configure CC1 channel as output
	// 7d
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M; // clear output compare mode bits
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // set output compare mode bits to PWM mode 1 (0110)
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // enable output preload
	// 7e
	TIM2->CCER &= ~TIM_CCER_CC1P; // Set the output polarity for compare 1 to active high (high = 0, low = 1)
	// 7f
	TIM2->CCER |= TIM_CCER_CC1E; // Enable the channel 1 output
	// 7g
	float duty_cycle = 0.5; // initialize with half duty cycle
	TIM2->CCR1 = (uint32_t)(duty_cycle * (TIM2->ARR+1));
	// 7h
	TIM2->CR1 |= TIM_CR1_CEN; // enable timer
}

void set_LED_voltage(double vin, double maxVoltage) {
	TIM2->CCR1 = (uint32_t)(vin/maxVoltage * (TIM2->ARR + 1));
}
