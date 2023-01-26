/*
 * ECE 153B - Winter 2021
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include "Button.h"

void Button_Init(void)
{
    // Enable GPIO Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    
    // GPIO Mode: Input(00), Output (01),
    // AF(10), Analog (11)
    GPIOC->MODER &= ~3U << 26; // Reset to 00
    GPIOC->MODER |= 0U << 26; // set to Input (00)

    // GPIO Push-Pull: No pull-up, pull-down (00),
    // Pull-up (01), Pull-down (10), Reserved (11)
    GPIOC->PUPDR &= ~3U << 26; // Reset to 00
    GPIOC->PUPDR |= 0U << 26; // set to no pull-up/down (00)
}
