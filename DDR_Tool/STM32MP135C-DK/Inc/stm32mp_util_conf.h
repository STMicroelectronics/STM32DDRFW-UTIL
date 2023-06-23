/**
  ******************************************************************************
  * @file    stm32mp_util_conf.h
  * @author  MCD Application Team
  * @brief   Header file of board configuration for STM32MP13xx.
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
#ifndef __STM32MP13XX_UTIL_CONF_H
#define __STM32MP13XX_UTIL_CONF_H

#include "stm32mp_util_def.h"
#include "stm32mp13xx_hal.h"

#define UTIL_MODEL         "STM32MP13XX_DK"

/* Clock related definitions */
#define UTIL_PLL2_M         3
#define UTIL_PLL2_N         66
#define UTIL_PLL2_P         2
#define UTIL_PLL2_Q         2
#define UTIL_PLL2_R         1
#define UTIL_PLL2_FRACV     0x1400
#define UTIL_PLL2_MODE      RCC_PLL_FRACTIONAL
#define UTIL_PLL2_DIVN_MIN  24
#define UTIL_PLL2_DIVN_MAX  199

/* UART related configuration */
#define UTIL_UART_INSTANCE        UTIL_UART4
#define UTIL_UART_TX_PIN          GPIO_PIN_6
#define UTIL_UART_TX_GPIO_PORT    UTIL_GPIOD
#define UTIL_UART_TX_AF           GPIO_AF8_UART4
#define UTIL_UART_RX_PIN          GPIO_PIN_8
#define UTIL_UART_RX_GPIO_PORT    UTIL_GPIOD
#define UTIL_UART_RX_AF           GPIO_AF8_UART4
#define UTIL_UART_BAUDRATE        115200
#define UTIL_UART_WORDLENGTH      UART_WORDLENGTH_8B
#define UTIL_UART_STOPBITS        UART_STOPBITS_1
#define UTIL_UART_PARITY          UART_PARITY_NONE
#define UTIL_UART_HWFLOWCTL       UART_HWCONTROL_NONE
#define UTIL_UART_MODE            UART_MODE_TX_RX
#define UTIL_UART_OVERSAMPLING    UART_OVERSAMPLING_16

/* PMIC related configuration */
#define UTIL_USE_PMIC                     1
#define UTIL_PMIC_I2C_PORT                UTIL_I2C4
#define UTIL_PMIC_I2C_SCL_PIN             GPIO_PIN_15
#define UTIL_PMIC_I2C_SCL_GPIO_PORT       UTIL_GPIOE
#define UTIL_PMIC_I2C_SCL_AF              GPIO_AF6_I2C4
#define UTIL_PMIC_I2C_SDA_PIN             GPIO_PIN_9
#define UTIL_PMIC_I2C_SDA_GPIO_PORT       UTIL_GPIOB
#define UTIL_PMIC_I2C_SDA_AF              GPIO_AF6_I2C4
#define UTIL_PMIC_BUCK2_MV                1350
#define UTIL_PMIC_LDO3_MV                 500

#endif /* __STM32MP13XX_UTIL_CONF_H */
