#include "hardware_usart1.h"

HardwareUsart1::HardwareUsart1(void)
    : UART(USART1) {}

// Configure Pins for Bluetooth
void HardwareUsart1::configureGpio(void) {
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

// Initialize UART for Bluetooth
void HardwareUsart1::configureUsart(void) {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // enable USART1 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL; 
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0; // 1.b select the sys clk as USART1 clk src 

	USARTx_->CR1 &= ~USART_CR1_UE; // disable USART before modifying regs

	USARTx_->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 3.a M1M0 = 00 = 1 start, 8 data bits, n stop bits
	USARTx_->CR1 &= ~USART_CR1_OVER8; // 0 = oversampling by 16
	USARTx_->CR2 &= ~USART_CR2_STOP; // 00 = 1 stop bit

	//3.b set USARTDIV in BRR[3:0] (*note: BRR[3:0] == USARTDIV[3:0] when USARTx->CR1 bit 16 (line 50) is 0)
	USARTx_->BRR &= ~0xFFFF; //clear [15:0] 
	USARTx_->BRR = (uint16_t) (CLK_FREQ/baud_rate_); // USARTDIV = f_clk/Baud Rate = 80Mhz/baud_rate

	USARTx_->CR1 |= USART_CR1_RXNEIE; // trigger interrupt on data receive

	//3.c enable transmitter and receiver 
	USARTx_->CR1 |= USART_CR1_TE; //enable transmitter
	USARTx_->CR1 |= USART_CR1_RE; //enable receiver

	USARTx_->CR1 |= USART_CR1_UE; // Enable USART

	// NVIC_EnableIRQ(USART1_IRQn); // Enable USART1 interrupt
	// NVIC_SetPriority(USART1_IRQn, 1); // set interrupt to second highest priority
}

HardwareUsart1 Serial1; // Declare Serial1 as USART1 object
