/**
  ******************************************************************************
  * @file    Templates/Src/stm32mp2xx_power.c
  * @author  MCD Application Team
  * @brief   Main Power Service Routines.
  *          This file provides template for power services.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32mp257f_disco.h"
#include "stm32mp257f_disco_pmic.h"
#include "stm32mp2xx_power.h"
#include "stm32mp_util_conf.h"

/** @addtogroup STM32MP2xx_HAL_Examples
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
  * @brief  This function handles System Power configuration.
  * @param  None
  * @retval None
  */
void SystemPower_Config(void)
{
#if (UTIL_USE_PMIC)
  /* Probe and initialize PMIC device */
  if (BSP_PMIC_Init() != BSP_ERROR_NONE)
  {
	  printf("ERROR: STPMIC initialization KO \n\r");
  }
#endif /* UTIL_USE_PMIC */
}

/**
  * @}
  */

/**
  * @}
  */
