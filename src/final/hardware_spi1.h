#ifndef __STM32L476R_NUCLEO_SPI1_H
#define __STM32L476R_NUCLEO_SPI1_H

#include "SPI.h"

#define SPI1_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
#define SPI1_MAX_FREQ 20000000 // 20Mhz

class HardwareSpi1 : public SPI { // inherit more general SPI class
public:
  // constructor
  HardwareSpi1();

private:
  void configure_GPIO(void);
  void configure_SPI(void);
};

#endif // __STM32L476R_NUCLEO_SPI1_H
