/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t vid, pid, temp;
  uint8_t Camera_WorkMode = 0;
  uint8_t start_shoot = 0;
  uint8_t stop = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  //___________________FROM ARDUCAM (BELOW)_____________________//

     //delay_init(); //use HAL_Delay() which is set up by HAL_Init();
  	 //USART1_UART_Init(921600); //USART (idk)

  	//ArduCAM_LED_init(); inits and LED (idk)

  	 ArduCAM_CS_init();

  	 sccb_bus_init(); //inits the sensor on camera (I2C setup)

  	 SPI1_Init();

  	while(1)
  	{
  		write_reg(ARDUCHIP_TEST1, 0x55);
  		temp = read_reg(ARDUCHIP_TEST1);
  		if (temp != 0x55)
  		{
  			printf("ACK CMD SPI interface Error!\n");
          	  	HAL_Delay(1000); //in ms

  			continue;
  		}
  		else
  		{
  			printf("ACK CMD SPI interface OK!\r\n");
  			break;
  		}
  	}

    while(1)
  	{
  		sensor_addr = 0x60;
  		wrSensorReg8_8(0xff, 0x01);
  		rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  		rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
  		if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
  			printf("ACK CMD Can't find OV2640 module!\r\n");
  		else
  		{
  		  sensor_model =  OV2640 ;
  		  printf("ACK CMD OV2640 detected.\r\n");
  		  break;
  		}
  		sensor_addr = 0x78;
  		rdSensorReg16_8(OV5640_CHIPID_HIGH, &vid);
  		rdSensorReg16_8(OV5640_CHIPID_LOW, &pid);
  		if ((vid != 0x56) || (pid != 0x40))
  			printf("ACK CMD Can't find OV5640 module!\r\n");
  		else
  		{
  			sensor_model =  OV5640 ;
  			printf("ACK CMD OV5640 detected.\r\n");
  		  break;
  		}
  		sensor_addr = 0x78;
  		rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
  		rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
  		if ((vid != 0x56) || (pid != 0x42))
  		{
  			printf("ACK CMD Can't find OV5642 module!\r\n");
  			continue;
  		}
  		else
  		{
  		 sensor_model =  OV5642 ;
  		 printf("ACK CMD OV5642 detected.\r\n");
  		 break;
  		}
  	}
  //Support OV2640/OV5640/OV5642 Init
    ArduCAM_Init(sensor_model);

  	while(1)
  	{
  		if(NewCMD == 1)
  		{
  			NewCMD = 0;
  			switch(USART1_RecieveData)
  			{
  				case 0:
  					if (sensor_model == OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_160x120);
  						printf("ACK CMD switch to OV2640_160x120\r\n");
  					}
  					else if(sensor_model == OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_320x240);
  						printf("ACK CMD switch to OV5640_320x240\r\n");
  					}
  					else if(sensor_model == OV5642)
  					{
  						OV5642_set_JPEG_size(OV5642_320x240);
  						printf("ACK CMD switch to OV5642_320x240\r\n");
  					}
  					break;
  				case 1:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_176x144);
  						printf("ACK CMD switch to OV2640_176x144\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_352x288);
  						printf("ACK CMD switch to OV5640_352x288\r\n");
  					}
  					else if(sensor_model== OV5642)
  					{
  						OV5642_set_JPEG_size(OV5642_640x480);
  						printf("ACK CMD switch to OV5642_640x480\r\n");
  					}
  					break;
  				case 2:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_320x240);
  						printf("ACK CMD switch to OV2640_320x240\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_640x480);
  					printf("ACK CMD switch to OV5640_640x480\r\n");
  					}
  					else if(sensor_model== OV5642)
  					{
  						OV5642_set_JPEG_size(OV5642_1024x768);
  						printf("ACK CMD switch to OV5642_1024x768\r\n");
  					}
  					break;
  				case 3:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_352x288);
  						printf("ACK CMD switch to OV2640_352x288\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_800x480);
  						printf("ACK CMD switch to OV5640_800x480\r\n");
  					}
  					else if(sensor_model== OV5642)
  					{
  						OV5642_set_JPEG_size(OV5642_1280x960);
  						printf("ACK CMD switch to OV5642_1280x960\r\n");
  					}
  					break;
  				case 4:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_640x480);
  						printf("ACK CMD switch to OV2640_640x480\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_1024x768);
  						printf("ACK CMD switch to OV5640_1024x768\r\n");
  					}
  					else if(sensor_model== OV5642)
  					{
  						OV5642_set_JPEG_size(OV5642_1600x1200);
  						printf("ACK CMD switch to OV5642_1600x1200\r\n");
  					}
  					break;
  				case 5:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_800x600);
  						printf("ACK CMD switch to OV2640_800x600\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_1280x960);
  						printf("ACK CMD switch to OV5640_1280x960\r\n");
  					}
  					else if(sensor_model== OV5642)
  					{
  						OV5642_set_JPEG_size(OV5642_2048x1536);
  						printf("ACK CMD switch to OV5642_2048x1536\r\n");
  					}
  					break;
  				case 6:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_1024x768);
  						printf("ACK CMD switch to OV2640_1024x768\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_1600x1200);
  						printf("ACK CMD switch to OV5640_1600x1200\r\n");
  					}
  					else if(sensor_model== OV5642)
  					{
  						OV5642_set_JPEG_size(OV5642_2592x1944);
  						printf("ACK CMD switch to OV5642_2592x1944\r\n");
  					}
  					break;
  				case 7:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_1280x1024);
  						printf("ACK CMD switch to OV2640_1280x1024\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_2048x1536);
  						printf("ACK CMD switch to OV5640_2048x1536\r\n");
  					}
  					break;
  				case 8:
  					if(sensor_model== OV2640)
  					{
  						OV2640_set_JPEG_size(OV2640_1600x1200);
  						printf("ACK CMD switch to OV2640_1600x1200\r\n");
  					}
  					else if(sensor_model== OV5640)
  					{
  						OV5640_set_JPEG_size(OV5640_2592x1944);
  						printf("ACK CMD switch to OV5640_2592x1944\r\n");
  					}
  					break;
  				case 0x10:
  					Camera_WorkMode = 1;
  					start_shoot = 1;
  					printf("ACK CMD CAM start single shoot.\r\n");
  					break;
  				case 0x11:
  					set_format(JPEG);
  					ArduCAM_Init(sensor_model);
  					#if !defined(OV2640)
  						set_bit(ARDUCHIP_TIM,VSYNC_MASK);
  					#endif
  					break;
  				case 0x20:
  					Camera_WorkMode = 2;
  					start_shoot = 2;
  					printf("ACK CMD CAM start video streaming.\r\n");
  					break;
  				case 0x21:
  					stop = 1;
  					printf("ACK CMD CAM stop video streaming.\r\n");
  					break;
  				case 0x30:
  					Camera_WorkMode = 3;
  					start_shoot = 3;
  					printf("ACK CMD CAM start single shoot.\r\n");
  					break;
  				case 0x31:
  					set_format(BMP);
  					ArduCAM_Init(sensor_model);
  					if(sensor_model != OV2640)
  					{
  						clear_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
  					}
  					wrSensorReg16_8(0x3818,0x81);
  					wrSensorReg16_8(0x3621,0xa7);
  					printf("ACK CMD SetToBMP \r\n");
  					break;
  				default:
  					break;
  			}
  		}
  		if(Camera_WorkMode == 1)
  		{
  			if(start_shoot == 1)
  			{
  				start_shoot = 0;
  				SingleCapTransfer();
  			}
  			if(receive_OK)
  			{
  				receive_OK= 0;
  				SendbyUSART1();
  			}
  		}
  		else if(Camera_WorkMode == 2)
  		{
  			if(start_shoot == 2)
  			{
  				if(send_OK)
  				{
  					if(stop)
  					{
  						printf("ACK CMD CAM stop video streaming.\r\n");
  						stop = 0;
  						Camera_WorkMode = 0;
  						start_shoot = 0;
  					}
  					send_OK=false;
  					SingleCapTransfer();
  				}
  				if(receive_OK)
  				{
  					receive_OK= 0;
  					SendbyUSART1();
  				}
  			}
  		}
  		else if(Camera_WorkMode == 3)
  		{
  			if(start_shoot == 3)
  			{
  				start_shoot = 0;
  				Camera_WorkMode = 0;
  				StartBMPcapture();
  			}
  		}
  	}
  }
 /* ___________________FROM ARDUCAM (ABOVE)_____________________*/
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
