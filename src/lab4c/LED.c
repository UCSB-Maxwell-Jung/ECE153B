/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 4C
 */

#include "LED.h"

void LED_Init(void)
{
	// Enable GPIO Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// set to output mode (01)
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
}

void Green_LED_Off(void)
{
	GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void Green_LED_On(void)
{
	GPIOA->ODR |= GPIO_ODR_OD5;
}

void Green_LED_Toggle(void)
{
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
