/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include "stm32l476xx.h"
#include "EXTI.h"
#include "LED.h"
#include "Button.h"
#include "SysClock.h"

int main() {
	// Initialization
	System_Clock_Init();
	LED_Init();
	Button_Init();
	EXTI_Init();
	
	while(1);
	
	return 0;
}