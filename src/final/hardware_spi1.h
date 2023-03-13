#ifndef __STM32L476R_NUCLEO_SPI1_H
#define __STM32L476R_NUCLEO_SPI1_H

#include "SPI.h"

class HardwareSpi1 : public SPI { // inherit more general SPI class
public:
  // constructor
  HardwareSpi1(void);

private:
  void configureGpio(void);
  void configureSpi(void);
};

#endif // __STM32L476R_NUCLEO_SPI1_H
