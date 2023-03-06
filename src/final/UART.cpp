#include "UART.h"
#include <stdio.h>

UART::UART(USART_TypeDef* USARTx) : _USARTx(USARTx) {}

void UART::begin(uint32_t baud_rate) {
	_baud_rate = baud_rate;
	configure_GPIO();
	configure_UART();
}

void UART::print(char* str) {
	printf("%s", str);
}

void UART::println(char* str) {
	printf("%s\n", str);
}
