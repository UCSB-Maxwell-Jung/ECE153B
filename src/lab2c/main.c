/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Baron Young, Maxwell Jung
 * Section: Wed 7pm
 * Lab: 2C
 */
 
#include "stm32l476xx.h"

#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

static char strTime[12] = {0};
static char strDate[12] = {0};

void RTC_Set_Alarm(void) {
	RTC->CR &= ~(RTC_CR_ALRAE | RTC_CR_ALRBE); // disables both alarms
	RTC_Disable_Write_Protection(); // remove RTC write protect
	RTC->CR &= ~(RTC_CR_ALRAE | RTC_CR_ALRBE); // clear alarm enable for A and B
	RTC->CR &= ~(RTC_CR_ALRAIE | RTC_CR_ALRBIE); // clear interrupt enable for A and B
	while (((RTC->ISR & RTC_ISR_ALRAWF) == 0) && ((RTC->ISR & RTC_ISR_ALRBWF) == 0)); // wait until

	RTC->ALRMAR &= ~(0xFFFFFFFF);
	RTC->ALRMAR |= 0x30; // set alarm A to 30 seconds
	RTC->ALRMAR |= (RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK4);
	RTC->ALRMAR &= ~RTC_ALRMAR_MSK1;
	RTC->ALRMBR &= ~(0xFFFFFFFF);
	// RTC->ALRMBR |= 0x01; // set alarm B to 1 second
	RTC->ALRMBR |= (RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK4 | RTC_ALRMBR_MSK1);

	RTC->CR |= (RTC_CR_ALRAIE | RTC_CR_ALRBIE); // clear interrupt enable for A and B

	RTC->CR |= (RTC_CR_ALRAE | RTC_CR_ALRBE); // disables both alarms

	RTC_Enable_Write_Protection();
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
	if ((EXTI->PR1 & EXTI_PR1_PIF18) != 0) // check interrupt flag
	{
		if (RTC->ISR & RTC_ISR_ALRAF) {
			// alarm A triggered
			// Green_LED_Toggle();
		}
		else if (RTC->ISR & RTC_ISR_ALRBF) {
			// alarm B triggered
			Green_LED_Toggle();
		}

		// Clear flags
		EXTI->PR1 |= EXTI_PR1_PIF18;
		RTC->ISR &= ~(RTC_ISR_ALRAF | RTC_ISR_ALRBF);
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
		Get_RTC_Calendar(strTime, strDate);
	}
}
