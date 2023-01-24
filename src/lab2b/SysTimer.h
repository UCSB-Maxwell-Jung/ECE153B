/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Baron Young, Maxwell Jung
 * Section: Wed 7pm
 * Lab: 2B
 */

#ifndef __STM32L476G_NUCLEO_SYSTICK_H
#define __STM32L476G_NUCLEO_SYSTICK_H

#include "stm32l476xx.h"

void SysTick_Init(void);
void SysTick_Handler(void);

void delay(uint32_t T);

#endif
