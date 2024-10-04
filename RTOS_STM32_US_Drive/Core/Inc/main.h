/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Left_Usonic_GPIO_output_Pin GPIO_PIN_2
#define Left_Usonic_GPIO_output_GPIO_Port GPIOC
#define Right_Usonic_GPIO_output_Pin GPIO_PIN_3
#define Right_Usonic_GPIO_output_GPIO_Port GPIOC
#define Center_Usonic_GPIO_output_Pin GPIO_PIN_7
#define Center_Usonic_GPIO_output_GPIO_Port GPIOA
#define TIM4_CH1_Center_link_PA7_Pin GPIO_PIN_6
#define TIM4_CH1_Center_link_PA7_GPIO_Port GPIOB
#define TIM4_CH2_Left_link_PA7_Pin GPIO_PIN_7
#define TIM4_CH2_Left_link_PA7_GPIO_Port GPIOB
#define TIM4_CH3_Right_link_PA7_Pin GPIO_PIN_8
#define TIM4_CH3_Right_link_PA7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
