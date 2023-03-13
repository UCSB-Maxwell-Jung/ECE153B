#ifndef __STM32L476R_NUCLEO_UART_H
#define __STM32L476R_NUCLEO_UART_H

#include "stm32l476xx.h"

#include "Print.h"

#define BufferSize 32
#define UART_DEFAULT_BAUD_RATE 9600
#define CLK_FREQ 80000000

class UART : public Print {
public:
  // constructor
  UART(USART_TypeDef* USARTx);
  // methods
  void begin(uint32_t baud_rate=UART_DEFAULT_BAUD_RATE);
  using Print::write;
  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  uint8_t read(void);

protected:
  USART_TypeDef* USARTx_;
  uint32_t baud_rate_;

  virtual void configureGpio(void) = 0; // must be implemented in derived class
  virtual void configureUsart(void) = 0; // must be implemented in derived class
};

// void USART1_IRQHandler(void);
// void USART2_IRQHandler(void);

#endif
