#ifndef __STM32L476R_NUCLEO_SPI3_H
#define __STM32L476R_NUCLEO_SPI3_H

#include "SPI.h"

class HardwareSpi3 : public SPI { // inherit more general SPI class
public:
  // constructor
  HardwareSpi3();

  // methods
  void setCsLow();
  void setCsHigh();

private:
  void configureGpio(void);
  void configureSpi(void);
};

#endif // __STM32L476R_NUCLEO_SPI1_H
