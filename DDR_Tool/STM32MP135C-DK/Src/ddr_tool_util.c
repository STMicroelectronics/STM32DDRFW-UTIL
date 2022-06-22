/**
  ******************************************************************************
  * @file    ddr_tool_util.c
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    01-March-2013
  * @brief   This file contains common functions used for STM32MP1xx DDR Tool
  *          project.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2013 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"

#include "stm32mp13xx_disco.h"
#include "stm32mp13xx_disco_stpmic1.h"

#include "ddr_tool_util.h"
#include "main.h"
#include "stm32mp_cubemx_conf.h"
#include "stm32mp_cubemx_ddr_conf.h"

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
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
                      BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = parity none
      - BaudRate    = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  huart.Instance        = USARTx;
  huart.Init.BaudRate   = MX_UART_BAUDRATE;
  huart.Init.WordLength = MX_UART_WORDLENGTH;
  huart.Init.StopBits   = MX_UART_STOPBITS;
  huart.Init.Parity     = MX_UART_PARITY;
  huart.Init.HwFlowCtl  = MX_UART_HWFLOWCTL;
  huart.Init.Mode       = MX_UART_MODE;
  huart.Init.OverSampling = MX_UART_OVERSAMPLING;

  /* Clear UART configuration if need be */
  if(HAL_UART_DeInit(&huart) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if(HAL_UART_Init(&huart) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  if(HAL_UARTEx_EnableFifoMode(&huart) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_UARTEx_SetTxFifoThreshold(&huart, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_UARTEx_SetRxFifoThreshold(&huart, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Output a message on Hyperterminal using printf function */
  printf("\n\r=============== UTILITIES-DDR Tool ===============\r");
  printf("\n\rModel: %s \r", MX_MODEL);
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
  __HAL_UART_CLEAR_OREFLAG(&huart);
  /* e.g. read a character from the EVAL_COM1 and Loop until RXNE = 1 */
  HAL_UART_Receive(&huart, (uint8_t *)&tmp, 1, HAL_TIMEOUT_VALUE);
#else
  tmp = getchar();
#endif
  if (tmp > value)
  {
    printf("\n\r  !!! Please enter valid number between 0 and %lu \n", value);
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
  HAL_UART_Transmit(&huart, (uint8_t *)&value, 1, HAL_TIMEOUT_VALUE);
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
  HAL_UART_Transmit(&huart, (uint8_t *)value, len, HAL_TIMEOUT_VALUE);
#endif
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{

  /* User may add here some code to deal with this error */
  while(1)
  {
    HAL_Delay(5000);
   log_err("\n\r Error Handler \n\r");
   log_dbg("\n\r Why is there an error? \n\r");
  }
}

/**
  * @brief  Check crypto status at BSEC level by reading OTP 1 bit 0.
  *         Enable MCE clock if crypto enabled.
  * @param  None.
  * @retval None.
  */
void BSEC_Check_Crypto(void)
{
  SET_BIT(RCC->MP_APB5ENSETR, RCC_MP_APB5ENSETR_BSECEN);

  HAL_Delay(10);

  BSEC->BSEC_OTP_CONFIG |= BSEC_OTP_CONFIG_PWRUP;

  HAL_Delay(10);

  /* Read OTP1 */
  BSEC->BSEC_OTP_CONTROL = 0x1;
  while((BSEC->BSEC_OTP_STATUS & BSEC_OTP_STATUS_BUSY) != 0)
  {
    ;
  }

  /* Read OTP1[0] (crypto disable) */
  if ((BSEC->BSEC_OTP_DATA[1] & 0x00000001) == 0)
  {
    printf("SoC crypto enabled\n\r");
    SET_BIT(RCC->MP_AHB6ENSETR, RCC_MP_AHB6ENSETR_MCEEN);
  }
  else
  {
    printf("SoC crypto disabled\n\r");
  }

  BSEC->BSEC_OTP_CONFIG &= ~BSEC_OTP_CONFIG_PWRUP;

  HAL_Delay(10);

  CLEAR_BIT(RCC->MP_APB5ENSETR, RCC_MP_APB5ENSETR_BSECEN);
}
