#include "UART.h"
#include <stdio.h>

UART::UART(USART_TypeDef* USARTx) : _USARTx(USARTx) {}

void UART::init_UART1(void) {
	// part a 2.3 (step 1)
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // enable USART1 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL; // 1.b select the sys clk as USART1 clk src 
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART::init_UART2(void) {
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; // 1.a enable USART2 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0; // 1.b select the sys clk as USART2 clk src 
}

void UART::init_UART1_GPIO(void) {
	// part a 2.3 (step 2)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled

	//pins set to High speed 
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6;	//6
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;	//7

	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;			//pins set to push-pull
	
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD6_0) | (GPIO_PUPDR_PUPD7_0);			//pins set to pull-up resistor
	
	//MODER//
	GPIOB->MODER &= ~GPIO_MODER_MODE6;	//pin 6
	GPIOB->MODER |= GPIO_MODER_MODE6_1; 
	
	GPIOB->MODER &= ~GPIO_MODER_MODE7;	//pin 7
	GPIOB->MODER |= GPIO_MODER_MODE7_1;							
	
	//set to AF//
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;		//pin 6
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_0);

	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7;		//pin 7
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL7_2 | GPIO_AFRL_AFSEL7_1 | GPIO_AFRL_AFSEL7_0);
}

void UART::init_UART2_GPIO(void) {
	//part a 
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //clk enabled

	// 2.a pins set to High speed//
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;	//pin2
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;	//pin3
	
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;			// 2.b pins set to push-pull
	
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPD2_0) | (GPIO_PUPDR_PUPD3_0);			// 2.c pins set to pull-up resistor
	
	//MODER//
	GPIOA->MODER &= ~GPIO_MODER_MODE2;	//pin 2
	GPIOA->MODER |= GPIO_MODER_MODE2_1; 
	
	GPIOA->MODER &= ~GPIO_MODER_MODE3;	//pin 3
	GPIOA->MODER |= GPIO_MODER_MODE3_1;							
	
	//set to AF//
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;		//pin 2
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_2 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_0);

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;		//pin 3
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_2 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_0);
}

void UART::begin(uint32_t baud_rate) {
	if (_USARTx == USART1) {
		init_UART1_GPIO();
		init_UART1();
	}
	else if (_USARTx == USART2) {
		init_UART2_GPIO();
		init_UART2();
	}
	else {
		return; // do nothing
	}

	_USARTx->CR1 &= ~USART_CR1_UE; // disable USART before modifying regs

	_USARTx->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 3.a M1M0 = 00 = 1 start, 8 data bits, n stop bits
	_USARTx->CR1 &= ~USART_CR1_OVER8; // 0 = oversampling by 16
	_USARTx->CR2 &= ~USART_CR2_STOP; // 00 = 1 stop bit

	//3.b set USARTDIV in BRR[3:0] (*note: BRR[3:0] == USARTDIV[3:0] when USARTx->CR1 bit 16 (line 50) is 0)
	_USARTx->BRR &= ~0xFFFF; //clear [15:0] 
	_USARTx->BRR = (uint16_t) (80*1000000/baud_rate); // USARTDIV = f_clk/Baud Rate = 80Mhz/baud_rate

	//3.c enable transmitter and receiver 
	_USARTx->CR1 |= USART_CR1_TE; //enable transmitter
	_USARTx->CR1 |= USART_CR1_RE; //enable receiver

	_USARTx->CR1 |= USART_CR1_UE; // Enable USART 
}

void UART::print(char* str) {
	printf("%s", str);
}

void UART::println(char* str) {
	printf("%s\n", str);
}