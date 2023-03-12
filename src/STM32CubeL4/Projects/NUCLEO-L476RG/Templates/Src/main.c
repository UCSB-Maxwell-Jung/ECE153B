/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();
  /* Configure the System clock to have a frequency of 80 MHz */
  SystemClock_Config();

  //delay_init(); //use HAL_delay() instead
  
	// USART1_UART_Init(921600);
	// ArduCAM_LED_init();
	// ArduCAM_CS_init();
	// sccb_bus_init();
	// SPI1_Init();
	
// 	while(1)
// 	{
// 		write_reg(ARDUCHIP_TEST1, 0x55);
// 		temp = read_reg(ARDUCHIP_TEST1);
// 		if (temp != 0x55)
// 		{
// 			printf("ACK CMD SPI interface Error!\n");
// 			delay_ms(1000);
// 			continue;
// 		}
// 		else
// 		{
// 			printf("ACK CMD SPI interface OK!\r\n");
// 			break;
// 		}
// 	}
	
//   while(1)
// 	{
// 		sensor_addr = 0x60;
// 		wrSensorReg8_8(0xff, 0x01);
// 		rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
// 		rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
// 		if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
// 			printf("ACK CMD Can't find OV2640 module!\r\n");
// 		else
// 		{
// 		  sensor_model =  OV2640 ;
// 		  printf("ACK CMD OV2640 detected.\r\n");   
// 		  break;
// 		}
// 		sensor_addr = 0x78;
// 		rdSensorReg16_8(OV5640_CHIPID_HIGH, &vid);
// 		rdSensorReg16_8(OV5640_CHIPID_LOW, &pid);
// 		if ((vid != 0x56) || (pid != 0x40))
// 			printf("ACK CMD Can't find OV5640 module!\r\n");
// 		else
// 		{
// 			sensor_model =  OV5640 ;
// 			printf("ACK CMD OV5640 detected.\r\n");
// 		  break;
// 		}
// 		sensor_addr = 0x78;
// 		rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
// 		rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
// 		if ((vid != 0x56) || (pid != 0x42))
// 		{
// 			printf("ACK CMD Can't find OV5642 module!\r\n");
// 			continue;
// 		}
// 		else
// 		{
// 		 sensor_model =  OV5642 ;
// 		 printf("ACK CMD OV5642 detected.\r\n"); 
// 		 break;		 
// 		}
// 	}  
// //Support OV2640/OV5640/OV5642 Init
//   ArduCAM_Init(sensor_model);

// 	while(1)
// 	{
// 		if(NewCMD == 1)
// 		{
// 			NewCMD = 0;
// 			switch(USART1_RecieveData)
// 			{
// 				case 0:
// 					if (sensor_model == OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_160x120);
// 						printf("ACK CMD switch to OV2640_160x120\r\n");
// 					}
// 					else if(sensor_model == OV5640)
// 					{
// 						OV5640_set_JPEG_size(OV5640_320x240);
// 						printf("ACK CMD switch to OV5640_320x240\r\n");
// 					}
// 					else if(sensor_model == OV5642)
// 					{
// 						OV5642_set_JPEG_size(OV5642_320x240);
// 						printf("ACK CMD switch to OV5642_320x240\r\n");
// 					}
// 					break;
// 				case 1:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_176x144);
// 						printf("ACK CMD switch to OV2640_176x144\r\n");
// 					}
// 					else if(sensor_model== OV5640)
// 					{ 
// 						OV5640_set_JPEG_size(OV5640_352x288);
// 						printf("ACK CMD switch to OV5640_352x288\r\n");
// 					}
// 					else if(sensor_model== OV5642)
// 					{ 
// 						OV5642_set_JPEG_size(OV5642_640x480);
// 						printf("ACK CMD switch to OV5642_640x480\r\n");
// 					}
// 					break;
// 				case 2:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_320x240);
// 						printf("ACK CMD switch to OV2640_320x240\r\n");
// 					}
// 					else if(sensor_model== OV5640)
// 					{ 
// 						OV5640_set_JPEG_size(OV5640_640x480);
// 					printf("ACK CMD switch to OV5640_640x480\r\n");
// 					}
// 					else if(sensor_model== OV5642)
// 					{
// 						OV5642_set_JPEG_size(OV5642_1024x768);
// 						printf("ACK CMD switch to OV5642_1024x768\r\n");
// 					}
// 					break;
// 				case 3:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_352x288);
// 						printf("ACK CMD switch to OV2640_352x288\r\n");
// 					}
// 					else if(sensor_model== OV5640)
// 					{ 
// 						OV5640_set_JPEG_size(OV5640_800x480);
// 						printf("ACK CMD switch to OV5640_800x480\r\n");
// 					}
// 					else if(sensor_model== OV5642)
// 					{
// 						OV5642_set_JPEG_size(OV5642_1280x960);
// 						printf("ACK CMD switch to OV5642_1280x960\r\n");
// 					}
// 					break;
// 				case 4:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_640x480);
// 						printf("ACK CMD switch to OV2640_640x480\r\n");
// 					}
// 					else if(sensor_model== OV5640)
// 					{
// 						OV5640_set_JPEG_size(OV5640_1024x768);
// 						printf("ACK CMD switch to OV5640_1024x768\r\n");
// 					}
// 					else if(sensor_model== OV5642)
// 					{
// 						OV5642_set_JPEG_size(OV5642_1600x1200);	
// 						printf("ACK CMD switch to OV5642_1600x1200\r\n");
// 					}
// 					break;
// 				case 5:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_800x600);
// 						printf("ACK CMD switch to OV2640_800x600\r\n");
// 					}
// 					else if(sensor_model== OV5640) 
// 					{
// 						OV5640_set_JPEG_size(OV5640_1280x960);
// 						printf("ACK CMD switch to OV5640_1280x960\r\n");
// 					}
// 					else if(sensor_model== OV5642)
// 					{
// 						OV5642_set_JPEG_size(OV5642_2048x1536);
// 						printf("ACK CMD switch to OV5642_2048x1536\r\n");
// 					}
// 					break;
// 				case 6:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_1024x768);
// 						printf("ACK CMD switch to OV2640_1024x768\r\n");
// 					}
// 					else if(sensor_model== OV5640)
// 					{
// 						OV5640_set_JPEG_size(OV5640_1600x1200);
// 						printf("ACK CMD switch to OV5640_1600x1200\r\n");
// 					}
// 					else if(sensor_model== OV5642)
// 					{
// 						OV5642_set_JPEG_size(OV5642_2592x1944);	
// 						printf("ACK CMD switch to OV5642_2592x1944\r\n");
// 					}
// 					break;
// 				case 7:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_1280x1024);
// 						printf("ACK CMD switch to OV2640_1280x1024\r\n");
// 					}
// 					else if(sensor_model== OV5640) 
// 					{
// 						OV5640_set_JPEG_size(OV5640_2048x1536);
// 						printf("ACK CMD switch to OV5640_2048x1536\r\n");
// 					}
// 					break;
// 				case 8:
// 					if(sensor_model== OV2640)
// 					{
// 						OV2640_set_JPEG_size(OV2640_1600x1200);
// 						printf("ACK CMD switch to OV2640_1600x1200\r\n");
// 					}
// 					else if(sensor_model== OV5640) 
// 					{
// 						OV5640_set_JPEG_size(OV5640_2592x1944);
// 						printf("ACK CMD switch to OV5640_2592x1944\r\n");
// 					}
// 					break;
// 				case 0x10:
// 					Camera_WorkMode = 1;
// 					start_shoot = 1;
// 					printf("ACK CMD CAM start single shoot.\r\n");
// 					break;
// 				case 0x11:
// 					set_format(JPEG);
// 					ArduCAM_Init(sensor_model);
// 					#if !defined(OV2640)
// 						set_bit(ARDUCHIP_TIM,VSYNC_MASK);
// 					#endif
// 					break;
// 				case 0x20:
// 					Camera_WorkMode = 2;
// 					start_shoot = 2;
// 					printf("ACK CMD CAM start video streaming.\r\n");
// 					break;
// 				case 0x21:
// 					stop = 1;
// 					printf("ACK CMD CAM stop video streaming.\r\n");
// 					break;
// 				case 0x30:
// 					Camera_WorkMode = 3;
// 					start_shoot = 3;
// 					printf("ACK CMD CAM start single shoot.\r\n");
// 					break;
// 				case 0x31:
// 					set_format(BMP);
// 					ArduCAM_Init(sensor_model);
// 					if(sensor_model != OV2640)
// 					{
// 						clear_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
// 					}
// 					wrSensorReg16_8(0x3818,0x81);
// 					wrSensorReg16_8(0x3621,0xa7);		
// 					printf("ACK CMD SetToBMP \r\n");	
// 					break;
// 				default:
// 					break;
// 			}
// 		}
// 		if(Camera_WorkMode == 1)
// 		{
// 			if(start_shoot == 1)
// 			{
// 				start_shoot = 0;
// 				SingleCapTransfer();
// 			}
// 			if(receive_OK) 
// 			{
// 				receive_OK= 0;
// 				SendbyUSART1(); 
// 			}
// 		}	
// 		else if(Camera_WorkMode == 2)
// 		{
// 			if(start_shoot == 2)
// 			{
// 				if(send_OK)
// 				{
// 					if(stop)
// 					{
// 						printf("ACK CMD CAM stop video streaming.\r\n");
// 						stop = 0;
// 						Camera_WorkMode = 0;
// 						start_shoot = 0;
// 					}
// 					send_OK=false;    
// 					SingleCapTransfer();		
// 				}	
// 				if(receive_OK) 
// 				{
// 					receive_OK= 0;
// 					SendbyUSART1();   
// 				}
// 			}
// 		}
// 		else if(Camera_WorkMode == 3)
// 		{	
// 			if(start_shoot == 3)
// 			{
// 				start_shoot = 0;
// 				Camera_WorkMode = 0;
// 				StartBMPcapture();
// 			}
// 		}
// 	}
// }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

