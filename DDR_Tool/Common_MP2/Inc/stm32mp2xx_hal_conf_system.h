/**
  ******************************************************************************
  * @file    stm32mp2xx_hal_conf_system.h
  * @author  MCD Application Team
  * @version $VERSION$
  * @date    $DATE$
  * @brief   HAL drivers default system config.
  * This file is included by BSPs stm32mp257cxx_clocks.h
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
#ifndef STM32MP2xx_HAL_CONF_SYSTEM_H
#define STM32MP2xx_HAL_CONF_SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */
#define  VDD_VALUE            3300U  /*!< Value of VDD in mv */
#if !defined(TICK_INT_PRIORITY)
  #ifdef CORE_CA35
    #define  TICK_INT_PRIORITY    (((uint32_t)1<<(GIC_PRIO_BITS-1)) - 1)  /*!< tick interrupt priority set by default */
                                                                      /*!< to GIC400 lowest (highest value) in secure mode */
  #else
    #define  TICK_INT_PRIORITY    (((uint32_t)1<<__NVIC_PRIO_BITS) - 1)   /*!< tick interrupt priority set by default */
                                                                      /*!< to NVIC lowest (highest value)         */
                                                                      /*  Warning: Must be set to higher priority for HAL_Delay() */
                                                                      /*  and HAL_GetTick() usage under interrupt context         */
  #endif
#endif /* ! TICK_INT_PRIORITY */

#if !defined(USE_RTOS)
#define  USE_RTOS                     0U
#endif /* ! USE_RTOS */
#if !defined(PREFETCH_ENABLE)
#define  PREFETCH_ENABLE              0U
#endif /* ! PREFETCH_ENABLE */
#if !defined(INSTRUCTION_CACHE_ENABLE)
#define  INSTRUCTION_CACHE_ENABLE     0U
#endif /* ! INSTRUCTION_CACHE_ENABLE */
#if !defined(DATA_CACHE_ENABLE)
#define  DATA_CACHE_ENABLE            0U
#endif /* ! DATA_CACHE_ENABLE */

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
#if !defined(USE_FULL_ASSERT)
  #define USE_FULL_ASSERT    1
#endif /* ! USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* STM32MP2xx_HAL_CONF_SYSTEM_H */
