#ifndef __STM32L476R_NUCLEO_SPI_Display_H
#define __STM32L476R_NUCLEO_SPI_Display_H

#include "SPI.h"

class SPI_Display : public SPI { // inherit more general SPI class
public:
  // constructor
  SPI_Display(SPI_TypeDef* SPIx, uint32_t freq = 0);

private:
  void configure_GPIO(void);
  void configure_SPI(void);
};

#endif // __STM32L476R_NUCLEO_SPI1_H
