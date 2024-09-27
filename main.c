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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
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
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rxData[8];
int speedvalue;
//volatile uint16_t joyStick[3];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  if(huart-> Instance == USART6)
//  {
//    HAL_UART_Transmit_IT(&huart6, rxData, 1);
//    HAL_UART_Receive_IT(&huart6, rxData, 1);
//  }
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
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_UART_Receive_IT( &huart6, rxData, 1);
//  HAL_ADC_Start_DMA(&hadc1, joyStick, 3);
  HAL_UART_Receive_DMA(&huart6, rxData, sizeof(rxData));
  PWM_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t dutyValue_Move = 0;
  uint8_t dutyValue_Speed = 0;
//  uint16_t joystickValue0 = 0;
//  uint16_t joystickValue1 = 0;
//  uint16_t joystickValue2 = 0;
  while (1)
  {
    if(rxData[6] == 0 || rxData[6] == 1 || rxData[6] == 2 || rxData[6] == 4 || rxData[6] == 8)
    {
      dutyValue_Move = rxData[6];
      switch(dutyValue_Move)
      {
        case 0:
          Speed(speedvalue);
          idle();
          break;
        case 1:
          Speed(speedvalue);
          forward();
          break;
        case 2:
          Speed(speedvalue);
          backward();
          break;
        case 4:
          Speed(speedvalue);
          left();
          break;
        case 8:
          Speed(speedvalue);
          right();
        default:
          break;
      }
   }

    if(rxData[5] == 4 || rxData[5] == 32 || rxData[5] ==16 || rxData[5] ==8)
    {
      dutyValue_Speed = rxData[5];

      switch(dutyValue_Speed)
      {
        case 4:
          speedvalue = 400;
          break;
        case 32:
          speedvalue = 550;
          break;
        case 16:
          speedvalue = 700;
          break;
        case 8:
          speedvalue = 950;
          break;
        default:
          break;
      }
    }
    /*
    HAL_Delay(500);
    printf("adc -> LeftX: %d\r\n", joyStick[0]);
    printf("adc -> RightY : %d\r\n", joyStick[1]);
    printf("adc -> LeftSW : %d\r\n", joyStick[2]);

    joystickValue0 = joyStick[0];
    joystickValue1 = joyStick[1];
    joystickValue2 = joyStick[2];
    if(joystickValue0 >= 3069 || joystickValue1 <= 3081)
    {
      Speed(0);
      move(0);
    }
    if(joyStick[1] >= 3100)
    {
      Speed(600);
      move(1);//전진
    }
    if(joystickValue1 <= 2000)
    {
      Speed(600);
      move(3);//후진
    }
    if(joystickValue0 >= 3080 )
    {
      Speed(600);
      move(4);//좌
    }
    if(joystickValue0 <= 2000)
    {
      Speed(600);
      move(2);//우
    }*/

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
