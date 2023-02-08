#include "SPI.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

void SPI2_GPIO_Init(void) {
	// TODO: initialize SPI2 GPIO pins
}

void SPI2_Init(void){
	// TODO: initialize SPI2 peripheral
}
 
void SPI_Transfer_Byte(SPI_TypeDef* SPIx, uint8_t write_data, uint8_t* read_data) {
	// TODO: perform SPI transfer
}
