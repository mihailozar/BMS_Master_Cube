/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

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
#define Uhvacc_Measurement_Pin GPIO_PIN_0
#define Uhvacc_Measurement_GPIO_Port GPIOC
#define Ihvacc_Measurement_Pin GPIO_PIN_1
#define Ihvacc_Measurement_GPIO_Port GPIOC
#define CloseAirPlus_Pin GPIO_PIN_2
#define CloseAirPlus_GPIO_Port GPIOC
#define CloseAirMinus_Pin GPIO_PIN_3
#define CloseAirMinus_GPIO_Port GPIOC
#define AirPlusStatus_Pin GPIO_PIN_6
#define AirPlusStatus_GPIO_Port GPIOA
#define AirMinusStatus_Pin GPIO_PIN_7
#define AirMinusStatus_GPIO_Port GPIOA
#define Precharge_Relay_Status_Pin GPIO_PIN_5
#define Precharge_Relay_Status_GPIO_Port GPIOC
#define SHD_Status_Pin GPIO_PIN_6
#define SHD_Status_GPIO_Port GPIOC
#define Prechrage_Control_Pin GPIO_PIN_7
#define Prechrage_Control_GPIO_Port GPIOC
#define bmsFault_Pin GPIO_PIN_8
#define bmsFault_GPIO_Port GPIOC
#define bmsWakeUp_Pin GPIO_PIN_9
#define bmsWakeUp_GPIO_Port GPIOC
#define BMS_SHD_Control_Pin GPIO_PIN_8
#define BMS_SHD_Control_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
