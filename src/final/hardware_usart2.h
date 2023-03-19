#ifndef __STM32L476R_NUCLEO_USART2_H
#define __STM32L476R_NUCLEO_USART2_H

#include "UART.h"

// #define SPI_DISPLAY_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
// #define SPI_DISPLAY_MAX_FREQ 20000000 // 20Mhz

class HardwareUsart2 : public UART { // inherit more general SPI class
public:
  // constructor
  HardwareUsart2(void);

private:
  void configureGpio(void);
  void configureUsart(void);
};

extern HardwareUsart2 console;

#endif // __STM32L476R_NUCLEO_UART_Wired_H
