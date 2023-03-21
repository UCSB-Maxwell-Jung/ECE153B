#include "LED.h"

#include "stm32l476xx.h"

void initLed(void)
{
	// Enable GPIO Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	//GPIO mode for green LED
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;

	ledOff();
}

void ledOff(void)
{
	GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void ledOn(void)
{
	GPIOA->ODR |= GPIO_ODR_OD5;
}

void toggleLed(void)
{
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
