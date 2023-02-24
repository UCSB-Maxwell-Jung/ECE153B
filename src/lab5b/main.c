/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 5B
 */


#include "ADC.h"
#include "DAC.h"
#include "Button.h"
#include "EXTI.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

uint32_t data;
double voltage;
double vref = 3.3;

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 8 MHz

    ADC_Init();
    DAC_Init();
	Button_Init();
    EXTI_Init();

    // Initialize PWM
    // PWM_Init();

    while (1) {
        // Start Regular Conversion
		ADC1->CR |= ADC_CR_ADSTART;
		
		// Wait for ADC conversion
		while((ADC1->ISR & ADC_ISR_EOC) == 0);
		
		// Ready ADC data register
		data = ADC1->DR;
		voltage = data/4095.0*vref;
		
		set_LED_voltage(voltage, vref); // set led brightness according to vin and vref
    }
}
