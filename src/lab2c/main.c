/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 2C
 */
 
#include "stm32l476xx.h"

#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

char strTime[12] = {0};
char strDate[12] = {0};

void RTC_Set_Alarm(void) {

}

void RTC_Alarm_Enable(void) {
	// Connect External Line to the GPIO
	// RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13; // reset to 0
	// SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// Rising trigger selection
	// 0 = trigger disabled, 1 = trigger enabled
	EXTI->RTSR1 |= EXTI_RTSR1_RT18;
	EXTI->RTSR2 |= EXTI_RTSR1_RT18;

	// Interrupt Mask Register
	// 0 = masked, 1 = not masked (enabled)
	EXTI->IMR1 |= EXTI_IMR1_IM18;
	EXTI->IMR2 |= EXTI_IMR1_IM18;

	// event mask
	EXTI->EMR1 |= EXTI_EMR1_EM18;
	EXTI->EMR2 |= EXTI_EMR1_EM18;

	EXTI->PR1 |= EXTI_PR1_PIF18;
	EXTI->PR2 |= EXTI_PR1_PIF18;

	NVIC_EnableIRQ(RTC_Alarm_IRQn);		 // Enable
	NVIC_SetPriority(RTC_Alarm_IRQn, 0); // set interrupt to highest priority
}

void RTC_Alarm_IRQHandler(void) {
	// Clear interrupt pending bit
	if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0)
	{
		// PC13 button is default HIGH for some reason
		Green_LED_Toggle();
		// Cleared flag by writing 1
		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
}

int main(void) {	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	LED_Init();
	
	RTC_Init();
	RTC_Alarm_Enable();
	RTC_Set_Alarm();
	
	while(1) {
		// [TODO]
	}
}
