/**
  ******************************************************************************
  * @file    stm32mp_util_ddr_conf.h
  * @author  MCD Application Team
  * @brief   Header file of util DDR configuration for STM32MP2xx.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32MP_UTIL_DDR_CONF_H
#define __STM32MP_UTIL_DDR_CONF_H

#if (STM32MP_LPDDR4_TYPE && (DDR_SIZE_Gb == 16))
  #include "stm32mp2xx-lpddr4-1x16Gbits-1x16bits-template.h"
#else
  #error "no DDR settings defined."
#endif

#endif /* __STM32MP_UTIL_DDR_CONF_H */
