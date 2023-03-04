#ifndef __STM32L476R_NUCLEO_SYSTICK_H
#define __STM32L476R_NUCLEO_SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l476xx.h"

void init_SysTick(void);
void SysTick_Handler(void);
void delay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L476R_NUCLEO_SYSTICK_H */
