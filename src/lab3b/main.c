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
	// Configure PA9
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;					// Enable GPIO Port A Clock
	uint8_t pin_number = 9;
	// GPIO Mode: Input(00), Output (01),
	// AF(10), Analog (11)
	GPIOA->MODER &= ~0b11U << (pin_number * 2);				// reset to 00
	GPIOA->MODER |= 0b10U << (pin_number * 2);				// set to AF

	// GPIO Output Speed: Low (00), Medium (01),
	// High (10), Very High (11)
	GPIOA->OSPEEDR &= ~0b11U << (pin_number * 2); 			// reset to 00
	GPIOA->OSPEEDR |= 0b11u << (pin_number * 2);  			// set to Very High

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~0b11U << (pin_number * 2); 			// reset to 00
	GPIOA->PUPDR |= 0b00U << (pin_number * 2);				// set to No Pull-Up, No Pull-Down

	// Write alternate function 1 (Timer 2 Channel 1) into AFR
	GPIOA->AFR[1] &= ~0b1111 << ((pin_number-8) * 4); 			// reset to 0000
	GPIOA->AFR[1] |= 0b0001 << ((pin_number-8) * 4);  			// set to AF1 (TIM1_CH2)

	// Configure PWM Output for TIM1 CH2
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;						// 2. Enable TIM1 Clock
	TIM1->PSC = 15;											// 3. Set prescalar value to 15
				   											// Explanation: downscale 4 Mhz clock input to 1 Mhz by dividing by PSC + 1: 4 MHz/(3+1) = 1 Mhz

	TIM1->CR1 |= TIM_CR1_ARPE;								// 4. Enable auto reload preload
	TIM1->ARR = 0xFFFF; 									// 4. Set auto-reload value to max value

	TIM1->CCR2 = 10;										// 5. Set the CCR value to 10
															// Explanation: 10 ticks @ 1 Mhz = 10 us

	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;						 	// 6. clear output compare mode bits
	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);	// 6. set output compare mode bits to PWM mode 1 (0110)
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;						 	// 6. enable output preload

	TIM1->CCER |= TIM_CCER_CC2E; 							// 7. Enable channel 2 output

	TIM1->BDTR |= (TIM_BDTR_MOE|TIM_BDTR_OSSR|TIM_BDTR_OSSI);// 8. enable main output mode, off-state selection for run mode/idle mode
	
	TIM1->EGR |= TIM_EGR_UG;								// 9. Enable update generation in the event generation register

	TIM1->DIER |= TIM_DIER_UIE;								// 10. Enable update interrupt in the DMA/Interrupt enable registe

	TIM1->SR = 0; 											// 10. clear the update interrupt flag in the status register

	TIM1->CR1 &= ~TIM_CR1_DIR;								// 11. Set the direction of the counter as up counting (0)

	TIM1->CCER &= ~TIM_CCER_CC2P;							// Set the output polarity for compare 2 to active high (high = 0, low = 1)

	TIM1->CR1 |= TIM_CR1_CEN; 								// 11. enable timer

	/**____Input_Capture___ for TIM4 CH1**/
	pin_number = 6; //PIN 6

	RCC->AHB2ENR = RCC_AHB2ENR_GPIOBEN;		 				// 1.a Enable GPIO Port B Clock
	// GPIO Mode: Input(00), Output (01),
	// AF(10), Analog (11)
	GPIOB->MODER &= ~0b11U << (pin_number * 2);				// 1.b reset mode
	GPIOB->MODER |= 0b10U << (pin_number * 2);				// set to AF

	GPIOB->AFR[1] &= ~0b1111 << ((pin_number) * 4); 		// reset to 0000
	GPIOB->AFR[1] |= 0b0010 << ((pin_number) * 4);  		// AF2 (TIM4_CH1)

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR &= ~0b11U << (pin_number * 2);  			// reset
	GPIOB->PUPDR |= 0b00U << (pin_number * 2);				// 1.c Set PB6 to no pull-up, no pull-down.

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;					// 2. Enable Timer 4 in RCC APB1ENRx

	TIM4->PSC = 15;											// 3. Set prescalar value to 15
				   											// Explanation: downscale 4 Mhz clock input to 1 Mhz by dividing by PSC + 1: 4 MHz/(3+1) = 1 Mhz

	TIM4->CR1 |= TIM_CR1_ARPE;								// 4. Enable auto reload preload
	TIM4->ARR = 0xFFFF; 									// 4. Set auto-reload value to max value

	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;						 	// 5. reset last 2 bits
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;						// 5. set last 2 bits to 01 (input, IC1 mapped on TI1)

	TIM4->CCER |= (TIM_CCER_CC1NP|TIM_CCER_CC1P);			// 6. set bit 3,1 to high (11) for capturing both rising/falling edges
	TIM4->CCER |= TIM_CCER_CC1E;							// 6. Enable capturing

	TIM4->DIER |= (TIM_DIER_UDE|TIM_DIER_CC1IE|TIM_DIER_UIE);// 7. enable DMA requests, interrupt for channel 1, update interrupt

	TIM4->EGR |= TIM_EGR_UG;								// 8. Enable update generation

	TIM4->SR &= ~TIM_SR_UIF;								// 9. Clear the update interrupt flag

	TIM4->CR1 &= ~TIM_CR1_DIR;								// 10. Set direction as upcounting (up = 0, down =1)
	
	TIM4->CR1 |= TIM_CR1_CEN; 								// 10. enable timer

	NVIC_EnableIRQ(TIM4_IRQn);								// 11. Enable Timer 4 interrupt
	NVIC_SetPriority(TIM4_IRQn, 2); 						// 11. set interrupt priority to 2
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
