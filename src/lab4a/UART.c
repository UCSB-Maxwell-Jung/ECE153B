#include "UART.h"

void UART1_Init(void) {
	// [TODO]
}

void UART2_Init(void) {
	uint8_t SEL_size = 2;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;			// 1.a enable USART2 clock in peripheral clk reg
	RCC->CCIPR  &= ~0b01 << (SEL_size * 1); 	
	RCC->CCIPR  |= 0b01 << (SEL_size * 1); 			// 1.b select the sys clk as USART2 clk src 
}

void configure_PA(int pin_number) {
	uint8_t width = 2;
	GPIOA->OSPEEDR &=  ~0b11 << (width * pin_number); 
	GPIOA->OSPEEDR |=  0b11 << (width * pin_number); 		// 2.a pins set to High speed

	width = 1; 
	GPIOA->OTYPER &= ~0b0 << (width * pin_number);
	GPIOA->OTYPER |= 0b0 << (width * pin_number);			// 2.b pins set to push-pull
	
	width = 2; 
	GPIOA->PUPDR &= ~0b01 << (width * pin_number);
	GPIOA->PUPDR |= 0b01 << (width * pin_number);			// 2.c pins set to pull-up resistor
}

void UART1_GPIO_Init(void) {
	// [TODO]
}

void UART2_GPIO_Init(void) {
	configure_PA(2);
	configure_PA(3);
}

void USART_Init(USART_TypeDef* USARTx) {
	uint8_t SEL_size = 1;
	USARTx->CR1 &= ~USART_CR1_UE; 					// disable USART before modifying regs

	USARTx->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); 	// 3.a M1M0 = 00 = 1 start, 8 data bits, n stop bits
	USARTx->CR1 &= ~USART_CR1_OVER8;				// 0 = oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;					// 00 = 1 stop bit

	//3.b set USARTDIV in BRR[3:0] (*note: BRR[3:0] == USARTDIV[3:0] when USARTx->CR1 bit 16 (line 50) is 0)
	USARTx->BRR &= ~0xFFFF; //clear [15:0] 
	USARTx->BRR |= ~0x208D;	//BaudRate = f_clk/(USARTDIV) = 8333.333 ~ 8333 = 0x208D

	//3.c enable transmitter and receiver 
	USARTx->CR1 |= USART_CR1_TE;					//enable transmitter
	USARTx->CR1 |= USART_CR1_RE;					//enable receiver

	USARTx->CR1 |= USART_CR1_UE; 					// EnablUSART diabled 
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
