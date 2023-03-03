#ifndef __STM32L476R_NUCLEO_SYSTICK_H
#define __STM32L476R_NUCLEO_SYSTICK_H

#include "stm32l476xx.h"

void init_SysTick(void);
void SysTick_Handler(void);
void delay(uint32_t T);
uint32_t micros(void);

#endif /* __STM32L476R_NUCLEO_SYSTICK_H */
