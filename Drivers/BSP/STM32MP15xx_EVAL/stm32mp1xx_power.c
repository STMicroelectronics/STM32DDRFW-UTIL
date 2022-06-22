/**
  ******************************************************************************
  * @file    Templates/Src/stm32mp1xx_power.c
  * @author  MCD Application Team
  * @brief   Main Power Service Routines.
  *          This file provides template for power services.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32mp15xx_eval_stpmic1.h"
#include "stm32mp1xx_power.h"
#include "log.h"
#include "main.h"

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

/**
  * @brief  This function handles System Power configuration.
  * @param  None
  * @retval None
  */
void SystemPower_Config(void)
{
#if (MX_USE_PMIC == 0)
  /* Nothing to do */
#else
  /* PMIC initialization */
  /***********************/
  log_dbg("PMIC initialization \n\r");
  BSP_PMIC_Init();

  STPMU1_Register_Write(MAIN_CONTROL_REG, 0x04);
  STPMU1_Register_Write(VIN_CONTROL_REG, 0xc0);
  STPMU1_Register_Write(USB_CONTROL_REG, 0x30);

  STPMU1_Register_Write(MASK_RESET_BUCK_REG, 0x04);
  STPMU1_Register_Write(MASK_RESET_LDO_REG, 0x00);
  STPMU1_Register_Write(MASK_RANK_BUCK_REG, 0x00);
  STPMU1_Register_Write(MASK_RANK_LDO_REG, 0x00);
  STPMU1_Register_Write(BUCK_PULL_DOWN_REG, 0x00);
  STPMU1_Register_Write(LDO14_PULL_DOWN_REG, 0x00);
  STPMU1_Register_Write(LDO56_PULL_DOWN_REG, 0x00);
  STPMU1_Register_Write(BUCK_ICC_TURNOFF_REG, 0x30);
  STPMU1_Register_Write(LDO_ICC_TURNOFF_REG, 0x3b);

  /* vddcore */
  STPMU1_Regulator_Voltage_Set(STPMU1_BUCK1, 1200);
  STPMU1_Regulator_Enable(STPMU1_BUCK1);

  /* vddddr */
//  STPMU1_Regulator_Voltage_Set(STPMU1_BUCK2, 1350);
//  STPMU1_Regulator_Enable(STPMU1_BUCK2);

  /* vdd */
  STPMU1_Regulator_Voltage_Set(STPMU1_BUCK3, 3300);
  STPMU1_Regulator_Enable(STPMU1_BUCK3);

  /* 3v3 */
  STPMU1_Regulator_Voltage_Set(STPMU1_BUCK4, 3300);
  STPMU1_Regulator_Enable(STPMU1_BUCK4);

  /* vdda */
  STPMU1_Regulator_Voltage_Set(STPMU1_LDO1, 2900);
  STPMU1_Regulator_Enable(STPMU1_LDO1);

  /* 2v8 */
  STPMU1_Regulator_Voltage_Set(STPMU1_LDO2, 2800);
  STPMU1_Regulator_Enable(STPMU1_LDO2);

  /* vtt_ddr  lod3 mode buck2/2 */
//  STPMU1_Regulator_Voltage_Set(STPMU1_LDO3, 0xFFFF);
//  STPMU1_Regulator_Enable(STPMU1_LDO3);

  /* vdd_usb */
  STPMU1_Regulator_Voltage_Set(STPMU1_LDO4, 3300);
  STPMU1_Regulator_Enable(STPMU1_LDO4);

  /* vdd_sd */
  STPMU1_Regulator_Voltage_Set(STPMU1_LDO5, 2900);
  STPMU1_Regulator_Enable(STPMU1_LDO5);

  /* 1v8 */
  STPMU1_Regulator_Voltage_Set(STPMU1_LDO6, 1800);
  STPMU1_Regulator_Enable(STPMU1_LDO6);

//  STPMU1_Regulator_Enable(STPMU1_VREFDDR);
#endif
}

/**
  * @}
  */

/**
  * @}
  */
