/*
  ArduCAM.cpp - Arduino library support for CMOS Image Sensor
  Copyright (C)2011-2015 ArduCAM.com. All right reserved

  Basic functionality of this library are based on the demo-code provided by
  ArduCAM.com. You can find the latest version of the library at
  http://www.ArduCAM.com

  Now supported controllers:
    - OV7670
    - MT9D111
    - OV7675
    - OV2640
    - OV3640
    - OV5642
    - OV7660
    - OV7725
    - MT9M112
    - MT9V111
    - OV5640
    - MT9M001
    - MT9T112
    - MT9D112

  We will add support for many other sensors in next release.

  Supported MCU platform
    - Theoretically support all Arduino families
      - Arduino UNO R3      (Tested)
      - Arduino MEGA2560 R3   (Tested)
      - Arduino Leonardo R3   (Tested)
      - Arduino Nano      (Tested)
      - Arduino DUE       (Tested)
      - Arduino Yun       (Tested)
      - Raspberry Pi      (Tested)
      - ESP8266-12        (Tested)

  If you make any modifications or improvements to the code, I would appreciate
  that you share the code with me so that I might include it in the next release.
  I can be contacted through http://www.ArduCAM.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*------------------------------------
  Revision History:
  2012/09/20  V1.0.0  by Lee  first release
  2012/10/23  V1.0.1  by Lee  Resolved some timing issue for the Read/Write Register
  2012/11/29  V1.1.0  by Lee  Add support for MT9D111 sensor
  2012/12/13  V1.2.0  by Lee  Add support for OV7675 sensor
  2012/12/28  V1.3.0  by Lee  Add support for OV2640,OV3640,OV5642 sensors
  2013/02/26  V2.0.0  by Lee  New Rev.B shield hardware, add support for FIFO control
                              and support Mega1280/2560 boards
  2013/05/28  V2.1.0  by Lee  Add support all drawing functions derived from UTFT library
  2013/08/24  V3.0.0  by Lee  Support ArudCAM shield Rev.C hardware, features SPI interface and low power mode.
                Support almost all series of Arduino boards including DUE.
  2014/02/06  V3.0.1  by Lee  Minor change to the library, fixed some bugs, add self test code to the sketches for easy debugging.
  2014/03/09  V3.1.0  by Lee  Add the more impressive example sketches.
                Optimise the OV5642 settings, improve image quality.
                Add live preview before JPEG capture.
                Add play back photos one by one after BMP capture.
  2014/05/01  V3.1.1  by Lee  Minor changes to add support Arduino IDE for linux distributions.
  2014/09/30  V3.2.0  by Lee  Improvement on OV5642 camera dirver.
  2014/10/06  V3.3.0  by Lee  Add OV7660,OV7725 camera support.
  2015/02/27  V3.4.0  by Lee  Add the support for Arduino Yun board, update the latest UTFT library for ArduCAM.
  2015/06/09  V3.4.1  by Lee  Minor changes and add some comments
  2015/06/19  V3.4.2  by Lee  Add support for MT9M112 camera.
  2015/06/20  V3.4.3  by Lee  Add support for MT9V111 camera.
  2015/06/22  V3.4.4  by Lee  Add support for OV5640 camera.
  2015/06/22  V3.4.5  by Lee  Add support for MT9M001 camera.
  2015/08/05  V3.4.6  by Lee  Add support for MT9T112 camera.
  2015/08/08  V3.4.7  by Lee  Add support for MT9D112 camera.
  2015/09/20  V3.4.8  by Lee  Add support for ESP8266 processor.
  2016/02/03  V3.4.9  by Lee  Add support for Arduino ZERO board.
  2016/06/07  V3.5.0  by Lee  Add support for OV5642_CAM_BIT_ROTATION_FIXED.
  2016/06/14  V3.5.1  by Lee  Add support for ArduCAM-Mini-5MP-Plus OV5640_CAM.
  2016/09/29  V3.5.2  by Lee  Optimize the OV5642 register settings
	2016/10/05	V4.0.0	by Lee	Add support for second generation hardware platforms like ArduCAM shield V2, ArduCAM-Mini-5MP-Plus(OV5642/OV5640).	  
  2016/10/28  V4.0.1  by Lee	Add support for Raspberry Pi
  2017/04/27  V4.1.0  by Lee	Add support for OV2640/OV5640/OV5642 functions.
  2017/07/07  V4.1.0  by Lee	Add support for ArduCAM_ESP32 paltform
  2017/07/25  V4.1.1  by Lee	Add support for MT9V034
  --------------------------------------*/
#include "ArduCAM.h"
#include "SysTick.h"
// #if defined ( RASPBERRY_PI )
// 	#include <string.h>
// 	#include <time.h>
// 	#include <stdio.h>
// 	#include <stdlib.h>
// 	#include <stdint.h>
// 	#include <unistd.h>
// 	#include <wiringPiI2C.h>
// 	#include <wiringPi.h>
// 	#include "ArduCAM.h"
// 	#include "arducam_arch_raspberrypi.h"
// #else
	// #include "Arduino.h
	// #include <Wire.h> // I2C
	// #include <SPI.h> // SPI
	// #include "HardwareSerial.h" // UART
// 	#if defined(__SAM3X8E__)
// 		#define Wire  Wire1
// 	#endif
// #endif

// default constructor
ArduCAM::ArduCAM(byte sensor_model) {
	sensor_model_ = sensor_model;
	switch (sensor_model_) {
		case OV7660:
		case OV7670:
		case OV7675:
		case OV7725:
			sensor_addr_ = 0x42;
			break;
		case MT9D111_A: //Standard MT9D111 module
      		sensor_addr_ = 0xba;
			break;
		case MT9D111_B: //Flex MT9D111 AF module
			sensor_addr_ = 0x90;
    		break;
		case MT9M112:
			sensor_addr_ = 0x90;
			break;
		case MT9M001:
			sensor_addr_ = 0xba;
			break;
		case MT9V034:
			sensor_addr_ = 0x90;
			break;
		case MT9M034:
			sensor_addr_ = 0x20;// 7 bits
			break;
		case OV3640:
		case OV5640:
		case OV5642:
		case MT9T112:
		case MT9D112:
			sensor_addr_ = 0x78;
			break;
		case OV2640:
		case OV9650:
		case OV9655:
			sensor_addr_ = 0x60;
			break;
		default:
			sensor_addr_ = 0x42;
			break;
		}
}

void ArduCAM::begin(uint32_t freq) {
	i2c_.begin();
	spi_.begin(freq); // 8 Mhz
}

void ArduCAM::InitCAM() {
	wrSensorReg8_8(0xff, 0x01);
	wrSensorReg8_8(0x12, 0x80);
	delay(100);
	if (m_fmt_ == JPEG) {
		wrSensorRegs8_8(OV2640_JPEG_INIT);
		wrSensorRegs8_8(OV2640_YUV422);
		wrSensorRegs8_8(OV2640_JPEG);
		wrSensorReg8_8(0xff, 0x01);
		wrSensorReg8_8(0x15, 0x00);
		wrSensorRegs8_8(OV2640_320x240_JPEG);
	}
	else {
		wrSensorRegs8_8(OV2640_QVGA);
	}
}

void ArduCAM::flush_fifo(void) {
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

void ArduCAM::start_capture(void) {
	write_reg(ARDUCHIP_FIFO, FIFO_START_MASK);
}

void ArduCAM::clear_fifo_flag(void ) {
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

uint32_t ArduCAM::read_fifo_length(void) {
	uint32_t len1,len2,len3,length=0;
	len1 = read_reg(FIFO_SIZE1);
	len2 = read_reg(FIFO_SIZE2);
	len3 = read_reg(FIFO_SIZE3) & 0x7f;
	length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return length;	
}

void ArduCAM::set_fifo_burst() {
	spi_.transfer(BURST_FIFO_READ);
}

void ArduCAM::CS_HIGH(void) {
	spi_.setCsHigh();
}
void ArduCAM::CS_LOW(void) {
	spi_.setCsLow();
}

uint8_t ArduCAM::read_fifo(void) {
	uint8_t data;
	data = bus_read(SINGLE_FIFO_READ);
	return data;
}

uint8_t ArduCAM::read_reg(uint8_t addr) {
	return bus_read(addr & 0x7F);
}

void ArduCAM::write_reg(uint8_t addr, uint8_t data) {
	bus_write(addr | 0x80, data);
}

//Set corresponding bit  
void ArduCAM::set_bit(uint8_t addr, uint8_t bit) {
	uint8_t temp;
	temp = read_reg(addr);
	write_reg(addr, temp | bit);
}

//Clear corresponding bit 
void ArduCAM::clear_bit(uint8_t addr, uint8_t bit) {
	uint8_t temp;
	temp = read_reg(addr);
	write_reg(addr, temp & (~bit));
}

//Get corresponding bit status
uint8_t ArduCAM::get_bit(uint8_t addr, uint8_t bit) {
  uint8_t temp;
  temp = read_reg(addr);
  temp = temp & bit;
  return temp;
}

//Set ArduCAM working mode
//MCU2LCD_MODE: MCU writes the LCD screen GRAM
//CAM2LCD_MODE: Camera takes control of the LCD screen
//LCD2MCU_MODE: MCU read the LCD screen GRAM
void ArduCAM::set_mode(uint8_t mode) {
  switch (mode) {
    case MCU2LCD_MODE:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
      break;
    case CAM2LCD_MODE:
      write_reg(ARDUCHIP_MODE, CAM2LCD_MODE);
      break;
    case LCD2MCU_MODE:
      write_reg(ARDUCHIP_MODE, LCD2MCU_MODE);
      break;
    default:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
      break;
  }
}

uint8_t ArduCAM::bus_write(int address,int value) {
	CS_LOW();
	spi_.beginTransaction();
	spi_.transfer(address);
	spi_.transfer(value);
	spi_.endTransaction();
	CS_HIGH();
	return 1;
}

uint8_t ArduCAM:: bus_read(int address) {
	uint8_t value;
	CS_LOW();
	spi_.beginTransaction();
	spi_.transfer(address);
	value = spi_.transfer(0x00);
	spi_.endTransaction();
	CS_HIGH();
	return value;
}

void ArduCAM::OV2640_set_JPEG_size(uint8_t size)
{
	switch(size) {
		case OV2640_160x120:
			wrSensorRegs8_8(OV2640_160x120_JPEG);
			break;
		case OV2640_176x144:
			wrSensorRegs8_8(OV2640_176x144_JPEG);
			break;
		case OV2640_320x240:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
		case OV2640_352x288:
	  		wrSensorRegs8_8(OV2640_352x288_JPEG);
			break;
		case OV2640_640x480:
			wrSensorRegs8_8(OV2640_640x480_JPEG);
			break;
		case OV2640_800x600:
			wrSensorRegs8_8(OV2640_800x600_JPEG);
			break;
		case OV2640_1024x768:
			wrSensorRegs8_8(OV2640_1024x768_JPEG);
			break;
		case OV2640_1280x1024:
			wrSensorRegs8_8(OV2640_1280x1024_JPEG);
			break;
		case OV2640_1600x1200:
			wrSensorRegs8_8(OV2640_1600x1200_JPEG);
			break;
		default:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
	}
}

void ArduCAM::set_format(byte fmt) {
  if (fmt == BMP)
    m_fmt_ = BMP;
  else if(fmt == RAW)
    m_fmt_ = RAW;
  else
    m_fmt_ = JPEG;
}

void ArduCAM::OV2640_set_Light_Mode(uint8_t Light_Mode) {
	switch(Light_Mode) {
		case Auto:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0xc7, 0x00); //AWB on
			break;
		case Sunny:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0xc7, 0x40); //AWB off
			wrSensorReg8_8(0xcc, 0x5e);
			wrSensorReg8_8(0xcd, 0x41);
			wrSensorReg8_8(0xce, 0x54);
			break;
		case Cloudy:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0xc7, 0x40); //AWB off
			wrSensorReg8_8(0xcc, 0x65);
			wrSensorReg8_8(0xcd, 0x41);
			wrSensorReg8_8(0xce, 0x4f);  
			break;
		case Office:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0xc7, 0x40); //AWB off
			wrSensorReg8_8(0xcc, 0x52);
			wrSensorReg8_8(0xcd, 0x41);
			wrSensorReg8_8(0xce, 0x66);
			break;
		case Home:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0xc7, 0x40); //AWB off
			wrSensorReg8_8(0xcc, 0x42);
			wrSensorReg8_8(0xcd, 0x3f);
			wrSensorReg8_8(0xce, 0x71);
			break;
		default :
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0xc7, 0x00); //AWB on
			break; 
	}
}

void ArduCAM::OV2640_set_Color_Saturation(uint8_t Color_Saturation) {
	switch(Color_Saturation) {
		case Saturation2:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x02);
			wrSensorReg8_8(0x7c, 0x03);
			wrSensorReg8_8(0x7d, 0x68);
			wrSensorReg8_8(0x7d, 0x68);
		break;
		case Saturation1:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x02);
			wrSensorReg8_8(0x7c, 0x03);
			wrSensorReg8_8(0x7d, 0x58);
			wrSensorReg8_8(0x7d, 0x58);
		break;
		case Saturation0:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x02);
			wrSensorReg8_8(0x7c, 0x03);
			wrSensorReg8_8(0x7d, 0x48);
			wrSensorReg8_8(0x7d, 0x48);
		break;
		case Saturation_1:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x02);
			wrSensorReg8_8(0x7c, 0x03);
			wrSensorReg8_8(0x7d, 0x38);
			wrSensorReg8_8(0x7d, 0x38);
		break;
		case Saturation_2:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x02);
			wrSensorReg8_8(0x7c, 0x03);
			wrSensorReg8_8(0x7d, 0x28);
			wrSensorReg8_8(0x7d, 0x28);
		break;	
	}
}

void ArduCAM::OV2640_set_Brightness(uint8_t Brightness) {
	switch(Brightness) {
		case Brightness2:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x09);
			wrSensorReg8_8(0x7d, 0x40);
			wrSensorReg8_8(0x7d, 0x00);
			break;
		case Brightness1:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x09);
			wrSensorReg8_8(0x7d, 0x30);
			wrSensorReg8_8(0x7d, 0x00);
			break;	
		case Brightness0:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x09);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x00);
			break;
		case Brightness_1:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x09);
			wrSensorReg8_8(0x7d, 0x10);
			wrSensorReg8_8(0x7d, 0x00);
			break;
		case Brightness_2:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x09);
			wrSensorReg8_8(0x7d, 0x00);
			wrSensorReg8_8(0x7d, 0x00);
			break;	
	}
}
	
void ArduCAM::OV2640_set_Contrast(uint8_t Contrast) {
	switch(Contrast) {
		case Contrast2:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x07);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x28);
			wrSensorReg8_8(0x7d, 0x0c);
			wrSensorReg8_8(0x7d, 0x06);
			break;
		case Contrast1:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x07);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x24);
			wrSensorReg8_8(0x7d, 0x16);
			wrSensorReg8_8(0x7d, 0x06); 
			break;
		case Contrast0:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x07);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x06); 
			break;
		case Contrast_1:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x07);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x2a);
			wrSensorReg8_8(0x7d, 0x06);	
			break;
		case Contrast_2:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x04);
			wrSensorReg8_8(0x7c, 0x07);
			wrSensorReg8_8(0x7d, 0x20);
			wrSensorReg8_8(0x7d, 0x18);
			wrSensorReg8_8(0x7d, 0x34);
			wrSensorReg8_8(0x7d, 0x06);
		break;
	}
}

void ArduCAM::OV2640_set_Special_effects(uint8_t Special_effect) {
	switch(Special_effect){
		case Antique:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x18);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0x40);
			wrSensorReg8_8(0x7d, 0xa6);
			break;
		case Bluish:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x18);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0xa0);
			wrSensorReg8_8(0x7d, 0x40);
		break;
			case Greenish:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x18);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0x40);
			wrSensorReg8_8(0x7d, 0x40);
			break;
		case Reddish:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x18);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0x40);
			wrSensorReg8_8(0x7d, 0xc0);
			break;
		case BW:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x18);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0x80);
			wrSensorReg8_8(0x7d, 0x80);
			break;
		case Negative:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x40);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0x80);
			wrSensorReg8_8(0x7d, 0x80);
			break;
		case BWnegative:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x58);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0x80);
			wrSensorReg8_8(0x7d, 0x80);
			break;
		case Normal:
			wrSensorReg8_8(0xff, 0x00);
			wrSensorReg8_8(0x7c, 0x00);
			wrSensorReg8_8(0x7d, 0x00);
			wrSensorReg8_8(0x7c, 0x05);
			wrSensorReg8_8(0x7d, 0x80);
			wrSensorReg8_8(0x7d, 0x80);
		break;
	}
}

// Write 8 bit values to 8 bit register address
int ArduCAM::wrSensorRegs8_8(const struct sensor_reg reglist[]) {
	int err = 0;
	uint16_t reg_addr = 0;
	uint16_t reg_val = 0;
	const struct sensor_reg *next = reglist;

	while ((reg_addr != 0xff) | (reg_val != 0xff)) {
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg8_8(reg_addr, reg_val);
		next++;
	}
	return 1;
}

// Write 16 bit values to 8 bit register address
int ArduCAM::wrSensorRegs8_16(const struct sensor_reg reglist[]) {
	int err = 0;
	unsigned int reg_addr, reg_val;
	const struct sensor_reg *next = reglist;

	while ((reg_addr != 0xff) | (reg_val != 0xffff)) {
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg8_16(reg_addr, reg_val);
		//  if (!err)
		//return err;
		next++;
	}
	return 1;
}

// Write 8 bit values to 16 bit register address
int ArduCAM::wrSensorRegs16_8(const struct sensor_reg reglist[])
{
	int err = 0;
	unsigned int reg_addr;
	unsigned char reg_val;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xffff) | (reg_val != 0xff)) {
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
	    err = wrSensorReg16_8(reg_addr, reg_val);
	    //if (!err)
	    //return err;
	    next++;
	}
	return 1;
}

//I2C Array Write 16bit address, 16bit data
int ArduCAM::wrSensorRegs16_16(const struct sensor_reg reglist[])
{
	int err = 0;
	unsigned int reg_addr, reg_val;
	const struct sensor_reg *next = reglist;
	reg_addr = pgm_read_word(&next->reg);
	reg_val = pgm_read_word(&next->val);

	while ((reg_addr != 0xffff) | (reg_val != 0xffff)) {
	    err = wrSensorReg16_16(reg_addr, reg_val);
	    //if (!err)
	    //   return err;
	    next++;
	    reg_addr = pgm_read_word(&next->reg);
	    reg_val = pgm_read_word(&next->val);
	}
 	return 1;
}

// Read/write 8 bit value to/from 8 bit register address	
byte ArduCAM::wrSensorReg8_8(uint8_t regID, uint8_t regDat)
{
	uint8_t buffer[2] = {regID, regDat}; // 2 bytes of data
	if (i2c_.transmit(sensor_addr_>>1, buffer, 2) != 2) // send 2 bytes
		return 0; // transmit failed
	delay(1);
	return 1;
	
}
byte ArduCAM::rdSensorReg8_8(uint8_t regID, uint8_t* regDat)
{	
	i2c_.transmit(sensor_addr_>>1, &regID, 1); // send 1 byte

	if (i2c_.receive(sensor_addr_>>1, regDat, 1) != 1) // read 1 byte
		return 0; // receive failed
	delay(1);
	return 1;
}
// Read/write 16 bit value to/from 8 bit register address
byte ArduCAM::wrSensorReg8_16(uint8_t regID, uint16_t regDat)
{
	uint8_t buffer[3] = {regID, (uint8_t)(regDat>>8), (uint8_t)(regDat)}; // 3 bytes of data
	if (i2c_.transmit(sensor_addr_>>1, buffer, 3) != 3) // send 3 bytes
		return 0; // transmit failed
	delay(1);
	return 1;
}
byte ArduCAM::rdSensorReg8_16(uint8_t regID, uint16_t* regDat)
{
  	uint8_t buffer[2]; // array for temporarily storing 2 bytes received from I2C
	i2c_.transmit(sensor_addr_>>1, &regID, 1); // send 1 byte

	if (i2c_.receive(sensor_addr_>>1, buffer, 2) != 2) // read 2 bytes
		return 0; // receive failed
	*regDat = (((uint16_t) buffer[0]) << 8) | buffer[1]; // concat 2 bytes into short
	delay(1);
  	return 1;
}

// Read/write 8 bit value to/from 16 bit register address
byte ArduCAM::wrSensorReg16_8(uint16_t regID, uint8_t regDat)
{
	uint8_t data[3] = {(uint8_t)(regID>>8), (uint8_t)(regID), regDat}; // 3 bytes of data
	if (i2c_.transmit(sensor_addr_>>1, data, 3) != 3) // send 3 bytes
		return 0; // transmit failed
	delay(1);
	return 1;
}
byte ArduCAM::rdSensorReg16_8(uint16_t regID, uint8_t* regDat)
{
	uint8_t data[2] = {(uint8_t)(regID>>8), (uint8_t)(regID)}; // 2 bytes of data
	i2c_.transmit(sensor_addr_>>1, data, 2); // send 2 bytes

	if (i2c_.receive(sensor_addr_>>1, regDat, 1) != 1) // read 1 byte
		return 0; // receive failed
	delay(1);
	return 1;
}

//I2C Write 16bit address, 16bit data
byte ArduCAM::wrSensorReg16_16(uint16_t regID, uint16_t regDat)
{
	uint8_t data[4] = {(uint8_t)(regID>>8), (uint8_t)(regID), (uint8_t)(regDat>>8), (uint8_t)(regDat)}; // 4 bytes of data
	if (i2c_.transmit(sensor_addr_>>1, data, 4) != 4) // send 4 bytes
		return 0; // transmit failed
	delay(1);
  	return (1);
}

//I2C Read 16bit address, 16bit data
byte ArduCAM::rdSensorReg16_16(uint16_t regID, uint16_t* regDat)
{
	uint8_t data[2] = {(uint8_t)(regID>>8), (uint8_t)(regID)}; // 2 bytes of data
	i2c_.transmit(sensor_addr_>>1, data, 2); // send 2 bytes

	if (i2c_.receive(sensor_addr_>>1, data, 2) != 2) // read 2 bytes
		return 0; // receive failed
	*regDat = (((uint16_t) data[0]) << 8) | data[1]; // concat 2 bytes into short
	delay(1);
  	return (1);
}
