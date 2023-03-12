#include "hardware_usart2.h"

HardwareUsart2::HardwareUsart2()
    : UART(USART2) {}

// Configure PB3(SPI1_SCK), PB4(SPI1_MISO), PB5(SPI1_MOSI), PA4(SPI1_NSS)
void HardwareUsart2::configure_GPIO() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIO A

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

// Initialize SPI1 peripheral as master
void HardwareUsart2::configure_UART() {
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; // 1.a enable USART2 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0; // 1.b select the sys clk as USART2 clk src

	USARTx_->CR1 &= ~USART_CR1_UE; // disable USART before modifying regs

	USARTx_->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 3.a M1M0 = 00 = 1 start, 8 data bits, n stop bits
	USARTx_->CR1 &= ~USART_CR1_OVER8; // 0 = oversampling by 16
	USARTx_->CR2 &= ~USART_CR2_STOP; // 00 = 1 stop bit

	//3.b set USARTDIV in BRR[3:0] (*note: BRR[3:0] == USARTDIV[3:0] when USARTx->CR1 bit 16 (line 50) is 0)
	USARTx_->BRR &= ~0xFFFF; //clear [15:0] 
	USARTx_->BRR = (uint16_t) (CLK_FREQ/_baud_rate); // USARTDIV = f_clk/Baud Rate = 80Mhz/baud_rate

	//3.c enable transmitter and receiver 
	USARTx_->CR1 |= USART_CR1_TE; //enable transmitter
	USARTx_->CR1 |= USART_CR1_RE; //enable receiver
}
