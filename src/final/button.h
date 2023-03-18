/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 2A
 */

#ifndef __STM32L476G_NUCLEO_BUTTON_H
#define __STM32L476G_NUCLEO_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

void initButton(void);
void EXTI15_10_IRQHandler(void);
void USART1_IRQHandler(void);

#ifdef __cplusplus
}
#endif


#endif
