#include "I2C.h"

I2C::I2C(I2C_TypeDef* I2Cx)
	: I2Cx_(I2Cx) {}

void I2C::begin(uint8_t address) {
	configureGpio();
	configureI2c();
	I2Cx_->CR2 &= ~I2C_CR2_AUTOEND; // force manual STOP control
}

// transmit data to slave adress and return number of bytes transmitted
// see flowchart in figure 365 on pg 1124 of reference manual for implementation details
uint8_t I2C::transmit(uint8_t address, uint8_t data[], uint8_t quantity, uint8_t sendStop) {
	uint8_t bytes_transmitted;
	beginTransaction(address, quantity, 0); // 0 for write request
    bytes_transmitted = write(data, quantity);
	endTransaction(sendStop);

	return bytes_transmitted;
}

// receive data from slave adress and return number of bytes received
// see flowchart in figure 368 on pg 1128 of reference manual for implementation details
uint8_t I2C::receive(uint8_t address, uint8_t data[], uint8_t quantity, uint8_t sendStop) {
	size_t bytes_received;
	beginTransaction(address, quantity, 1); // 1 for read request
    bytes_received = read(data, quantity);
	endTransaction(sendStop);

	return bytes_received;
}

// rd_wrn = 0 = write request
// rd_wrn = 1 = read request
void I2C::beginTransaction(uint8_t address, uint8_t quantity, bool rd_wrn) {
	I2Cx_->CR1 |= I2C_CR1_PE; // enable I2C
	I2Cx_->CR2 &= ~I2C_CR2_SADD; // clear address
	I2Cx_->CR2 |= address << 1; // set address of targeted slave
	if (rd_wrn)
		I2Cx_->CR2 |=  I2C_CR2_RD_WRN; // request read transfer (1)
	else
		I2Cx_->CR2 &= ~I2C_CR2_RD_WRN; // request write transfer (0)

	I2Cx_->CR2 &= ~I2C_CR2_NBYTES;
	I2Cx_->CR2 |= quantity << 16; // set number of bytes to transfer

	I2Cx_->CR2 |= I2C_CR2_START; // start I2C
}


uint8_t I2C::endTransaction(uint8_t sendStop) {
	if (sendStop == 0) { // RESTART
		I2Cx_->CR2 |= I2C_CR2_RELOAD; // reload number of bytes to transfer
		I2Cx_->CR2 |= I2C_CR2_START; // generate (RE)START
		return 0;
	}

	I2Cx_->CR2 |= I2C_CR2_STOP;  // generate STOP after current byte transfer
	while((I2Cx_->ISR & I2C_ISR_STOPF) == 0); // Wait until STOP flag is set
	I2Cx_->CR1 &= ~I2C_CR1_PE; // disable I2C

	return 0;
}

size_t I2C::read(uint8_t *data, size_t quantity) {
	size_t bytes_received;
    for (bytes_received = 0; bytes_received < quantity; bytes_received++) {
		while ((I2Cx_->ISR & I2C_ISR_RXNE) == 0) { // wait for receive to complete
			if ((I2Cx_->ISR & I2C_ISR_NACKF) != 0)
				return bytes_received; // exit if receive is incomplete due to NACK
		}
		data[bytes_received] = I2Cx_->RXDR;  // save received data
	}

	return bytes_received; // this value should match quantity if everything worked
}

size_t I2C::write(const uint8_t *data, size_t quantity) {
	size_t bytes_transmitted;
	// // wait for any previous data to finish transferring
	// while ((I2Cx_->ISR & I2C_ISR_TXIS) == 0) {
	// 	if ((I2Cx_->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
	// 		return -1; // address not transferred properly
	// }
    for (bytes_transmitted = 0; bytes_transmitted < quantity; bytes_transmitted++) {
		I2Cx_->TXDR = data[bytes_transmitted];  // queue next byte to transmit
		while ((I2Cx_->ISR & I2C_ISR_TXIS) == 0) { // wait for transmit to complete
			if ((I2Cx_->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
				return bytes_transmitted; // exit if transmit is incomplete due to NACK
		}
	}

	return bytes_transmitted; // this value should match quantity if everything worked
}
