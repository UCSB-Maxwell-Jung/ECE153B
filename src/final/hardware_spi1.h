#ifndef __STM32L476R_NUCLEO_SPI1_H
#define __STM32L476R_NUCLEO_SPI1_H

#include "SPI.h"

class HardwareSpi1 : public SPI { // inherit more general SPI class
public:
  // constructor
  HardwareSpi1();

private:
  void configure_GPIO(void);
  void configure_SPI(void);
};

#endif // __STM32L476R_NUCLEO_SPI1_H
