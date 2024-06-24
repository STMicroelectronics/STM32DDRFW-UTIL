/**
  ******************************************************************************
  * @file    stm32mp_util_conf.h
  * @author  MCD Application Team
  * @brief   Header file of board configuration for STM32MP2xx.
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
#ifndef __STM32MP2XX_UTIL_CONF_H
#define __STM32MP2XX_UTIL_CONF_H

#include "stm32mp2xx_hal.h"
#include "stm32mp_util_def.h"
#include "ddr_tool_util.h"

#define UTIL_MODEL                "STM32MP2XX"

/* UART related configuration */
#define UTIL_UART_INSTANCE      UTIL_USART2
#define UTIL_UART_TX_PIN        GPIO_PIN_4
#define UTIL_UART_TX_GPIO_PORT  UTIL_GPIOA
#define UTIL_UART_TX_AF         GPIO_AF6_USART2
#define UTIL_UART_RX_PIN        GPIO_PIN_8
#define UTIL_UART_RX_GPIO_PORT  UTIL_GPIOA
#define UTIL_UART_RX_AF         GPIO_AF8_USART2
#define UTIL_UART_BAUDRATE      115200
#define UTIL_UART_WORDLENGTH    UART_WORDLENGTH_8B
#define UTIL_UART_STOPBITS      UART_STOPBITS_1
#define UTIL_UART_PARITY        UART_PARITY_NONE
#define UTIL_UART_HWFLOWCTL     UART_HWCONTROL_NONE

/* PMIC related configuration */
#define UTIL_USE_PMIC                     1
#define UTIL_PMIC_I2C_PORT                UTIL_I2C7
#define UTIL_PMIC_I2C_SCL_PIN             GPIO_PIN_15
#define UTIL_PMIC_I2C_SCL_GPIO_PORT       UTIL_GPIOD
#define UTIL_PMIC_I2C_SCL_AF              GPIO_AF10_I2C7
#define UTIL_PMIC_I2C_SDA_PIN             GPIO_PIN_14
#define UTIL_PMIC_I2C_SDA_GPIO_PORT       UTIL_GPIOD
#define UTIL_PMIC_I2C_SDA_AF              GPIO_AF10_I2C7
#endif /* __STM32MP2XX_UTIL_CONF_H */
