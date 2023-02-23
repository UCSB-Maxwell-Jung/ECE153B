/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 5A
 */


#include "ADC.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

uint32_t measurement;
double vin;
double vref = 3.3;

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();

    // [TODO] Initialize PWM

    while (1) {
        // Trigger ADC and get result
        ADC1->CR |= ADC_CR_ADSTART; // start regular conversion
        while (ADC123_COMMON->CSR & ADC_CSR_EOC_MST == 0); // wait until ADC conversion is complete
        measurement = ADC1->DR; // ADC outputs value in range [0,4095)

        vin = vref*measurement/4096.0; // measurement/4096 = vin/vref
        // [TODO] LED behavior based on ADC result
    }
}
