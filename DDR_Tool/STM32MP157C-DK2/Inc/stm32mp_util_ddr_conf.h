/**
  ******************************************************************************
  * @file    stm32mp_util_conf.h
  * @author  MCD Application Team
  * @brief   Header file of util DDR configuration for STM32MP1xx.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32MP1XX_UTIL_DDR_CONF_H
#define __STM32MP1XX_UTIL_DDR_CONF_H

#if defined DDR_TYPE_DDR3_4Gb
  #include "stm32mp1xx-ddr3-4Gb-template.h"
#elif defined DDR_TYPE_DDR3_8Gb
  #include "stm32mp1xx-ddr3-8Gb-template.h"
#else
  #error "no DDR settings defined."
#endif

#endif /* __STM32MP1XX_UTIL_DDR_CONF_H */
