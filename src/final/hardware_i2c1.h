#ifndef __STM32L476R_NUCLEO_I2C1_H
#define __STM32L476R_NUCLEO_I2C1_H

#include "I2C.h"

class HardwareI2c1 : public I2C { // inherit more general I2C class
public:
  // constructor
  HardwareI2c1();

private:
  void configureGpio(void);
  void configureI2c(void);
};

#endif // __STM32L476R_NUCLEO_I2C1_H
