/*
    This file contains various hardware definitions, 
    constants, and comments describing hardware configuration
    specific to the digital_camera final project
*/

#ifndef __STM32L476R_NUCLEO_CAMERA_H
#define __STM32L476R_NUCLEO_CAMERA_H

#include "stm32l476xx.h"

#include <stddef.h>

#define CLK_FREQ  80000000 // 80Mhz

#define BLUETOOTH USART1
#define WIRED USART2
#define ILI9341 SPI1


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
*/

#endif