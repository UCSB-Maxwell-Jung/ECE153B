/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 2A
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// GPIO Mode: Input(00), Output (01),
	// AF(10), Analog (11)
	GPIOA->MODER &= ~3U << 10; // reset to 00
	GPIOA->MODER |=  1U << 10; // set to 01

	// 2.3b Set the output type of PA 5 as Push-Pull
	GPIOA->OTYPER &= ~1U << 5; // reset to 0
	GPIOA->OTYPER |= 0u << 5; // set to 0

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~3U << 10; // reset to 00
	GPIOA->PUPDR |= 0U << 10; // set to 00

	// Initialize Green LED
	Green_LED_Off();
}

void update_LED(void) {
	GPIOA->ODR ^= 1UL<<5;
}

void Green_LED_Off(void) {
	GPIOA->ODR &= ~1U << 5;
}

void Green_LED_On(void) {
	GPIOA->ODR |= 1U << 5;
}

void Green_LED_Toggle(void) {
	GPIOA->ODR ^= 1U << 5;
}
