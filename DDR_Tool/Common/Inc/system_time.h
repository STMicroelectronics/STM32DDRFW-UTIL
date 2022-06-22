/**
  ******************************************************************************
  * @file    Templates/Inc/system_time.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the time services.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_TIME_H
#define __SYSTEM_TIME_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_device_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void delay_us(unsigned long delay_us);
__IO uint32_t timeout_init_us(unsigned long timeout_us);
bool timeout_elapsed(__IO uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_TIME_H */
