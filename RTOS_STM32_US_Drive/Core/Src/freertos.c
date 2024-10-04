/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern uint8_t distance_1;
extern uint8_t distance_2;
extern uint8_t distance_3;
extern uint8_t dutyValue_Move;
extern int speedvalue;
extern int pwm_duty;
extern int count;
int F_count;
uint8_t buffer[3];
extern uint8_t rxData[1];
int Flag;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UltraSonic */
osThreadId_t UltraSonicHandle;
const osThreadAttr_t UltraSonic_attributes = {
  .name = "UltraSonic",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Motor */
osThreadId_t MotorHandle;
const osThreadAttr_t Motor_attributes = {
  .name = "Motor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void HCSR04_C(void)
{
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, 0);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, 1);
  delay_us(10);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, 0);
  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1); // __HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1); 와 같이 없어도 되지만.
}
void HCSR04_L(void)
{
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2, 0);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2, 1);
  delay_us(10);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2, 0);
  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC2); // __HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1); 와 같이 없어도 되지만.
}
void HCSR04_R(void)
{
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, 0);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, 1);
  delay_us(10);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, 0);
  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC3); // __HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1); 와 같이 없어도 되지만.
}



/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void UltraSonic_Task(void *argument);
void Car_move(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of UltraSonic */
  UltraSonicHandle = osThreadNew(UltraSonic_Task, NULL, &UltraSonic_attributes);

  /* creation of Motor */
  MotorHandle = osThreadNew(Car_move, NULL, &Motor_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_UltraSonic_Task */
/**
* @brief Function implementing the UltraSonic thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UltraSonic_Task */
void UltraSonic_Task(void *argument)
{
  /* USER CODE BEGIN UltraSonic_Task */
  /* Infinite loop */
  for(;;)
  {
    HCSR04_C();
    buffer[0] = distance_1;
    HAL_Delay(10);
    HCSR04_L();
    buffer[1] = distance_2;
    HAL_Delay(10);
    HCSR04_R();
    buffer[2] = distance_3;
    HAL_Delay(10);
    __HAL_TIM_SET_COUNTER(&htim4 , 0);
    osDelay(1);
  }
  /* USER CODE END UltraSonic_Task */
}

/* USER CODE BEGIN Header_Car_move */
/**
* @brief Function implementing the Motor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Car_move */
void Car_move(void *argument)
{
  /* USER CODE BEGIN Car_move */
  /* Infinite loop */
  for(;;)
  {
    if(Flag == 0)
    {
      if(rxData[0] == 75/*'i'*/ || rxData[0] == 51/*'f'*/ || rxData[0] == 19/*'b'*/ || rxData[0] == 99/*'l'*/ || rxData[0] == 147/*'r'*/ || rxData[0] == 155/*'s'*/ || rxData[0] == 123/*'o'*/)
          {
            dutyValue_Move = rxData[0];
            switch(dutyValue_Move)
            {
              case 75/*'i'*/:
                Speed(0);
                idle();
                break;
              case 51/*'f'*/:
                Speed(700);
                forward();
                break;
              case 19/*'b'*/:
                Speed(pwm_duty);
                backward();
                break;
              case 99/*'l'*/:
                Speed(pwm_duty);
                left();
                break;
              case 147/*'r'*/:
                Speed(pwm_duty);
                right();
              case 155/*'s'*/:
                Flag = 0;
                count++;
                if(count == 1)
                {
                  pwm_duty = 350;
                }
                else if(count == 2)
                {
                  pwm_duty = 500;
                }
                else if(count == 3)
                {
                  pwm_duty = 950;
                }
                else count = 0;
                break;
              case 123:
                Flag = 1;
                break;
              default:
                break;
            }
          }
       }

    if(Flag == 1)
    {
      if(rxData[0] == 123/*'o'*/ || rxData[0] == 155)
      {
        dutyValue_Move = rxData[0];
        switch(dutyValue_Move)
        {
          case 155:
            Flag = 0;
            break;
          default:
            break;
        }
      }

      if(distance_1 > 50)
      {
        Speed(800);
        forward();

      }
      else if(distance_1 < 15)
      {
        Speed(500);
        if(distance_2 > distance_3)
        {
          left();
        }
        else
        {
          right();
        }
      }

      if(distance_2 < 11)
      {
        Speed(600);
        right();
        if(distance_2 < 5 && distance_1 < 5)
        {
          Speed(300);
          backward();
        }
      }
      if(distance_3 < 11)
      {
        Speed(600);
        left();
        if(distance_3 < 5 && distance_1 < 5)
        {
          Speed(300);
          backward();
        }
      }
    }
    osDelay(1);
  }
  /* USER CODE END Car_move */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

