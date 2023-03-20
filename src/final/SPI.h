#ifndef __STM32L476R_NUCLEO_SPI_H
#define __STM32L476R_NUCLEO_SPI_H

#include "stm32l476xx.h"

#define CLK_FREQ 80000000
#define SPI_DEFAULT_FREQ 2500000 // 2.5Mhz equivalent to br=4 80Mhz/2^(5)
#define SPI_MAX_FREQ 40000000 // 4Mhz

class SPI {
public:
  // constructor
  SPI(SPI_TypeDef* SPIx);
  // methods
  virtual void begin(uint32_t desired_freq=SPI_DEFAULT_FREQ);
  void beginTransaction(void);
  void endTransaction(void);
  uint8_t transfer(uint8_t write_data);

  virtual void setCsLow(){}; // override in subclass for manual CS pin
  virtual void setCsHigh(){}; // override in subclass for manual CS pin

protected:
  SPI_TypeDef* spix_;
  uint8_t br_;
  uint32_t desired_freq_;
  uint32_t actual_freq_;

  virtual void configureGpio(void) = 0; // must be implemented in derived class
  virtual void configureSpi(void) = 0; // must be implemented in derived class
};

#endif
