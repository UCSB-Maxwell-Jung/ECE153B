#include "I2C.h"

I2C::I2C(I2C_TypeDef* I2Cx)
	: _I2Cx(I2Cx) {}

void I2C::begin(uint8_t address) {
	configure_GPIO();
	configure_I2C();
}

uint8_t I2C::requestFrom(uint8_t address, uint8_t quantity, bool stop) {
	// [TODO]

	return 0;
}

void I2C::beginTransmission(uint8_t address) {	
	// [TODO]
}

uint8_t I2C::endTransmission(bool stop) {
	// [TODO]
}

int8_t I2C::write(uint8_t data[], uint8_t length) {
	// [TODO]

	return 0;
}

uint8_t I2C::available() {
	// [TODO]
	
	return 0;
}

int8_t I2C::read() {
	// [TODO]

	return 0xff;
}
