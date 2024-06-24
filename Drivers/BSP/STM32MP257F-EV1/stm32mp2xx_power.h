/**
  ******************************************************************************
  * @file    Templates/Inc/stm32mp2xx_power.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the power services.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32MP2xx_POWER_H
#define __STM32MP2xx_POWER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void SystemPower_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32MP2xx_POWER_H */
