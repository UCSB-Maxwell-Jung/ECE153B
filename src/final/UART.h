#ifndef __STM32L476R_NUCLEO_UART_H
#define __STM32L476R_NUCLEO_UART_H

#include "stm32l476xx.h"

#define BufferSize 32

class UART {
protected:
  USART_TypeDef* _USARTx;

  void init_UART1(void);
  void init_UART2(void);

  void init_UART1_GPIO(void);
  void init_UART2_GPIO(void);

public:
  // constructor
  UART(USART_TypeDef* USARTx);
  // methods
  void begin(uint32_t baud_rate);
  void print(char* str);
  void println(char* str);
};

// void USART1_IRQHandler(void);
// void USART2_IRQHandler(void);

#endif
