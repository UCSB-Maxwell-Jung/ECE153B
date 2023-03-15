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

class I2C {
public:
  // constructor
  I2C(I2C_TypeDef* I2Cx=NULL);
  // methods
  virtual void begin(uint8_t address=0);
  uint8_t transmit(uint8_t address, uint8_t data[], uint8_t quantity, uint8_t sendStop=true);
  uint8_t receive(uint8_t address, uint8_t data[], uint8_t quantity, uint8_t sendStop=true);

protected:
  I2C_TypeDef* I2Cx_;

  virtual void configureGpio(void) = 0; // must be implemented in derived class
  virtual void configureI2c(void) = 0; // must be implemented in derived class

private:
  void beginTransaction(uint8_t address, uint8_t quantity, bool rd_wrn);
  uint8_t endTransaction(uint8_t sendStop);

  size_t read(uint8_t *data, size_t quantity);
  size_t write(const uint8_t *data, size_t quantity);
};

#endif
