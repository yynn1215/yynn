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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hcsr04.h"
#include "uart_test.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLAG 	0
#define ROLL	1
#define PITCH	2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t adcIndex=0;
uint16_t adcval[2];
TM_HCSR04_t hcsr04;
uint32_t hcsr04_distance;
int voice_key;
int i=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
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


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
   */
	if(adcIndex == 0)
	{adcval[adcIndex]=HAL_ADC_GetValue(&hadc3);}
	else if(adcIndex == 1)
	{
		adcval[1]=HAL_ADC_GetValue(&hadc3);
#if 1
		if((band_info[FLAG]!=1)&& (band_info[FLAG]!=2))
		{
			  if(hcsr04.Distance>=1)
			  {
				  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  }
			TIM4->CCR4=2600+(((uint16_t)adcval[0])/3);
			//printf("%d\n",((uint16_t)adcval[0])/3);
			if(adcval[1]<1900) //앞으로
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
				TIM1->CCR2=600;
				  if(hcsr04.Distance<1)
				  {
					  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
				  }
			}
			else if(adcval[1]>2250) //뒤로
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,0);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);

				TIM1->CCR2=600;
			}
			else
			{
				TIM1->CCR2=0;
			}
			//printf("%d\n",adcval[1]);
			//printf("test\n");
		}
#endif
	}
	adcIndex++;
	if(adcIndex == 2)
	{
		adcIndex=0;

	}

	HAL_ADC_Start_IT(&hadc3);
}

void HAL_Delay1(uint32_t Delay)
{
  uint32_t tickstart1 = HAL_GetTick();


  /* Add a freq to guarantee minimum wait */

  while((HAL_GetTick() - tickstart1) < Delay)
  {
	  TM_HCSR04_Read(&hcsr04);
	  //printf("%f\n",hcsr04.Distance);
	  if((hcsr04.Distance<1) && (voice_key==1) )
	  {
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
	  	break;
	  }
  }
}




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
	band_info[FLAG]=3;
	band_info[ROLL]=0;

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
  MX_ADC3_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
  HAL_TIM_PWM_Start_IT(&htim4,TIM_CHANNEL_4);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
  if(HAL_UART_Receive_IT(&huart1, g_Uart_RxBuffer, UART_RXBUFFERSIZE) != HAL_OK)
  {
      Error_Handler();
  }

  if(HAL_UART_Receive_IT(&huart6, g_Uart_6_Com_RxBuffer, UART_RXBUFFERSIZE) != HAL_OK)
  {
      Error_Handler();
  }

  TIM4->CCR4=3250;

  HAL_ADC_Start_IT(&hadc3);


  printf("Start\n");

	if(TM_HCSR04_Init(&hcsr04, GPIOE,GPIO_PIN_15, GPIOE,GPIO_PIN_14 )!=1)
	{
		printf("hc error!\r\n");
	}

    printf("printf test\n");
    HAL_UART_Transmit(&huart6, (uint8_t *)"test", strlen("test"), 0xFFFF);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  UART6_Test();
	  TM_HCSR04_Read(&hcsr04);
	  //printf("%f\n",hcsr04.Distance);
#if 1
	  if(hcsr04.Distance>=1)
	  {
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
	  }
#endif
	  if(band_info[FLAG]==1)
	  {
		  UART6_Test();
		  if(band_info[PITCH]>15)
			  //if(band_info[PITCH]>25)
		  {
			  TIM4->CCR4=3250+((int16_t)band_info[PITCH]*15*-1);
			  //TIM1->CCR2=500;
			  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,1);
			  //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,0);
		  }
		  else if(band_info[PITCH]<-15)
			  //if(band_info[PITCH]<-25)
		  {
			  TIM4->CCR4=3250+((int16_t)band_info[PITCH]*15*-1);
			  //TIM1->CCR2= 500;
			  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,0);
			  //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
		  }
		  else
		  {
			  TIM4->CCR4=3250;
			  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
		  }
		  //else if(IMU.Pitch>=20 && IMU.Pitch<=-20)
		  UART6_Test();
		  if(band_info[ROLL] <150 && band_info[ROLL]>90)
			  //if(band_info[ROLL]<150 && band_info[ROLL]>90)
		  {
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  TIM1->CCR2=400;
			  if(hcsr04.Distance<1)
			  {
				  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  }

			  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,1);

		  }
		  //if(band_info[ROLL]>-150 && band_info[ROLL]<-90)
		  else if(band_info[ROLL] > -150 && band_info[ROLL] <-90)
		  {
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,0);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  TIM1->CCR2= 400;
			  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,0);

		  }
		  else
		  {
			  TIM1->CCR2=0;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
		  }

	  }

	  else if(band_info[FLAG]==2)
	  {
		  TIM1->CCR2=0;
		  voice_key=band_info[ROLL];
		  if(voice_key==1)
		  {
			  TIM4->CCR4=3250;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
			  TIM1->CCR2=400;
			  HAL_Delay1(3000);
//			  if(hcsr04.Distance<1)
//			  {
//				  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
//			  }
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  band_info[ROLL]=5;
		  }
		  else if(voice_key==2)
		  {
			  TIM4->CCR4=3250;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,0);
			  TIM1->CCR2=500;
			   HAL_Delay(2300);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  band_info[ROLL]=5;
		  }
		  else if(voice_key==3)
		  {
			  TIM4->CCR4=3250+880;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
			  TIM1->CCR2=550;
			  HAL_Delay1(2200);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  TIM4->CCR4=3250;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  HAL_Delay1(1000);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  band_info[ROLL]=5;

		  }
		  else if(voice_key==4)
		  {
			  TIM4->CCR4=3250-880;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,1);
			  TIM1->CCR2=550;
			  HAL_Delay1(2200);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  TIM4->CCR4=3250;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,0);
			  HAL_Delay1(1000);
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  band_info[ROLL]=5;

		  }
		  else if(voice_key==5)
		  {
			  TIM1->CCR2=0;
			  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,1);
			  band_info[FLAG]=3;


		  }

	  }
	  //printf("0: %d ,1: %d , %f \n",adcval[0],adcval[1],hcsr04.Distance);
	  //printf("%f\n",hcsr04.Distance);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
