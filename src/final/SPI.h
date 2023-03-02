#ifndef __STM32L476R_NUCLEO_SPI_H
#define __STM32L476R_NUCLEO_SPI_H

#include "device.h"
#include "stm32l476xx.h"

#include <stddef.h>

class SPI {
public:
  // constructor
  SPI(SPI_TypeDef* SPIx=NULL);
  // methods
  void begin(uint32_t freq);
  uint8_t transfer(uint8_t write_data);

protected:
  SPI_TypeDef* _SPIx;

  void init_SPI1(uint32_t freq);
  void init_SPI2(void);

  void init_SPI1_GPIO(void);
  void init_SPI2_GPIO(void);
};

#endif
