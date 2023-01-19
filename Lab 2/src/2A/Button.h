/*
 * ECE 153B - Winter 2021
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#ifndef __STM32L476G_NUCLEO_BUTTON_H
#define __STM32L476G_NUCLEO_BUTTON_H

#include "stm32l476xx.h"

enum button_state {pressed, released};
static enum button_state btn;

void Button_Init(void);

#endif
