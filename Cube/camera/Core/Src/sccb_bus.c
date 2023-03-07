/**
  * @file    sccb_bus.c 
  * @author  Arducam
  * @version V0.1
  * @date    2018.06.22
  * @brief   sccb bus
  */
#include "sccb_bus.h"
#include "delay.h"
 uint32_t ntime;

/**
  * @brief  init i2c bus
  * @param  None
  * @retval None
  */
void sccb_bus_init(void)
{
	// GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//1 
	
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//2 

 	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 	//3

 	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//4
 	// GPIO_Init(GPIOB, &GPIO_InitStructure);					//5
 	// GPIO_SetBits(GPIOB,GPIO_Pin_11);						 	//6

	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//7
 	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//8
 	// GPIO_Init(GPIOB, &GPIO_InitStructure);					//9
 	// GPIO_SetBits(GPIOB,GPIO_Pin_10);							//10
	//SCCB_DATA_OUT;											//11
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//Refer to Pg 26 of STM32L4 Hal Document
	
	/*___Serial Data Line (SDA)___*/

	/*[TODO] RCC->APB2ER = GPIOB */

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;				  //1 Uses peripheral clk for GPIOB

	GPIO_InitStructure.Pin = GPIO_PIN_6;						//2
 	
	/*[TODO] Not sure how to set up internal pull up in step 3 */ 

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD; 				//3	 set to output mode and use internal pull up
	GPIO_InitStructure.Pull = GPIO_PULLUP;

	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;			//4
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						//5
 	GPIO_SetBits(GPIOB, GPIO_PIN_6);							//6

	/*___Serial Clk Line (SCL)___*/
	GPIO_InitStructure.Pin = GPIO_PIN_7;						//7
 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 				//8 set to Pull up pull down
 	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);					//9
 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);  		//10 set pin high

 	SCCB_DATA_OUT;												//11 //sets pin to output data

}




void sccb_bus_start(void)
{
    SCCB_SID_H();             
    delay_us(I2C_TIM);
    SCCB_SIC_H();	           
    delay_us(I2C_TIM);
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_L();	           
    delay_us(I2C_TIM);
}


void sccb_bus_stop(void)
{
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_H();	
    delay_us(I2C_TIM);  
    SCCB_SID_H();	
    delay_us(I2C_TIM);  
}


void sccb_bus_send_noack(void)
{	
	SCCB_SID_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SID_L();	
	delay_us(I2C_TIM);
}

void sccb_bus_send_ack(void)
{	
	SCCB_SID_L();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SIC_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SID_L();	
	delay_us(I2C_TIM);
}

uint8_t sccb_bus_write_byte(uint8_t data)
{
	uint32_t i;
	uint8_t tem;

	for(i = 0; i < 8; i++) 
	{
		if((data<<i) & 0x80)
		{
			SCCB_SID_H();
		}
		else 
		{
			SCCB_SID_L();
		}
		delay_us(I2C_TIM);
		SCCB_SIC_H();	
		delay_us(I2C_TIM);
		SCCB_SIC_L();	

	}
	SCCB_DATA_IN;
	delay_us(I2C_TIM);
	SCCB_SIC_H();	
	delay_us(I2C_TIM);
	if(SCCB_SID_STATE)
	{
		tem = 0;               
	}
	else 
	{
		tem = 1;                
	}

	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_DATA_OUT;
	return tem;  
}

uint8_t sccb_bus_read_byte(void)
{	
	uint32_t i;
	uint8_t read = 0;
	
	SCCB_DATA_IN;
	delay_us(I2C_TIM);
	for(i = 8; i > 0; i--) 
	{		     
		delay_us(I2C_TIM);
		SCCB_SIC_H();
		delay_us(I2C_TIM);
		read = read << 1;
		if(SCCB_SID_STATE)
		{
			read += 1; 
		}
		SCCB_SIC_L();
		delay_us(I2C_TIM);
	}	
    SCCB_DATA_OUT;
	return read;
}

