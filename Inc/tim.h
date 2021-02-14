/**
  ******************************************************************************
  * File Name          : TIM.h
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "display/display_info.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/**
 * TIM2 is used for PWM generation
 * The overflow time is calculated from desired fps.
 * Time for OC output should be calculated from this value.
 */

#define TIM2_CLK_FREQ       (72000000ul)
#define TIM2_OVF_PER_FRAME  (NW_DISPLAY_SCAN_DIVIDER * NW_DISPLAY_DEPTH)
#define TIM2_OVF_PER_SECOND (TIM2_OVF_PER_FRAME * NW_DESIRED_FPS)
#define TIM2_RELOAD         (TIM2_CLK_FREQ / TIM2_OVF_PER_SECOND)

/* USER CODE END Private defines */

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
