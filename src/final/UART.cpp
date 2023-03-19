#include "UART.h"
#define USART_TIMEOUT 0xfffff // some large value

UART::UART(USART_TypeDef* USARTx) : USARTx_(USARTx) {}

void UART::begin(uint32_t baud_rate) {
	baud_rate_ = baud_rate;
	configureGpio();
	configureUsart();
}

int UART::available(void) {
  return BufferSize; // [TODO] properly implement in the future
}

int UART::peek(void) {
  return -1; // [TODO] properly implement in the future
}

size_t UART::write(uint8_t c) {
	USARTx_->CR1 |= USART_CR1_UE; // Enable USART

	// send idle frame as first transmission by sending '0' pulse to TE
	USARTx_->CR1 &= ~USART_CR1_TE;
	// while ((_USARTx->ISR & USART_ISR_TEACK) == 0); // wait for idle frame to be sent
	USARTx_->CR1 |= USART_CR1_TE;

	while ((USARTx_->ISR & USART_ISR_TXE) == 0); // wait until TX empty
	USARTx_->TDR = c; // Writing USART_DR automatically clears the TXE flag 

	while ((USARTx_->ISR & USART_ISR_TC) == 0); // wait until Transmission Complete
	USARTx_->ICR |= USART_ICR_TCCF;  // clear TC by writting 1 to Transmission complete clear flag

	USARTx_->CR1 &= ~USART_CR1_UE; // Disable USART

	return 1;
}

size_t UART::write(const uint8_t *buffer, size_t size) {
	size_t n = 0;
	USARTx_->CR1 |= USART_CR1_UE; // Enable USART

	// send idle frame as first transmission by sending '0' pulse to TE
	USARTx_->CR1 &= ~USART_CR1_TE;
	// while ((_USARTx->ISR & USART_ISR_TEACK) == 0); // wait for idle frame to be sent
	USARTx_->CR1 |= USART_CR1_TE;

	while (size--) {
		while ((USARTx_->ISR & USART_ISR_TXE) == 0); // wait until TX empty
		USARTx_->TDR = *buffer++; // Writing USART_DR automatically clears the TXE flag 
		n++;
	}
	while ((USARTx_->ISR & USART_ISR_TC) == 0); // wait until Transmission Complete
	USARTx_->ICR |= USART_ICR_TCCF;  // clear TC by writting 1 to Transmission complete clear flag

	USARTx_->CR1 &= ~USART_CR1_UE; // Disable USART

	return n;
}

int UART::read(void) {
	uint32_t i = 0;
	USARTx_->CR1 |= USART_CR1_UE; // Enable USART
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while ((USARTx_->ISR & USART_ISR_RXNE) == 0) { // Wait until RXNE (RX not empty) bit is set
		if (i++ > USART_TIMEOUT) {
			return -1;
		}
	} 
	uint8_t received = USARTx_->RDR; // Reading USART_DR automatically clears the RXNE flag 
	USARTx_->CR1 &= ~USART_CR1_UE; // Disable USART

	return received;
}
