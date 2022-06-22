/**
  ******************************************************************************
  * @file    system_time.c
  * @author  MCD Application Team
  * @brief   Main Time Service Routines.
  *          This file provides template for time services.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "system_time.h"

/** @addtogroup STM32MP1xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function handles delay service.
  * @param  delay_us: delay in microseconds
  * @retval None
  */
void delay_us(unsigned long delay_us)
{
  __IO uint32_t wait_loop_index = 0U;

  wait_loop_index = (delay_us * (SystemCoreClock / (1000000UL * 2UL)));

  while(wait_loop_index != 0UL)
  {
    wait_loop_index--;
  }
}

/**
  * @brief  This function handles timeout initialization service.
  * @param  delay_us: timeout in microseconds
  * @retval None
  */
__IO uint32_t timeout_init_us(unsigned long timeout_us)
{
  __IO uint32_t wait_loop_index = 0U;

  wait_loop_index = timeout_us * (SystemCoreClock / (1000000UL * 2UL));

  return wait_loop_index;
}

/**
  * @brief  This function handles timeout elapsed checking service.
  * @param  delay_us: timeout in microseconds
  * @retval None
  */
bool timeout_elapsed(__IO uint32_t timeout)
{
  if (timeout == 0U)
  {
    return true;
  }

  return false;
}

/**
  * @}
  */

/**
  * @}
  */
