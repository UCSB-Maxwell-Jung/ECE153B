/*
    This file contains various hardware definitions, 
    constants, and comments describing hardware configuration
    specific to the digital_camera final project
*/

#ifndef __STM32L476R_NUCLEO_CAMERA_H
#define __STM32L476R_NUCLEO_CAMERA_H

#include <stddef.h>

#include "stm32l476xx.h"

#include "SysClock.h"
#include "SysTick.h"
#include "LED.h"
#include "hardware_usart2.h"
#include "hardware_usart1.h"

#define CLK_FREQ  80000000 // 80Mhz

extern HardwareUsart2 Serial;

typedef uint8_t byte;

#define BLUETOOTH USART1
#define WIRED USART2
#define ILI9341 SPI1

void setup(void);
void loop(void);

/*
    WIRED
    UART2_TX = PA2
    UART2_RX = PA3
*/

/*
    ili9341 pins
    VDD - 5V
    GND - GND
    CS = chip select = SPI1 NSS = PA4
    RST = reset = PA8
    DC = Data / Command = PA10
    SDI(MOSI) = Serial Data In = SPI1 MOSI = PB5
    SCK = Serial Clock = SPI1 SCK = PB3
    LED = 5V
    SDO(MISO) = Serial Data Out = SPI1 MISO = PB4


    OV264  pins 
    CS (chip select) = SPI2_NSS = PB12 
    SCK Serial Clk line = SPI2_SCK = PB13   
    SPI2_MISO = PB14 
    SPI2_MOSI = PB15 
    I2C1_SCL = PB6
    I2C1_SDA = PB7 

*/
#endif
