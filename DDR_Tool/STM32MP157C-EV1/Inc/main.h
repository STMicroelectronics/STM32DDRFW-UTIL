/**
  ******************************************************************************
  * @file    Templates/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32mp1xx_hal.h"
#include "stm32mp15xx_eval.h"
#include "stm32mp1xx_power.h"

#include "log.h"
#include "ddr_tool.h"

#include "stm32mp_util_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx Pins and clock resources */
#define USARTx_TX_PIN                    UTIL_UART_TX_PIN
#define USARTx_TX_AF                     UTIL_UART_TX_AF
#define USARTx_RX_PIN                    UTIL_UART_RX_PIN
#define USARTx_RX_AF                     UTIL_UART_RX_AF

#if (UTIL_UART_INSTANCE == UTIL_UART4)
  #define USARTx                           UART4
  #define USARTx_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
  #define USARTx_FORCE_RESET()             __HAL_RCC_UART4_FORCE_RESET()
  #define USARTx_RELEASE_RESET()           __HAL_RCC_UART4_RELEASE_RESET()
#elif (UTIL_UART_INSTANCE == UTIL_UART5)
  #define USARTx                           UART5
  #define USARTx_CLK_ENABLE()              __HAL_RCC_UART5_CLK_ENABLE()
  #define USARTx_FORCE_RESET()             __HAL_RCC_UART5_FORCE_RESET()
  #define USARTx_RELEASE_RESET()           __HAL_RCC_UART5_RELEASE_RESET()
#elif (UTIL_UART_INSTANCE == UTIL_UART7)
  #define USARTx                           UART7
  #define USARTx_CLK_ENABLE()              __HAL_RCC_UART7_CLK_ENABLE()
  #define USARTx_FORCE_RESET()             __HAL_RCC_UART7_FORCE_RESET()
  #define USARTx_RELEASE_RESET()           __HAL_RCC_UART7_RELEASE_RESET()
#elif (UTIL_UART_INSTANCE == UTIL_UART8)
  #define USARTx                           UART8
  #define USARTx_CLK_ENABLE()              __HAL_RCC_UART8_CLK_ENABLE()
  #define USARTx_FORCE_RESET()             __HAL_RCC_UART8_FORCE_RESET()
  #define USARTx_RELEASE_RESET()           __HAL_RCC_UART8_RELEASE_RESET()
#else
  #error "unknown UTIL_UART_INSTANCE definition."
#endif

#if (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOA)
  #define USARTx_TX_GPIO_PORT              GPIOA
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOB)
  #define USARTx_TX_GPIO_PORT              GPIOB
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOC)
  #define USARTx_TX_GPIO_PORT              GPIOC
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOD)
  #define USARTx_TX_GPIO_PORT              GPIOD
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOE)
  #define USARTx_TX_GPIO_PORT              GPIOE
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOE_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOF)
  #define USARTx_TX_GPIO_PORT              GPIOF
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOG)
  #define USARTx_TX_GPIO_PORT              GPIOG
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOH)
  #define USARTx_TX_GPIO_PORT              GPIOH
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOH_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOI)
  #define USARTx_TX_GPIO_PORT              GPIOI
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOJ)
  #define USARTx_TX_GPIO_PORT              GPIOJ
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOJ_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOK)
  #define USARTx_TX_GPIO_PORT              GPIOK
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOK_CLK_ENABLE()
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOZ)
  #define USARTx_TX_GPIO_PORT              GPIOZ
  #define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOZ_CLK_ENABLE()
#else
  #error "unknown UTIL_UART_TX_GPIO_PORT definition."
#endif

#if (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOA)
  #define USARTx_RX_GPIO_PORT              GPIOA
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOB)
  #define USARTx_RX_GPIO_PORT              GPIOB
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOC)
  #define USARTx_RX_GPIO_PORT              GPIOC
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOD)
  #define USARTx_RX_GPIO_PORT              GPIOD
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOE)
  #define USARTx_RX_GPIO_PORT              GPIOE
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOE_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOF)
  #define USARTx_RX_GPIO_PORT              GPIOF
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOG)
  #define USARTx_RX_GPIO_PORT              GPIOG
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOH)
  #define USARTx_RX_GPIO_PORT              GPIOH
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOH_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOI)
  #define USARTx_RX_GPIO_PORT              GPIOI
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOJ)
  #define USARTx_RX_GPIO_PORT              GPIOJ
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOJ_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOK)
  #define USARTx_RX_GPIO_PORT              GPIOK
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOK_CLK_ENABLE()
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOZ)
  #define USARTx_RX_GPIO_PORT              GPIOZ
  #define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOZ_CLK_ENABLE()
#else
  #error "unknown UTIL_UART_RX_GPIO_PORT definition."
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
