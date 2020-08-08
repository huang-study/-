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
#include "math.h"
#include "screen.h"
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
uint8_t DevID = 0, buf[6], cmd = XL345_OFSX;
int32_t AveX, AveY, AveZ;
char ch[100];
double AccX, AccY;
unsigned int INCnt[2] = {0, 0};
uint32_t AccCalc;
double VelocityX = 0, VelocityY = 0;
double ElasticAttritionRato = 0.90;
char keyState = 0;
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
  int32_t ball_x = 63, ball_y = 31, ball_r = 3, ball_degree = 4;
  uint32_t Tick, x_axle;
  uint8_t DspMode;
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
  OLED_Init(); //初始化OLED

  buf[0] = 0x1;
  SimulateI2C_WriteByte(i2c2, 0x31, 0x53, 1, buf); //测量范围,正负16g，13位模式
  buf[0] = 0x08;
  SimulateI2C_WriteByte(i2c2, 0x2C, 0x53, 1, buf); //速率设定为12.5 参考pdf13页
  buf[0] = 0x08;
  SimulateI2C_WriteByte(i2c2, 0x2D, 0x53, 1, buf); //选择电源模式   参考pdf24页
  buf[0] = 0x80;
  SimulateI2C_WriteByte(i2c2, 0x2E, 0x53, 1, buf); //X 偏移量 根据测试传感器的状态写入pdf29页
  buf[0] = 0x00;
  SimulateI2C_WriteByte(i2c2, 0x1E, 0x53, 1, buf); //Y 偏移量 根据测试传感器的状态写入pdf29页
  buf[0] = 0x05;
  SimulateI2C_WriteByte(i2c2, 0x20, 0x53, 1, buf); //Z 偏移量 根据测试传感器的状态写入pdf29页

  ScreenFill(0);
  RefreshScreen();
  Tick = HAL_GetTick() + 100;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (HAL_GetTick() == Tick)
    {
      if (0 != (keyState & 0x01))
      {
        keyState &= ~(0x01 << 0);
        DspMode = 0;
      }
      else if (0 != (keyState & 0x02))
      {
        keyState &= ~(0x01 << 1);
        DspMode = 1;
        x_axle = 0;
      }
      else if (0 != (keyState & 0x04))
      {
        keyState &= ~(0x01 << 2);
        DspMode = 2;
      }
      if (0 == DspMode)
      {
        ScreenFill(0);
        RefreshScreen();
      }
      Tick = HAL_GetTick() + 100;

      keyState |= (GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) ? (0x01 << 0) : 0x00);

      keyState |= (GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) ? (0x01 << 1) : 0x00);

      keyState |= (GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) ? (0x01 << 2) : 0x00);

      keyState |= (GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) ? (0x01 << 3) : 0x00);

      if (1 == DspMode)
      {
        if (x_axle++ >= 127)
        {
          x_axle = 0;
          //				ScreenFill(0);
        }

        SimulateI2C_ReadByte(i2c2, 0x32, 0x53, 6, buf);

        ScreenDrawLine(x_axle, 20, x_axle, 1, 1, ScreenReverse);
        ScreenDrawLine(x_axle, 10, x_axle, 10 - (((short)(buf[1] << 8) + buf[0]) * 20) / 512, 2, ScreenNoReverse);

        ScreenDrawLine(x_axle, 41, x_axle, 22, 1, ScreenReverse);
        ScreenDrawLine(x_axle, 31, x_axle, 31 - (((short)+(buf[3] << 8) + buf[2]) * 20) / 512, 2, ScreenNoReverse);

        ScreenDrawLine(x_axle, 62, x_axle, 43, 1, ScreenReverse);
        ScreenDrawLine(x_axle, 52, x_axle, 52 - (((short)+(buf[5] << 8) + buf[4]) * 20) / 512, 2, ScreenNoReverse);
        RefreshScreen();
      }
    }
    if (2 == DspMode)
    {
      AveX = 0;
      AveY = 0;
      AveZ = 0;
      for (cnt = 0; cnt < 50; cnt++)
      {
        SimulateI2C_ReadByte(i2c2, 0x32, 0x53, 6, buf);
        AveX += ((short)(buf[1] << 8) + buf[0]);
        AveY += ((short)(buf[3] << 8) + buf[2]);
        AveZ += ((short)(buf[5] << 8) + buf[4]);
      }
      AveX = AveX / 50;
      AveY = AveY / 50;
      AveZ = AveZ / 50;

      // s = a*t^2
      if ((HAL_GetTick() - AccCalc) >= (50 + (uint32_t)(uwTickFreq)))
      {
        AccCalc = HAL_GetTick();

        ScreenFill(0);
        ScreenDrawCircule(63, 31, 10, 2, ScreenNoReverse);
        ScreenDrawLine(0, 31, 127, 31, 2, ScreenReverse);
        ScreenDrawLine(63, 0, 63, 64, 2, ScreenReverse);
        ScreenDrawCircule(ball_x + 63, ball_y + 31, ball_r, ball_degree, ScreenNoReverse);
        RefreshScreen();

        if (0 == AveZ)
        {
          AccX = AveX * cos(atan(AveX / AveZ));
          AccY = AveY * cos(atan(AveY / AveZ));
        }
        else
        {
          AccX = AveX;
          AccY = AveY;
        }

        VelocityX -= ((AccX * 9.8) / 256) * 0.05;
        VelocityY -= ((AccY * 9.8) / 256) * 0.05;

        ball_x += (VelocityX * 500) * 0.05;
        ball_y -= (VelocityY * 500) * 0.05;

        if (ball_x > 64 - ball_r - ball_degree)
        {
          ball_x = 64 - ball_r - ball_degree;
          VelocityX = VelocityX * ElasticAttritionRato * -1;
        }
        else if (ball_x < -64 + ball_r + 1)
        {
          ball_x = -64 + ball_r + 1;
          VelocityX = VelocityX * ElasticAttritionRato * -1;
        }

        if (ball_y < -32 + ball_r + 1)
        {
          ball_y = -32 + ball_r + 1;
          VelocityY = VelocityY * ElasticAttritionRato * -1;
        }
        else if (ball_y > 32 - ball_r - ball_degree)
        {
          ball_y = 32 - ball_r - ball_degree;
          VelocityY = VelocityY * ElasticAttritionRato * -1;
        }
      }
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

#ifdef USE_FULL_ASSERT
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
