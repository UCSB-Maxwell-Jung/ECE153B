#ifndef __STM32L476R_NUCLEO_I2C_CAMERA_H
#define __STM32L476R_NUCLEO_I2C_CAMERA_H

#include "I2C.h"

// #define I2C_Camera_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
// #define I2C_Camera_MAX_FREQ 20000000 // 20Mhz

class I2C_Camera : public I2C { // inherit more general I2C class
public:
  // constructor
  I2C_Camera();

private:
  void configure_GPIO(void);
  void configure_I2C(void);
};

#endif // __STM32L476R_NUCLEO_I2C1_H
