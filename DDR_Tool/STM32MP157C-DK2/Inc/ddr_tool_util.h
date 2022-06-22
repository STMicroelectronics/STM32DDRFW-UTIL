/**
  ******************************************************************************
  * @file    ddr_tool_util.h
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    01-March-2013
  * @brief   Header for ddr_tool_util.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DDR_TOOL_UTIL_H
#define __DDR_TOOL_UTIL_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UART_Config(void);
uint32_t Serial_Scanf(uint32_t value);
void Serial_Putchar(char value);
void Serial_Printf(char *value, int len);
void Error_Handler(void);

#endif /* __DDR_TOOL_UTIL_H */
