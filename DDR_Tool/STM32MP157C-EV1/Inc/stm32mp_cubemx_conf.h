/**
  ******************************************************************************
  * @file    stm32mp_cubemx_conf.h
  * @author  MCD Application Team
  * @brief   Header file of STM32CubeMX board configuration for STM32MP1xx.
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
#ifndef __STM32MP1XX_CUBEMX_CONF_H
#define __STM32MP1XX_CUBEMX_CONF_H

#include "stm32mp_cubemx_def.h"
#include "stm32mp1xx_hal.h"

#define MX_MODEL         "STM32MP15XX"

/* Clock related definitions */
#define MX_PLL2_M         2
#define MX_PLL2_N         44
#define MX_PLL2_P         2
#define MX_PLL2_Q         1
#define MX_PLL2_R         1
#define MX_PLL2_FRACV     0
#define MX_PLL2_MODE      RCC_PLL_INTEGER
#define MX_PLL2_DIVN_MIN  24
#define MX_PLL2_DIVN_MAX  199

/* UART related configuration */
#define MX_UART_INSTANCE        MX_UART4
#define MX_UART_TX_PIN          GPIO_PIN_11
#define MX_UART_TX_GPIO_PORT    MX_GPIOG
#define MX_UART_TX_AF           GPIO_AF6_UART4
#define MX_UART_RX_PIN          GPIO_PIN_2
#define MX_UART_RX_GPIO_PORT    MX_GPIOB
#define MX_UART_RX_AF           GPIO_AF8_UART4
#define MX_UART_BAUDRATE        115200
#define MX_UART_WORDLENGTH      UART_WORDLENGTH_8B
#define MX_UART_STOPBITS        UART_STOPBITS_1
#define MX_UART_PARITY          UART_PARITY_NONE
#define MX_UART_HWFLOWCTL       UART_HWCONTROL_NONE
#define MX_UART_MODE            UART_MODE_TX_RX
#define MX_UART_OVERSAMPLING    UART_OVERSAMPLING_16

/* PMIC related configuration */
#define MX_USE_PMIC         1
#define MX_PMIC_BUCK2_MV    1350
#define MX_PMIC_LDO3_MV     500

#endif /* __STM32MP1XX_CUBEMX_CONF_H */
