#ifndef __STM32L476R_NUCLEO_USART2_H
#define __STM32L476R_NUCLEO_USART2_H

#include "UART.h"

class HardwareUsart2 : public UART { // inherit more general UART class
public:
  // constructor
  HardwareUsart2(void);

private:
  void configureGpio(void);
  void configureUsart(void);
};

extern HardwareUsart2 Serial;

#endif // __STM32L476R_NUCLEO_UART_Wired_H
