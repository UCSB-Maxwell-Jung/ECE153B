#ifndef __STM32L476R_NUCLEO_UART_BLUETOOTH_H
#define __STM32L476R_NUCLEO_UART_BLUETOOTH_H

#include "UART.h"

// #define SPI_DISPLAY_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
// #define SPI_DISPLAY_MAX_FREQ 20000000 // 20Mhz

class UART_Bluetooth : public UART { // inherit more general SPI class
public:
  // constructor
  UART_Bluetooth();

private:
  void configure_GPIO(void);
  void configure_UART(void);
};

#endif // __STM32L476R_NUCLEO_UART_BLUETOOTH_H
