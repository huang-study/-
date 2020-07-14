/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "bmp.h"
#include "simulate_i2c.h"
#include "bit_domain.h"
#include "ADXL345.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t DevID = 0, buf[6], cmd=XL345_OFSX;
char ch[100];
unsigned int INCnt[2] = {0,0};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	extern union BitDomain_64 VideoMem[128];
	uint32_t cnt;
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
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	SimulateI2C_Init(i2c1);
	SimulateI2C_Init(i2c2);
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	; 
	
	buf[0] = 0xB;
	SimulateI2C_WriteByte(i2c2 ,0x31, 0x53, 1,buf); //测量范围,正负16g，13位模式
	buf[0] = 0x08;
	SimulateI2C_WriteByte(i2c2 ,0x2C, 0x53, 1,buf); //速率设定为12.5 参考pdf13页
	buf[0] = 0x08;
	SimulateI2C_WriteByte(i2c2 ,0x2D, 0x53, 1,buf); //选择电源模式   参考pdf24页
	buf[0] = 0x80;
	SimulateI2C_WriteByte(i2c2 ,0x2E, 0x53, 1,buf);  //X 偏移量 根据测试传感器的状态写入pdf29页
	buf[0] = 0x00;
	SimulateI2C_WriteByte(i2c2 ,0x1E, 0x53, 1,buf);  //Y 偏移量 根据测试传感器的状态写入pdf29页
	buf[0] = 0x05;
	SimulateI2C_WriteByte(i2c2 ,0x20, 0x53, 1,buf); //Z 偏移量 根据测试传感器的状态写入pdf29页
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		SimulateI2C_ReadByte(i2c2, 0x32, 0x53, 6,buf);
		
		sprintf(ch, "x:%8d", (short)(buf[1]<<8)+buf[0]);
		OLED_ShowString(5, 0, ch, 16);
		sprintf(ch, "Y:%8d", (short)(buf[3]<<8)+buf[2]);
		OLED_ShowString(5, 2, ch, 16);
		sprintf(ch, "z:%8d", (short)(buf[5]<<8)+buf[4]);
		OLED_ShowString(5, 4, ch, 16);
//		sprintf(ch, "I1:%3d:I2:%3d", INCnt[0], INCnt[1]);
//		OLED_ShowString(5, 6, ch, 16);
//		
//		OLEDFill(0);
//		RefreshVideoMem();
//		
//		for(cnt = 0; cnt < 8; cnt++)
//		{
//			OLEDDrawLine(0, 0, 16+cnt*16, 64, 1, OLEDNORever);
//			RefreshVideoMem();
//			HAL_Delay(10);
//		}
//		for(cnt = 0; cnt < 8; cnt++)
//		{
//		OLEDDrawLine(127, 64, cnt*16, 0, 1, OLEDNORever);
//			RefreshVideoMem();
//			HAL_Delay(10);
//		}
//		for(cnt = 0; cnt < 7; cnt++)
//		{
//			OLEDDrawLine(127, 0, 127-16-cnt*16, 64, 1, OLEDNORever);
//			RefreshVideoMem();
//			HAL_Delay(10);
//		}
//		for(cnt = 0; cnt < 8; cnt++)
//		{
//		OLEDDrawLine(0, 63, 127-cnt*16, 0, 1, OLEDNORever);
//			RefreshVideoMem();
//			HAL_Delay(10);
//		}
//		HAL_Delay(500);
//		
//		OLEDFill(0);
//		
//		OLEDDrawCircule(64, 32, 5, 2, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawCircule(64, 32, 10, 2, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawCircule(64, 32, 15, 2, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawCircule(64, 32, 20, 2, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawCircule(64, 32, 25, 2, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawCircule(64, 32, 30, 2, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		
//		OLEDFill(0);
//		OLEDDrawSquareLangth(0, 0, 20, 20, 0, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(25, 0, 20, 20, 1, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(50, 0, 20, 20, 0, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(75, 0, 20, 20, 1, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(100, 0, 20, 20, 0, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(0, 25, 20, 20, 1, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(25, 25, 20, 20, 0, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(50, 25, 20, 20, 1, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(75, 25, 20, 20, 0, 1, OLEDNORever);
//		RefreshVideoMem();
//		HAL_Delay(500);
//		OLEDDrawSquareLangth(100, 25, 20, 20, 1, 1, OLEDNORever);
//		RefreshVideoMem();
		HAL_Delay(500);
//		
//		HAL_Delay(1000);
  /* USER CODE END 3 */
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
