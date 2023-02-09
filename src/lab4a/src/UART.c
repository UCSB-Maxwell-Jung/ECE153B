#include "UART.h"

void UART1_Init(void) {
	// [TODO]
}

void UART2_Init(void) {
	uint8_t SEL = 2;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;		//1.a enable USART2 clock in peripheral clk reg
	RCC->CCRIPR  |= 0b01 << (SEL * 1); 			//1.b select the sys clk as USART2 clk src 
}

void UART1_GPIO_Init(void) {
	unit8 pin_size = 2;
	GPIOA->OSPEEDR |=  0b11 << (pin_size * 2); 		//2.a PA2 set High speed
	GPIOA->OSPEEDR |=  0b11 << (pin_size * 3);		//	  PA3 set High speed

	pin_size = 1; 
	GPIOA->OTYPER |= 0b0 << (pin_size * 2);		//2.b	pins set to push-pull
	GPIOA->OTYPER |= 0b0 << (pin_size * 3);		

	pin_size = 2; 
	GPIOA->PUPDR |= 0b01 << (pin_size * 2);		//2.c  pins set using pull-up resistor
	GPIOA->PUPDR |= 0b01 << (pin_size * 3);		
}

void UART2_GPIO_Init(void) {
	// [TODO]
}

void USART_Init(USART_TypeDef* USARTx) {
	// [TODO]
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
