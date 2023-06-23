/**
  ******************************************************************************
  * @file    Templates/Src/stm32mp13xx_power.c
  * @author  MCD Application Team
  * @brief   Main Power Service Routines.
  *          This file provides template for power services.
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
#include "stm32mp13xx_disco_stpmic1.h"
#include "stm32mp13xx_power.h"
#include "log.h"
#include "main.h"

/** @addtogroup STM32MP13xx_HAL_Examples
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
  /* PMIC initialization */
  /***********************/
  log_dbg("PMIC initialization \n\r");
  BSP_PMIC_Init();

  BSP_PMIC_InitRegulators();
}

/**
  * @}
  */

/**
  * @}
  */
