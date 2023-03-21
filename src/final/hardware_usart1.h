#ifndef __STM32L476R_NUCLEO_USART1_H
#define __STM32L476R_NUCLEO_USART1_H

#include "UART.h"

class HardwareUsart1 : public UART { // inherit more general UART class
public:
  // constructor
  HardwareUsart1(void);

private:
  void configureGpio(void);
  void configureUsart(void);
};

extern HardwareUsart1 Serial1;

#endif // __STM32L476R_NUCLEO_USART1_H
