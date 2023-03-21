#include "hardware_spi1.h"

HardwareSpi1::HardwareSpi1(void)
    : SPI(SPI1) {}

// Configure PB3(SPI1_SCK), PB4(SPI1_MISO), PB5(SPI1_MOSI), PA4(SPI1_NSS)
void HardwareSpi1::configureGpio(void) {
	uint8_t af_num;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIOA

	// set to alternate function mode (10)
	// explanation: default is 11, so use mask &~(01) to set second bit to 0
	GPIOB->MODER &= ~(GPIO_MODER_MODER3_0 | // PB3
					  GPIO_MODER_MODER4_0 | // PB4
					  GPIO_MODER_MODER5_0); // PB5
	GPIOA->MODER &= ~(GPIO_MODER_MODER4_0); // PA4

	// reset AF for PA4, PB3, PB4, PB5
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL3 | // PB3
	                   GPIO_AFRL_AFSEL4 | // PB4
	                   GPIO_AFRL_AFSEL5); // PB5
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL4); // PA4


	// set PB3, PB4, PB5, PA4 to AF5
	af_num = 5;
	GPIOB->AFR[0] |= (af_num << (3*4)); // PB3
	GPIOB->AFR[0] |= (af_num << (4*4)); // PB4
	GPIOB->AFR[0] |= (af_num << (5*4)); // PB5
	GPIOA->AFR[0] |= (af_num << (4*4)); // PA4

	// set to push-pull (0)
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT3 | 
	                   GPIO_OTYPER_OT4 | 
	                   GPIO_OTYPER_OT5);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4);

	// set to very high (11)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR3 | 
	                   GPIO_OSPEEDER_OSPEEDR4 | 
	                   GPIO_OSPEEDER_OSPEEDR5);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4);
	
	// set to no pull-up, pull-down (00)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3 | 
	                  GPIO_PUPDR_PUPD4 | 
	                  GPIO_PUPDR_PUPD5);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4);
}

// Initialize SPI1 peripheral as master
void HardwareSpi1::configureSpi(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1 clock
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; // set, then reset to clear SPI1
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	
	SPI1->CR1 &= ~SPI_CR1_SPE; // disable SPI1

	// ---------------CR1-----------------
	// set spi clock prescalar to be the next highest requested freq
	SPI1->CR1 &= ~SPI_CR1_BR; // reset baud rate control bits to 000
	SPI1->CR1 |= br_ << 3;

	// set clock polarity to low (0)
	SPI1->CR1 &= ~SPI_CR1_CPOL;
	// set clock phase to first edge (0)
	SPI1->CR1 &= ~SPI_CR1_CPHA;

	// set to full-duplex (0)
	SPI1->CR1 &= ~SPI_CR1_RXONLY;
	// set to 2-line unidirectional data mode (0)
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
	// disable output in bidirectional mode (0)
	SPI1->CR1 &= ~SPI_CR1_BIDIOE;

	// MSB first (0)
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	
	// disable SSM (Hardware NSS management) (0)
	SPI1->CR1 &= ~SPI_CR1_SSM;

	// set to master (1)
	SPI1->CR1 |= SPI_CR1_MSTR;

	// ----------------CR2--------------
	// reset data size bits to 0000;
	SPI1->CR2 &= ~SPI_CR2_DS;
	// set data size to 8 (strangely, 8 corresponds to 0111 in the datasheet)
	SPI1->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);

	// set NSS output enable (1)
	SPI1->CR2 |= SPI_CR2_SSOE;

	// use motorola SPI mode (0)
	SPI1->CR2 &= ~SPI_CR2_FRF; 
	
	// set FIFO threshold to 1/4 (8-bit) (1)
	SPI1->CR2 |= SPI_CR2_FRXTH;

	// disable hardware CRC calculation (0)
	SPI1->CR1 &= ~SPI_CR1_CRCEN;

	// disable NSS pulse generation (0)
	SPI1->CR2 &= ~SPI_CR2_NSSP;
}

void HardwareSpi1::configureDma(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // enable DMA1 controller

	DMA1_Channel2->CCR &= ~DMA_CCR_EN; // deactivate channel

	// DMA1_Channel2->CNDTR = ??; 

	// DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
	DMA1_Channel2->CPAR = (uint32_t)&SPI2->DR;
	// DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA1_Channel2->CCR &= ~DMA_CCR_DIR; // read from peripheral
	// DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA1_Channel2->CCR &= ~DMA_CCR_PINC; // disable peripheral increment mode
	// DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA1_Channel2->CCR |= DMA_CCR_MINC; // enable memory increment mode
	// DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA1_Channel2->CCR &= ~DMA_CCR_PSIZE; // set peripheral size to 8-bits
	// DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA1_Channel2->CCR &= ~DMA_CCR_MSIZE; // set memory size to 8-bits
	// DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA1_Channel2->CCR &= ~DMA_CCR_CIRC; // disable circular mode
	// DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA1_Channel2->CCR &= ~DMA_CCR_PL; // reset channel priority level
	DMA1_Channel2->CCR |= DMA_CCR_PL_1; // set channel priority level to high (10)
	// DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA1_Channel2->CCR &= ~DMA_CCR_MEM2MEM; // disable memory to memory
	// DMA_Init(PB_SPI_RX_DMA_Channel, &DMA_InitStructure);
	DMA1_Channel2->CCR |= DMA_CCR_EN; // activate channel

	// NVIC_InitStructure.NVIC_IRQChannel = PB_SPI_DMA_IRQ;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(DMA1_Channel2_IRQn); // Enable
	NVIC_SetPriority(DMA1_Channel2_IRQn, 0); // set interrupt to highest priority


	// DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	// DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	// DMA_ITConfig(PB_SPI_RX_DMA_Channel, DMA_IT_TC, ENABLE);

	DMA1_Channel3->CCR &= ~DMA_CCR_EN; // deactivate channel
	// DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
	DMA1_Channel3->CPAR = (uint32_t)&SPI2->DR;

	// DMA1_Channel2->CNDTR = ??; 
	
	// DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA1_Channel3->CCR |= DMA_CCR_DIR; // read from memory
	// DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA1_Channel3->CCR &= ~DMA_CCR_PINC; // disable peripheral increment mode
	// DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA1_Channel3->CCR |= DMA_CCR_MINC; // enable memory increment mode
	// DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA1_Channel3->CCR &= ~DMA_CCR_PSIZE; // set peripheral size to 8-bits
	// DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA1_Channel3->CCR &= ~DMA_CCR_MSIZE; // set memory size to 8-bits
	// DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA1_Channel3->CCR &= ~DMA_CCR_CIRC; // disable circular mode
	// DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA1_Channel3->CCR &= ~DMA_CCR_PL; // reset channel priority level
	DMA1_Channel3->CCR |= DMA_CCR_PL_1; // set channel priority level to high (10)
	// DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA1_Channel3->CCR &= ~DMA_CCR_MEM2MEM; // disable memory to memory
	// DMA_Init(PB_SPI_RX_DMA_Channel, &DMA_InitStructure);
	DMA1_Channel3->CCR |= DMA_CCR_EN; // activate channel

	// NVIC_InitStructure.NVIC_IRQChannel = PB_SPI_DMA_IRQ;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(DMA1_Channel3_IRQn); // Enable
	NVIC_SetPriority(DMA1_Channel3_IRQn, 0); // set interrupt to highest priority
}