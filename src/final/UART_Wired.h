#ifndef __STM32L476R_NUCLEO_UART_Wired_H
#define __STM32L476R_NUCLEO_UART_Wired_H

#include "UART.h"

// #define SPI_DISPLAY_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
// #define SPI_DISPLAY_MAX_FREQ 20000000 // 20Mhz

class UART_Wired : public UART { // inherit more general SPI class
public:
  // constructor
  UART_Wired();

private:
  void configure_GPIO(void);
  void configure_UART(void);
};

#endif // __STM32L476R_NUCLEO_UART_Wired_H
