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

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();

    // [TODO] Initialize PWM

    while (1) {
        // [TODO] Trigger ADC and get result

        // [TODO] LED behavior based on ADC result
    }
}
