/*
    This file contains various hardware definitions, 
    constants, and comments describing hardware configuration
    specific to the digital_camera final project
*/

#ifndef __STM32L476R_NUCLEO_CAMERA_H
#define __STM32L476R_NUCLEO_CAMERA_H

#include "stm32l476xx.h"

#include <stddef.h>

// useful Arduino macros
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

typedef uint8_t byte; // alias for uint8_t

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


    OV264  pins 
    CS (chip select) = SPI2_NSS = PB12 
    SCK Serial Clk line = SPI2_SCK = PB13   
    SPI2_MISO = PB14 
    SPI2_MOSI = PB15 
    I2C1_SCL = PB6
    I2C1_SDA = PB7 

*/

//Only when using raspberry,enable it
//#define RASPBERRY_PI

//There are two steps you need to modify in this file before normal compilation
//Only ArduCAM Shield series platform need to select camera module, ArduCAM-Mini series platform doesn't

//Step 1: select the hardware platform, only one at a time
//#define OV2640_MINI_2MP
//#define OV3640_MINI_3MP
//#define OV5642_MINI_5MP
//#define OV5642_MINI_5MP_BIT_ROTATION_FIXED
#define OV2640_MINI_2MP_PLUS
//#define OV5642_MINI_5MP_PLUS
//#define OV5640_MINI_5MP_PLUS


//#define ARDUCAM_SHIELD_REVC	
//#define ARDUCAM_SHIELD_V2


//Step 2: Select one of the camera module, only one at a time
#if (defined(ARDUCAM_SHIELD_REVC) || defined(ARDUCAM_SHIELD_V2))
	//#define OV7660_CAM
	//#define OV7725_CAM
	//#define OV7670_CAM
	//#define OV7675_CAM
    //#define OV2640_CAM
	//#define OV3640_CAM
	//#define OV5642_CAM
	//#define OV5640_CAM 
	
	//#define MT9D111A_CAM
	//#define MT9D111B_CAM
	//#define MT9M112_CAM
	//#define MT9V111_CAM	
	//#define MT9M001_CAM	
	//#define MT9V034_CAM
	//#define MT9M034_CAM
	//#define MT9T112_CAM
	//#define MT9D112_CAM
#endif 

#endif