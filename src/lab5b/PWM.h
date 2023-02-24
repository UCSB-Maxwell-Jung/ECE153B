#ifndef __STM32L476G_DISCOVERY_PWM_H
#define __STM32L476G_DISCOVERY_PWM_H

#include "stm32l476xx.h"

void PWM_Init();
void set_LED_voltage(double vin, double maxVoltage);

#endif /* __STM32L476G_DISCOVERY_PWM_H */