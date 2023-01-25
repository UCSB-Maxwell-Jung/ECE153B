/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 2A
 */

#ifndef __STM32L476G_NUCLEO_EXTI_H
#define __STM32L476G_NUCLEO_EXTI_H

#include "stm32l476xx.h"

void EXTI_Init(void);
void EXTI15_10_IRQHandler(void);

#endif
