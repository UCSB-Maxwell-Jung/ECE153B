#ifndef __STM32L476R_NUCLEO_SPI_H
#define __STM32L476R_NUCLEO_SPI_H

#include "stm32l476xx.h"

#define CLK_FREQ 80000000
#define SPI_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16

class SPI {
public:
  // constructor
  SPI(SPI_TypeDef* SPIx);
  // methods
  virtual void begin(uint32_t desired_freq=SPI_DEFAULT_FREQ);
  void beginTransaction(void);
  void endTransaction(void);
  uint8_t transfer(uint8_t write_data);

protected:
  SPI_TypeDef* spix_;
  uint8_t br_;
  uint32_t desired_freq_;
  uint32_t actual_freq_;

  virtual void configure_GPIO(void) = 0; // must be implemented in derived class
  virtual void configure_SPI() = 0; // must be implemented in derived class
  // void init_SPI2(void);
  // void init_SPI2_GPIO(void);
};

#endif
