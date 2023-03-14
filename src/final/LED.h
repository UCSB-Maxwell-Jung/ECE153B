#ifndef __STM32L476R_NUCLEO_LED_H
#define __STM32L476R_NUCLEO_LED_H

#include "nucleo.h"

void initLed(void);

void ledOff(void);
void ledOn(void);
void ledToggle(void);

#endif
