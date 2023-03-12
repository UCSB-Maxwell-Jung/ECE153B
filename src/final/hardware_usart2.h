#ifndef __STM32L476R_NUCLEO_USART2_H
#define __STM32L476R_NUCLEO_USART2_H

#include "UART.h"

// #define SPI_DISPLAY_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
// #define SPI_DISPLAY_MAX_FREQ 20000000 // 20Mhz

class HardwareUsart2 : public UART { // inherit more general SPI class
public:
  // constructor
  HardwareUsart2();

private:
  void configure_GPIO(void);
  void configure_UART(void);
};

extern HardwareUsart2 Serial;

#endif // __STM32L476R_NUCLEO_UART_Wired_H
