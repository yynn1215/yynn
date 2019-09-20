/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "MPU9250.h"
#include "I2C_1.h"
#include "tm_stm32_ahrs_imu.h"
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
uint16_t adcval[2];
uint8_t adcIndex=0;
MPU9250_t MPU9250;
float filtered_Az;
TM_AHRSIMU_t IMU;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the Debug USART and Loop until the end of transmission */

    if('\n' == ch)
    {
        int ch_tmp = '\r';

        /* Place your implementation of fputc here */
        /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
        HAL_UART_Transmit(&huart1, (uint8_t *)&ch_tmp, 1, 0xFFFF);
    }

    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
   */
#if 0
	  adcval[adcIndex]=HAL_ADC_GetValue(&hadc3);

	  if(adcIndex == 1)
	  {
	 // TIM4->CCR4=2600+(((uint16_t)adcval[0])/3);
	  //printf("%d\n",((uint16_t)adcval[0])/3);
	  if(adcval[1]<1900) //앞으로
	  {
		  //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
		  TIM1->CCR2=500;
	  }
	  if(adcval[1]>2200) //뒤로
	  {
		  //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,0);
		  TIM1->CCR2=800;
	  }
	  if((adcval[1]>=1900) && (adcval[1]<=2200))
	  {
		  TIM1->CCR2=0;
	  }
	  //printf("%d\n",adcval[1]);
	  //printf("test\n");
	  }
	  adcIndex++;
	  if(adcIndex > 1) adcIndex=0;
#endif
	  HAL_ADC_Start_IT(&hadc3);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  //MX_DMA_Init();
  //MX_ADC3_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  //HAL_ADC_Start_DMA(&hadc3,&adcval[0], sizeof(adcval));
	printf("Start\n");
	if (MPU9250_Init(&hi2c1, &MPU9250, MPU9250_Device_0) != MPU9250_Result_Ok) {
  printf("Device error!\r\n");}
	TM_AHRSIMU_Init(&IMU, 1000, 0.5, 0);
  //HAL_ADC_Start_IT(&hadc3);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
  HAL_TIM_PWM_Start_IT(&htim4,TIM_CHANNEL_4);
  TIM4->CCR4=2700;
  TIM1->CCR2=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	 // HAL_Delay(5);
	  if (MPU9250_DataReady(&hi2c1, &MPU9250) == MPU9250_Result_Ok) {
	  				MPU9250_ReadAcce(&hi2c1, &MPU9250);
	          MPU9250_ReadGyro(&hi2c1, &MPU9250);
	          MPU9250_ReadMag(&hi2c1, &MPU9250);
	  		}
	  TM_AHRSIMU_UpdateIMU(&IMU, MPU9250.Gx, MPU9250.Gy, MPU9250.Gz, MPU9250.Ax, MPU9250.Ay, MPU9250.Az);
#if 1

	  if(IMU.Pitch>25)
	  {
		  //TIM4->CCR4=3250+((int8_t)IMU.Pitch*15*-1);
		  TIM1->CCR2=700;
		  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,1);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,0);
	  }
	  else if(IMU.Pitch<-25)
	  {
		  //TIM4->CCR4=3250+((int8_t)IMU.Pitch*15*-1);
		  TIM1->CCR2= 1;
		  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,0);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
	  }
	  //else if(IMU.Pitch>=20 && IMU.Pitch<=-20)
	  else
	  {
		  TIM4->CCR4=3250;
		  TIM1->CCR2=0;
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
	  }
#endif

#if 1

#endif
	  printf("R: %.0f, P: %.0f, Y: %.0f\n", IMU.Roll, IMU.Pitch, IMU.Yaw);
	  //printf("%d\n",((int8_t)IMU.Pitch*15)*-1);
	  //printf("R: %d\n",(int8_t)IMU.Roll);

	  //HAL_Delay(20);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 // printf("ab ");
	  //HAL_ADC_Start(&hadc3);
	 // if((HAL_ADC_PollForConversion(&hadc3, 1000000))==HAL_OK)
	 // {
	//	  adcval=HAL_ADC_GetValue(&hadc3);
	 // }
//	  else
//	  {
		  //continue;
//	  }
	  //printf("%d\n",(adcval)/3);
	//  HAL_Delay(1000);
	 // TIM4->CCR4=2600+(((uint16_t)adcval)/3);
	  //HAL_Delay(10);
	 // printf("test\n");


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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
