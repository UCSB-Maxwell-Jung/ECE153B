#include "UART_Bluetooth.h"
#include "camera.h"

UART_Bluetooth::UART_Bluetooth()
    : UART(BLUETOOTH) {}

// Configure Pins for Bluetooth
void UART_Bluetooth::configure_GPIO() {
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
void UART_Bluetooth::configure_UART() {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // enable USART1 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL; 
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0; // 1.b select the sys clk as USART1 clk src 

	_USARTx->CR1 &= ~USART_CR1_UE; // disable USART before modifying regs

	_USARTx->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 3.a M1M0 = 00 = 1 start, 8 data bits, n stop bits
	_USARTx->CR1 &= ~USART_CR1_OVER8; // 0 = oversampling by 16
	_USARTx->CR2 &= ~USART_CR2_STOP; // 00 = 1 stop bit

	//3.b set USARTDIV in BRR[3:0] (*note: BRR[3:0] == USARTDIV[3:0] when USARTx->CR1 bit 16 (line 50) is 0)
	_USARTx->BRR &= ~0xFFFF; //clear [15:0] 
	_USARTx->BRR = (uint16_t) (CLK_FREQ/_baud_rate); // USARTDIV = f_clk/Baud Rate = 80Mhz/baud_rate

	//3.c enable transmitter and receiver 
	_USARTx->CR1 |= USART_CR1_TE; //enable transmitter
	_USARTx->CR1 |= USART_CR1_RE; //enable receiver

	_USARTx->CR1 |= USART_CR1_UE; // Enable USART
}
