#ifndef __STM32L476R_NUCLEO_SPI2_H
#define __STM32L476R_NUCLEO_SPI2_H

#include "SPI.h"

class HardwareSpi2 : public SPI { // inherit more general SPI class
public:
  // constructor
  HardwareSpi2();

  // methods
  void setCsLow();
  void setCsHigh();

private:
  void configureGpio(void);
  void configureSpi(void);
};

#endif // __STM32L476R_NUCLEO_SPI1_H
