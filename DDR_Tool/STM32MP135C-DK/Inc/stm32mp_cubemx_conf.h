/**
  ******************************************************************************
  * @file    stm32mp_cubemx_conf.h
  * @author  MCD Application Team
  * @brief   Header file of STM32CubeMX board configuration for STM32MP13xx.
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
#ifndef __STM32MP13XX_CUBEMX_CONF_H
#define __STM32MP13XX_CUBEMX_CONF_H

#include "stm32mp_cubemx_def.h"
#include "stm32mp13xx_hal.h"

#define MX_MODEL         "STM32MP13XX_DK"

/* Clock related definitions */
#define MX_PLL2_M         3
#define MX_PLL2_N         66
#define MX_PLL2_P         2
#define MX_PLL2_Q         2
#define MX_PLL2_R         1
#define MX_PLL2_FRACV     0x1400
#define MX_PLL2_MODE      RCC_PLL_FRACTIONAL
#define MX_PLL2_DIVN_MIN  24
#define MX_PLL2_DIVN_MAX  199

/* UART related configuration */
#define MX_UART_INSTANCE        MX_UART4
#define MX_UART_TX_PIN          GPIO_PIN_6
#define MX_UART_TX_GPIO_PORT    MX_GPIOD
#define MX_UART_TX_AF           GPIO_AF8_UART4
#define MX_UART_RX_PIN          GPIO_PIN_8
#define MX_UART_RX_GPIO_PORT    MX_GPIOD
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

#endif /* __STM32MP13XX_CUBEMX_CONF_H */
