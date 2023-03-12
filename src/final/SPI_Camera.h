#ifndef __STM32L476R_NUCLEO_SPI_CAMERA_H
#define __STM32L476R_NUCLEO_SPI_CAMERA_H

#include "SPI.h"

#define SPI_DISPLAY_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16
#define SPI_DISPLAY_MAX_FREQ 20000000 // 20Mhz

class SPI_Camera : public SPI { // inherit more general SPI class
public:
  // constructor
  SPI_Camera();

private:
  void configure_GPIO(void);
  void configure_SPI(void);
};

#endif // __STM32L476R_NUCLEO_SPI1_H
