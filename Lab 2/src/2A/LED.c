/*
 * ECE 153B - Winter 2021
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	// Enable GPIO Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// GPIO Mode: Input(00), Output (01),
	// AF(10), Analog (11)
	GPIOA->MODER &= ~3U << 10; // 10 = pin 5 times 2

	GPIOA->OTYPER &= ~(0x00000020UL); // 2.3b Set the output type of PA 5 as Push-Pull

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~3U << 10; // reset
	GPIOA->PUPDR |= 0U << 10; // Pull down

	// Initialize Green LED
	// Turn on Green LED
	GPIOA->ODR |= 1UL << 5;
}

void Green_LED_Off(void) {
	// [TODO]
}

void Green_LED_On(void) {
	// [TODO]
}

void Green_LED_Toggle(void) {
	// [TODO]
}
