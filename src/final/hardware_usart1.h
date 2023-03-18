#ifndef __STM32L476R_NUCLEO_USART1_H
#define __STM32L476R_NUCLEO_USART1_H

#include "UART.h"

// #define SPI_DISPLAY_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
// #define SPI_DISPLAY_MAX_FREQ 20000000 // 20Mhz

class HardwareUsart1 : public UART { // inherit more general SPI class
public:
  // constructor
  HardwareUsart1(void);

private:
  void configureGpio(void);
  void configureUsart(void);
};

extern HardwareUsart1 camera;

#endif // __STM32L476R_NUCLEO_USART1_H
