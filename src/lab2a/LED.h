/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 2A
 */

#ifndef __STM32L476G_NUCLEO_LED_H
#define __STM32L476G_NUCLEO_LED_H

#include "stm32l476xx.h"

void LED_Init(void);

void update_LED(void);
void Green_LED_Off(void);
void Green_LED_On(void);
void Green_LED_Toggle(void);

#endif
