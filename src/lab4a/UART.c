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

void UART1_GPIO_Init(void) {
	uint8_t pin_size = 2;
	GPIOA->OSPEEDR &=  ~0b11 << (pin_size * 2); 
	GPIOA->OSPEEDR |=  0b11 << (pin_size * 2); 		// 2.a PA2 set High speed
	
	GPIOA->OSPEEDR &=  ~0b11 << (pin_size * 3);
	GPIOA->OSPEEDR |=  0b11 << (pin_size * 3);		// 2.a PA3 set High speed

	pin_size = 1; 
	GPIOA->OTYPER &= ~0b0 << (pin_size * 2);
	GPIOA->OTYPER |= 0b0 << (pin_size * 2);			// 2.b pins set to push-pull
	
	GPIOA->OTYPER &= ~0b0 << (pin_size * 3);		
	GPIOA->OTYPER |= 0b0 << (pin_size * 3);			// 2.b pins set to push-pull

	pin_size = 2; 
	GPIOA->PUPDR &= ~0b01 << (pin_size * 2);
	GPIOA->PUPDR |= 0b01 << (pin_size * 2);			// 2.c  pins set using pull-up resistor
	
	GPIOA->PUPDR &= ~0b01 << (pin_size * 3);		
	GPIOA->PUPDR |= 0b01 << (pin_size * 3);			// 2.c  pins set using pull-up resistor
}

void UART2_GPIO_Init(void) {
	// [TODO]
}

void USART_Init(USART_TypeDef* USARTx) {
	uint8_t SEL_size = 1;
	USARTx->CR1 |= 0b1 << (SEL_size * 0); // USART diabled before modifying regs

	//3.a 1 start, 8 data bits, n stop bits
	USARTx->CR1 |= 0b0 << (SEL_size * 28); // M1
	USARTx->CR1 |= 0b0 << (SEL_size * 12); // M0
	
	//oversamle mode by 16
	USARTx->CR1 |= 0b0 <<(SEL_size * 15);	

	//3.b set USARTDIV in BRR[3:0] (*note: BRR[3:0] == USARTDIV[3:0] when USARTx->CR1 bit 16 (line 50) is 0)
	USARTx->BRR &= ~0xFFFF; //clear [3:0] 
	USARTx->BRR |= ~0x208D;	//BaudRate = f_clk/(USARTDIV) = 8333.333 ~ 8333 = 0x208D

	//3.c enable transmitter and receiver 
	USARTx->CR3 &= ~0b1 << (SEL_size * 6);	//enable transmitter
	USARTx->CR3 |= 	0b1 << (SEL_size * 6);

	USARTx->CR3 &= ~0b1 << (SEL_size * 5);	//enable receiver
	USARTx->CR3 |=  0b1 << (SEL_size * 5);

	USARTx->CR1 |= 0b0 << (SEL_size * 0); // USART diabled before modifying regs
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
