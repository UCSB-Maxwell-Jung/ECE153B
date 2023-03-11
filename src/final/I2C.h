#ifndef __STM32L476R_NUCLEO_I2C_H
#define __STM32L476R_NUCLEO_I2C_H

#include "nucleo.h"

#define READ_FROM_SLAVE 1
#define WRITE_TO_SLAVE  0

#define I2C_TIMINGR_PRESC_POS	28
#define I2C_TIMINGR_SCLDEL_POS	20
#define I2C_TIMINGR_SDADEL_POS	16
#define I2C_TIMINGR_SCLH_POS	8
#define I2C_TIMINGR_SCLL_POS	0

#define I2C_DEFAULT_FREQ 5000000 // equivalent to 80Mhz/16

class I2C {
public:
  // constructor
  I2C(I2C_TypeDef* I2Cx=NULL);
  // methods
  virtual void begin(uint32_t desired_freq=I2C_DEFAULT_FREQ);
  int8_t beginTransmission(uint32_t DevAddress, uint8_t Size, uint8_t Direction);
  int8_t write(uint8_t DeviceAddress, uint8_t *pData, uint8_t Size);
  int8_t read(uint8_t DeviceAddress, uint8_t *pData, uint8_t Size);
  uint8_t endTransmission();
  uint8_t requestFrom(uint8_t address, uint8_t quantity);
  uint8_t available();

protected:
  I2C_TypeDef* _I2Cx;

  virtual void configure_GPIO(void) = 0; // must be implemented in derived class
  virtual void configure_I2C() = 0; // must be implemented in derived class
};

#endif
