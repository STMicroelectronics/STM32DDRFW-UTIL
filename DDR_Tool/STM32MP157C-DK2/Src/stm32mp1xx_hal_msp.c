/**
  ******************************************************************************
  * @file    Templates/Src/stm32mp1xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32mp15xx_disco.h"
#include "stm32mp15xx_disco_stpmic1.h"

/** @addtogroup STM32MP1xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspDeInit(void)
{
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of UART4 clocks */
#if (UTIL_UART_INSTANCE == UTIL_UART4)
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART24;
  RCC_PeriphClkInit.Uart24ClockSelection = RCC_UART24CLKSOURCE_HSI;
#elif (UTIL_UART_INSTANCE == UTIL_UART5)
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART35;
  RCC_PeriphClkInit.Uart35ClockSelection = RCC_UART35CLKSOURCE_HSI;
#elif (UTIL_UART_INSTANCE == UTIL_UART7)
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART78;
  RCC_PeriphClkInit.Uart78ClockSelection = RCC_UART78CLKSOURCE_HSI;
#elif (UTIL_UART_INSTANCE == UTIL_UART8)
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART78;
  RCC_PeriphClkInit.Uart78ClockSelection = RCC_UART78CLKSOURCE_HSI;
#endif
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

}

#define STPMIC1_LDO12356_OUTPUT_MASK       (uint8_t)0x7C
#define STPMIC1_LDO12356_OUTPUT_SHIFT      2
#define STPMIC1_LDO3_MODE                  (uint8_t)BIT(7)
#define STPMIC1_LDO3_DDR_SEL               31U

#define STPMIC1_BUCK_OUTPUT_SHIFT          2
#define STPMIC1_BUCK3_1V8                  (39U << STPMIC1_BUCK_OUTPUT_SHIFT)

#define STPMIC1_DEFAULT_START_UP_DELAY_MS  1

/**
  * @brief  This function handles System Power configuration.
  * @param  DDR type
  * @retval
  *  0: Test passed
  *  Value different from 0: Test failed
  */
int HAL_DDR_MspInit(ddr_type type)
{
#if (UTIL_USE_PMIC)
  bool buck3_at_1v8 = false;
  uint8_t read_val;
  uint16_t buck2_mv = 1350;
  uint16_t ldo3_mv = 500;

  switch (type) {
  case STM32MP_DDR3:
    /* Set LDO3 to sync mode */
    read_val = STPMU1_Register_Read(LDO3_CONTROL_REG);

    read_val &= ~STPMIC1_LDO3_MODE;
    read_val &= ~STPMIC1_LDO12356_OUTPUT_MASK;
    read_val |= STPMIC1_LDO3_DDR_SEL << STPMIC1_LDO12356_OUTPUT_SHIFT;

    STPMU1_Register_Write(LDO3_CONTROL_REG, read_val);

    STPMU1_Regulator_Voltage_Set(STPMU1_BUCK2, buck2_mv);

    STPMU1_Regulator_Enable(STPMU1_BUCK2);

    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);

    STPMU1_Regulator_Enable(STPMU1_VREFDDR);

    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);

    STPMU1_Regulator_Enable(STPMU1_LDO3);

    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
    break;

  case STM32MP_LPDDR2_16:
  case STM32MP_LPDDR2_32:
  case STM32MP_LPDDR3_16:
  case STM32MP_LPDDR3_32:
    /*
     * Set LDO3 to 1.8V
     * Set LDO3 to bypass mode if BUCK3 = 1.8V
     * Set LDO3 to normal mode if BUCK3 != 1.8V
     */
    read_val = STPMU1_Register_Read(BUCK3_CONTROL_REG);

    if ((read_val & STPMIC1_BUCK3_1V8) == STPMIC1_BUCK3_1V8) {
      buck3_at_1v8 = true;
    }

    read_val = STPMU1_Register_Read(LDO3_CONTROL_REG);

    read_val &= ~STPMIC1_LDO3_MODE;
    read_val &= ~STPMIC1_LDO12356_OUTPUT_MASK;
    if (buck3_at_1v8)
    {
      read_val |= STPMIC1_LDO3_MODE;
    }

    STPMU1_Register_Write(LDO3_CONTROL_REG, read_val);

    STPMU1_Regulator_Voltage_Set(STPMU1_LDO3, ldo3_mv);

    /* Set BUCK2 to 1.2V (16 bits) or 1.25V (32 bits) */
    switch (type)
    {
    case STM32MP_LPDDR2_32:
    case STM32MP_LPDDR3_32:
      buck2_mv = 1250;
      break;
    default:
    case STM32MP_LPDDR2_16:
    case STM32MP_LPDDR3_16:
      buck2_mv = 1200;
      break;
    }

    STPMU1_Regulator_Voltage_Set(STPMU1_BUCK2, buck2_mv);

    STPMU1_Regulator_Enable(STPMU1_LDO3);

    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);

    STPMU1_Regulator_Enable(STPMU1_BUCK2);

    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);

    STPMU1_Regulator_Enable(STPMU1_VREFDDR);

    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
    break;

  default:
    break;
  };
#endif /* UTIL_USE_PMIC */

  return 0;
}

/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspInit(void)
{*/
/*}*/

/**
  * @brief  DeInitializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspDeInit(void)
{*/
/*}*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
