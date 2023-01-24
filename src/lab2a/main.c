/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 2A
 */

#include "stm32l476xx.h"
#include "EXTI.h"
#include "LED.h"
#include "Button.h"
#include "SysClock.h"

int main(void) {
	Initialization
	System_Clock_Init();
	LED_Init();
	Button_Init();
	EXTI_Init();
	
	while(1);
	
	return 0;
}
