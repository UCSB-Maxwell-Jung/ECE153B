#ifndef __STM32L476R_NUCLEO_UART_H
#define __STM32L476R_NUCLEO_UART_H

#include "Print.h"

#include "stm32l476xx.h"

#define BufferSize 32
#define UART_DEFAULT_BAUD_RATE 9600

class UART : public Print {
public:
  // constructor
  UART(USART_TypeDef* USARTx);
  // methods
  void begin(uint32_t baud_rate=UART_DEFAULT_BAUD_RATE);
  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  uint8_t read();

protected:
  USART_TypeDef* _USARTx;
  uint32_t _baud_rate;

  virtual void configure_GPIO(void) = 0; // must be implemented in derived class
  virtual void configure_UART() = 0; // must be implemented in derived class
};

// void USART1_IRQHandler(void);
// void USART2_IRQHandler(void);

#endif
