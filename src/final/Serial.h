#ifndef __STM32L476R_NUCLEO_SERIAL_H
#define __STM32L476R_NUCLEO_SERIAL_H

#include "stm32l476xx.h"

#define BufferSize 32

#define BLUETOOTH USART1
#define TERMITE USART2

class Serial {
protected:
  static void init_UART1(void);
  static void init_UART2(void);

  static void init_UART1_GPIO(void);
  static void init_UART2_GPIO(void);

public:
  static USART_TypeDef* _USARTx;

  // methods
  static void begin(uint32_t baud_rate);
  static void print(char* str);
  static void println(char* str);
};

uint8_t serial_read();
void serial_write(uint8_t *buffer, uint32_t nBytes);
void serial_delay(uint32_t us);

// void USART1_IRQHandler(void);
// void USART2_IRQHandler(void);

#endif
