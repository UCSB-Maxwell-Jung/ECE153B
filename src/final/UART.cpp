#include "UART.h"
#include <stdio.h>

UART::UART(USART_TypeDef* USARTx) : _USARTx(USARTx) {}

void UART::begin(uint32_t baud_rate) {
	_baud_rate = baud_rate;
	configure_GPIO();
	configure_UART();
}

size_t UART::write(uint8_t c) {
	_USARTx->CR1 |= USART_CR1_UE; // Enable USART

	// send idle frame as first transmission by sending '0' pulse to TE
	_USARTx->CR1 &= ~USART_CR1_TE;
	// while ((_USARTx->ISR & USART_ISR_TEACK) == 0); // wait for idle frame to be sent
	_USARTx->CR1 |= USART_CR1_TE;

	while ((_USARTx->ISR & USART_ISR_TXE) == 0); // wait until TX empty
	_USARTx->TDR = c; // Writing USART_DR automatically clears the TXE flag 

	while ((_USARTx->ISR & USART_ISR_TC) == 0); // wait until Transmission Complete
	_USARTx->ICR |= USART_ICR_TCCF;  // clear TC by writting 1 to Transmission complete clear flag

	_USARTx->CR1 &= ~USART_CR1_UE; // Disable USART

	return 1;
}

size_t UART::write(const uint8_t *buffer, size_t size) {
	size_t n = 0;
	_USARTx->CR1 |= USART_CR1_UE; // Enable USART

	// send idle frame as first transmission by sending '0' pulse to TE
	_USARTx->CR1 &= ~USART_CR1_TE;
	// while ((_USARTx->ISR & USART_ISR_TEACK) == 0); // wait for idle frame to be sent
	_USARTx->CR1 |= USART_CR1_TE;

	while (size--) {
		while ((_USARTx->ISR & USART_ISR_TXE) == 0); // wait until TX empty
		_USARTx->TDR = *buffer++; // Writing USART_DR automatically clears the TXE flag 
		n++;
	}
	while ((_USARTx->ISR & USART_ISR_TC) == 0); // wait until Transmission Complete
	_USARTx->ICR |= USART_ICR_TCCF;  // clear TC by writting 1 to Transmission complete clear flag

	_USARTx->CR1 &= ~USART_CR1_UE; // Disable USART

	return n;
}

uint8_t UART::read() {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(_USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(_USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}
