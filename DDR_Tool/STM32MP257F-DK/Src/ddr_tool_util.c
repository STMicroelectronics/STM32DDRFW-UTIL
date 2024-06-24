
/**
  ******************************************************************************
  * @file    ddr_tool_util.c
  * @author  MCD Application Team
  * @brief   This file contains common functions used for STM32MP2xx DDR Tool
  *          project.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal.h"

#include "stm32mp257f_disco.h"

#include "ddr_tool_util.h"
#include "main.h"
#include "stm32mp_util_conf.h"
#include "stm32mp_util_ddr_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HAL_TIMEOUT_VALUE   HAL_MAX_DELAY

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration, used for logging */
UART_HandleTypeDef huart;

/* Private function prototypes -----------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is executed to configure in case of error occurrence.
  * @param  None
  * @retval None
  */
void UART_Config(void)
{
  COM_InitTypeDef COM_Init;

  BSP_COM_SelectLogPort(COM1);
  COM_Init.BaudRate   = UTIL_UART_BAUDRATE;
  COM_Init.Parity     = (COM_ParityTypeDef)UTIL_UART_PARITY;
  COM_Init.StopBits   = (COM_StopBitsTypeDef)UTIL_UART_STOPBITS;
  COM_Init.WordLength = UTIL_UART_WORDLENGTH;
  COM_Init.HwFlowCtl  = (COM_HwFlowCtlTypeDef)UTIL_UART_HWFLOWCTL;
  BSP_COM_Init(COM1, &COM_Init);

  if(HAL_UARTEx_EnableFifoMode(&hcom_uart[COM1]) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_UARTEx_SetTxFifoThreshold(&hcom_uart[COM1], UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_UARTEx_SetRxFifoThreshold(&hcom_uart[COM1], UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Output a message on Hyperterminal using printf function */
  printf("\n\r=============== UTILITIES-DDR Tool ===============\r");
  printf("\n\rModel: %s \r", UTIL_MODEL);
  printf("\n\rRAM: %s \n\r", DDR_MEM_NAME);

#if 0
#if defined (__LOG_UART_IO_)
  log_info("__LOG_UART_IO_ switch is activated, log is evacuated over UART \n\r");
#endif

#if defined (__LOG_TRACE_IO_)
  log_info("\n\r __LOG_TRACE_IO_ switch is activated, log is evacuated in a circular buffer \n\r");
#endif

#if LOGLEVEL >= LOGDBG
  log_info("Log level is not at max\n\r");
  log_info("you can configure LOGLEVEL in preprocessor options LOGLEVEL=LOGDBG \n\r");
#endif
#endif
}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
uint32_t Serial_Scanf(uint32_t value)
{
  uint16_t tmp = 0;

#ifndef __TERMINAL_IO__
  __HAL_UART_CLEAR_OREFLAG(&hcom_uart[COM1]);
  /* e.g. read a character from the EVAL_COM1 and Loop until RXNE = 1 */
  HAL_UART_Receive(&hcom_uart[COM1], (uint8_t *)&tmp, 1, HAL_TIMEOUT_VALUE);
#else
  tmp = getchar();
#endif
  if (tmp > value)
  {
    printf("\n\r  !!! Please enter valid number between 0 and %u \n", value);
    return 0xFF;
  }
  return tmp;
}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
void Serial_Putchar(char value)
{
#ifndef __TERMINAL_IO__
  /* e.g. read a character from the EVAL_COM1 and Loop until RXNE = 1 */
  HAL_UART_Transmit(&hcom_uart[COM1], (uint8_t *)&value, 1, HAL_TIMEOUT_VALUE);
#endif
}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
void Serial_Printf(char *value, int len)
{
#ifndef __TERMINAL_IO__
  /* e.g. read a character from the EVAL_COM1 and Loop until RXNE = 1 */
  HAL_UART_Transmit(&hcom_uart[COM1], (uint8_t *)value, len, HAL_TIMEOUT_VALUE);
#endif
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{

  /* Turn LED3 on */
  BSP_LED_Off(LED3);

  /* User may add here some code to deal with this error */
  while(1)
  {
    HAL_Delay(5000);
    printf("\n\r Error Handler \n\r");
    log_dbg("\n\r Why is there an error? \n\r");
  }
}

void valid_delay_us(unsigned long delay_us)
{
  __IO uint32_t wait_loop_index = 0U;

  wait_loop_index = (delay_us * (SystemCoreClock / (1000000UL * 2UL)));

  while(wait_loop_index != 0UL)
  {
    wait_loop_index--;
  }
}
