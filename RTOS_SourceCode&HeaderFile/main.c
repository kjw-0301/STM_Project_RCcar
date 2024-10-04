/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "cmsis_os.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

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
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHARPROTOTYPE int fputc(int ch, FILE *f)
#endif // __GNUC /

PUTCHAR_PROTOTYPE
{
  // Place your implementation of fputc here /
  // e.g. write a character to the EVAL_COM1 and Loop until the end of transmission /
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t IC_value1_1 = 0;
uint16_t IC_value2_1 = 0;

uint16_t IC_value1_2 = 0;
uint16_t IC_value2_2 = 0;

uint16_t IC_value1_3 = 0;
uint16_t IC_value2_3 = 0;

uint16_t echoTime_1 = 0;
uint16_t echoTime_2 = 0;
uint16_t echoTime_3 = 0;

uint8_t captureFlag_1 = 0;
uint8_t captureFlag_2 = 0;
uint8_t captureFlag_3 = 0;

uint8_t distance_1 = 0;
uint8_t distance_2 = 0;
uint8_t distance_3 = 0;
uint8_t rxData[1];
int speedvalue;
int pwm_duty;
int count = 0;
uint8_t dutyValue_Move = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    if(captureFlag_1 == 0)
    {
      IC_value1_1 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1); //상승엣지일때 IC_value1에 그 값을 할당.
      captureFlag_1 = 1;
      //상승엣지 읽은 후 하강엣지인식하기 위한 세팅
      __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }
    else if(captureFlag_1 == 1)
    {
      IC_value2_1 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
      //__HAL_TIM_SET_COUNTER(&htim4 , 0);
      if(IC_value2_1 > IC_value1_1)
      {
        echoTime_1 = IC_value2_1 - IC_value1_1;
      }
      else if(IC_value1_1 > IC_value2_1) // 이 경우는 타이머가 16비트 이기 때문에 65535까지 카운트가 가능하다. 만약 IC_value1과 IC_value2 사이에  타이머 카운터의 오버플로우가 발생할 경우 계산.
      {
        echoTime_1 = (0xffff - IC_value1_1) + IC_value2_1;
      }
      distance_1 = echoTime_1 / 58;
      captureFlag_1 = 0;
      __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);
    }
  }

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      if(captureFlag_2 == 0)
      {
        IC_value1_2 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2); //상승엣지일때 IC_value1에 그 값을 할당.
        captureFlag_2 = 1;
        //상승엣지 읽은 후 하강엣지인식하기 위한 세팅
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
      }
      else if(captureFlag_2 == 1)
      {
        IC_value2_2 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2);
        //__HAL_TIM_SET_COUNTER(&htim4 , 0);
        if(IC_value2_2 > IC_value1_2)
        {
          echoTime_2 = IC_value2_2 - IC_value1_2;
        }
        else if(IC_value1_2 > IC_value2_2) // 이 경우는 타이머가 16비트 이기 때문에 65535까지 카운트가 가능하다. 만약 IC_value1과 IC_value2 사이에  타이머 카운터의 오버플로우가 발생할 경우 계산.
        {
          echoTime_2 = (0xffff - IC_value1_2) + IC_value2_2;
        }
        distance_2 = echoTime_2 / 58;
        captureFlag_2 = 0;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
        __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC2);
      }
    }

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
      {
        if(captureFlag_3 == 0)
        {
          IC_value1_3 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_3); //상승엣지일때 IC_value1에 그 값을 할당.
          captureFlag_3 = 1;
          //상승엣지 읽은 후 하강엣지인식하기 위한 세팅
          __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else if(captureFlag_3 == 1)
        {
          IC_value2_3 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_3);
          //__HAL_TIM_SET_COUNTER(&htim4 , 0);
          if(IC_value2_3 > IC_value1_3)
          {
            echoTime_3 = IC_value2_3 - IC_value1_3;
          }
          else if(IC_value1_3 > IC_value2_3) // 이 경우는 타이머가 16비트 이기 때문에 65535까지 카운트가 가능하다. 만약 IC_value1과 IC_value2 사이에  타이머 카운터의 오버플로우가 발생할 경우 계산.
          {
            echoTime_3 = (0xffff - IC_value1_3) + IC_value2_3;
          }
          distance_3 = echoTime_3 / 58;
          captureFlag_3 = 0;
          __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
          __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC3);
        }
      }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart-> Instance == USART6)
  {
    HAL_UART_Receive_IT(&huart6, rxData, 1);
  }
}
//void HCSR04(void)
//{
////  HAL_GPIO_WritePin(TRIG_PORT,TRIG_PIN, 0);
////  HAL_Delay(1);
//  HAL_GPIO_WritePin(TRIG_PORT,TRIG_PIN, 1);
//  delay_us(10);
//  HAL_GPIO_WritePin(TRIG_PORT,TRIG_PIN, 0);
//  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1); // __HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1); 와 같이 없어도 되지만.
//}

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
  MX_TIM3_Init();
  MX_TIM10_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim10);//딜레이만들기위함
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);//초음파센서의 상승엣지와 하강엣지 INPUT캡쳐를 하기위한 카운터 인터럽트
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);
  HAL_UART_Receive_IT(&huart6, rxData, 1);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM11) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
