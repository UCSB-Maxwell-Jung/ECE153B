#ifndef __STM32L476R_NUCLEO_SPI_H
#define __STM32L476R_NUCLEO_SPI_H

#include "camera.h"
#include "stm32l476xx.h"

#include <stddef.h>

#define SPI_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16

class SPI {
public:
  // constructor
  SPI(SPI_TypeDef* SPIx=NULL);
  // methods
  virtual void begin(uint32_t desired_freq=SPI_DEFAULT_FREQ);
  void enable(void);
  void disable(void);
  void transmit(uint8_t write_data);
  uint8_t transmit_receive(uint8_t write_data);

protected:
  SPI_TypeDef* _SPIx;
  uint8_t _br;
  uint32_t _desired_freq;
  uint32_t _actual_freq;

  virtual void configure_GPIO(void) = 0; // must be implemented in derived class
  virtual void configure_SPI() = 0; // must be implemented in derived class
  // void init_SPI2(void);
  // void init_SPI2_GPIO(void);
};

#endif
