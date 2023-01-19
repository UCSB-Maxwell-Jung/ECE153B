/*
 * ECE 153B - Winter 2021
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Connect External Line to the GPIO
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// Rising trigger selection
	// 0 = trigger disabled, 1 = trigger enabled
	EXTI->FTSR1 |= EXTI_RTSR1_RT13;

	// Interrupt Mask Register
	// 0 = masked, 1 = not masked (enabled)
	EXTI->IMR1 |= EXTI_IMR1_IM13;

	NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable
	NVIC_SetPriority(EXTI15_10_IRQn, 0); // set interrupt to highest priority
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)

void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0) {
		// PC13 button is default HIGH for some reason
		update_LED();
		// Cleared flag by writing 1
 		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
}