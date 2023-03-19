/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 2A
 */

#include "Button.h"
#include "stm32l476xx.h"
#include "camera_interface.h"

void initButton(void)
{
    // Enable GPIO Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    
    // GPIO Mode: Input(00), Output (01),
    // AF(10), Analog (11)
    GPIOC->MODER &= ~GPIO_MODER_MODER13; // Reset to 00

    // GPIO Push-Pull: No pull-up, pull-down (00),
    // Pull-up (01), Pull-down (10), Reserved (11)
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13; // Reset to 00

	// Connect External Line to the GPIO
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_PC;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// Rising trigger selection
	// 0 = trigger disabled, 1 = trigger enabled
	EXTI->FTSR1 |= EXTI_RTSR1_RT13;

	// Interrupt Mask Register
	// 0 = masked, 1 = not masked (enabled)
	EXTI->IMR1 |= EXTI_IMR1_IM13;

	NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable
	NVIC_SetPriority(EXTI15_10_IRQn, 1); // set interrupt to highest priority
}

void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0) {
		capturePhoto();
		// Cleared flag by writing 1
 		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
}

// void USART1_IRQHandler(void) {
// 	// Clear interrupt pending bit
// 	if ((USART1->ISR & USART_ISR_RXNE) != 0) {
// 		saveCameraByte();
// 	}
// }
