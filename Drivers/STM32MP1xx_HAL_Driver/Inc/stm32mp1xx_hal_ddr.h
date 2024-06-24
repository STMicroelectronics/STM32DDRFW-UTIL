/**
  ******************************************************************************
  * @file    stm32mp1xx_hal_ddr.h
  * @author  MCD Application Team
  * @brief   Header file of DDR HAL module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32MP1xx_HAL_DDR_H
#define STM32MP1xx_HAL_DDR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "stm32mp1xx_hal_def.h"

#if defined (DDRCTRL) && defined (DDRPHYC)

/** @addtogroup STM32MP1xx_HAL_Driver
  * @{
  */

/** @addtogroup DDR
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup DDR_Exported_Types DDR Exported Types
  * @{
  */

/**
  * @brief  HAL DDR type definition
  */
typedef enum
{
  STM32MP_DDR3,
  STM32MP_LPDDR2_16,
  STM32MP_LPDDR2_32,
  STM32MP_LPDDR3_16,
  STM32MP_LPDDR3_32,
} ddr_type;

/**
  * @brief  HAL DDR Self Refresh modes definition
  */
typedef enum
{
  HAL_DDR_SW_SELF_REFRESH_MODE = 0x0U,   /*!< DDR Software Self Refresh Mode */
  HAL_DDR_AUTO_SELF_REFRESH_MODE = 0x1U, /*!< DDR Automatic Self Refresh Mode */
  HAL_DDR_HW_SELF_REFRESH_MODE = 0x2U,   /*!< DDR Hardware Self Refresh Mode */
  HAL_DDR_INVALID_MODE = 0x3U,           /*!< DDR Invalid Self Refresh Mode */
} HAL_DDR_SelfRefreshModeTypeDef;

/**
  * @brief  HAL DDR settings definition
  */
typedef struct {
  uint32_t MSTR;
  uint32_t MRCTRL0;
  uint32_t MRCTRL1;
  uint32_t DERATEEN;
  uint32_t DERATEINT;
  uint32_t PWRCTL;
  uint32_t PWRTMG;
  uint32_t HWLPCTL;
  uint32_t RFSHCTL0;
  uint32_t RFSHCTL3;
  uint32_t CRCPARCTL0;
  uint32_t ZQCTL0;
  uint32_t DFITMG0;
  uint32_t DFITMG1;
  uint32_t DFILPCFG0;
  uint32_t DFIUPD0;
  uint32_t DFIUPD1;
  uint32_t DFIUPD2;
  uint32_t DFIPHYMSTR;
  uint32_t ODTMAP;
  uint32_t DBG0;
  uint32_t DBG1;
  uint32_t DBGCMD;
  uint32_t POISONCFG;
  uint32_t PCCFG;
}HAL_DDR_RegTypeDef;

typedef struct {
  uint32_t RFSHTMG;
  uint32_t DRAMTMG0;
  uint32_t DRAMTMG1;
  uint32_t DRAMTMG2;
  uint32_t DRAMTMG3;
  uint32_t DRAMTMG4;
  uint32_t DRAMTMG5;
  uint32_t DRAMTMG6;
  uint32_t DRAMTMG7;
  uint32_t DRAMTMG8;
  uint32_t DRAMTMG14;
  uint32_t ODTCFG;
}HAL_DDR_TimingTypeDef;

typedef struct {
  uint32_t ADDRMAP1;
  uint32_t ADDRMAP2;
  uint32_t ADDRMAP3;
  uint32_t ADDRMAP4;
  uint32_t ADDRMAP5;
  uint32_t ADDRMAP6;
  uint32_t ADDRMAP9;
  uint32_t ADDRMAP10;
  uint32_t ADDRMAP11;
}HAL_DDR_MapTypeDef;

typedef struct {
  uint32_t SCHED;
  uint32_t SCHED1;
  uint32_t PERFHPR1;
  uint32_t PERFLPR1;
  uint32_t PERFWR1;
  uint32_t PCFGR_0;
  uint32_t PCFGW_0;
  uint32_t PCFGQOS0_0;
  uint32_t PCFGQOS1_0;
  uint32_t PCFGWQOS0_0;
  uint32_t PCFGWQOS1_0;
#ifdef DDR_DUAL_AXI_PORT
  uint32_t PCFGR_1;
  uint32_t PCFGW_1;
  uint32_t PCFGQOS0_1;
  uint32_t PCFGQOS1_1;
  uint32_t PCFGWQOS0_1;
  uint32_t PCFGWQOS1_1;
#endif
}HAL_DDR_PerfTypeDef;

typedef struct {
  uint32_t PGCR;
  uint32_t ACIOCR;
  uint32_t DXCCR;
  uint32_t DSGCR;
  uint32_t DCR;
  uint32_t ODTCR;
  uint32_t ZQ0CR1;
  uint32_t DX0GCR;
  uint32_t DX1GCR;
#ifdef DDR_32BIT_INTERFACE
  uint32_t DX2GCR;
  uint32_t DX3GCR;
#endif
}HAL_DDR_PhyRegTypeDef;

typedef struct {
  uint32_t PTR0;
  uint32_t PTR1;
  uint32_t PTR2;
  uint32_t DTPR0;
  uint32_t DTPR1;
  uint32_t DTPR2;
  uint32_t MR0;
  uint32_t MR1;
  uint32_t MR2;
  uint32_t MR3;
}HAL_DDR_PhyTimingTypeDef;

typedef struct {
  const char *name;
  uint32_t speed; /* in kHZ */
  uint32_t size; /* Memory size in byte = col * row * width */
}HAL_DDR_InfoTypeDef;

typedef struct  {
  HAL_DDR_InfoTypeDef      info;
  HAL_DDR_RegTypeDef       c_reg;
  HAL_DDR_TimingTypeDef    c_timing;
  HAL_DDR_MapTypeDef       c_map;
  HAL_DDR_PerfTypeDef      c_perf;
  HAL_DDR_PhyRegTypeDef    p_reg;
  HAL_DDR_PhyTimingTypeDef p_timing;
}HAL_DDR_ConfigTypeDef;

/**
  * @brief  HAL DDR interactive step definition
  */
typedef enum  {
	STEP_DDR_RESET,
	STEP_CTL_INIT,
	STEP_PHY_INIT,
	STEP_DDR_READY,
	STEP_RUN,
}HAL_DDR_InteractStepTypeDef;

/**
  * @brief  DDR Initialization Structure definition
  */
typedef struct
{
  bool wakeup_from_standby; /*!< [input]
                                 Specifies if the system is waking up from the
                                 standby mode. The application is in charge to
                                 get this information. It will impact the self
                                 refresh state and the related steps within
                                 the initialization sequence. */

  bool self_refresh;        /*!< [output]
                                 Specifies the DDR self refresh status.
                                 Self Refresh enabled if true. */

  uint32_t zdata;           /*!< [input]
                                 Specifies the ZQ calibration restored value
                                 if any. Only used by the initialization
                                 sequence if wakeup_from_standby is true. The
                                 application is in charge of saving/restoring
                                 it during low power transitions. Value is
                                 obtaines when calling HAL_DDR_SR_Entry. */

  bool clear_bkp;           /*!< [output]
                                 Specifies if backup should be cleared after
                                 DDR initialization (DDR lost content case).
                                 Clear requested if true. */
} DDR_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup DDR_Exported_Constants DDR Exported Constants
  * @{
  */

/**
  * @brief  AXI Sub-System clock source
  */
#define HAL_DDR_CLK_AXI_HSI   0
#define HAL_DDR_CLK_AXI_HSE   1
#define HAL_DDR_CLK_AXI_PLL2  2

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup DDR_Exported_Macros DDR Exported Macros
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup DDR_Exported_Functions
  * @{
  */

HAL_StatusTypeDef HAL_DDR_Init(DDR_InitTypeDef *iddr);
HAL_StatusTypeDef HAL_DDR_SR_Entry(uint32_t *zq0cr0_zdata);
HAL_StatusTypeDef HAL_DDR_SR_Exit(void);
HAL_StatusTypeDef HAL_DDR_SR_SetMode(HAL_DDR_SelfRefreshModeTypeDef mode);
HAL_DDR_SelfRefreshModeTypeDef HAL_DDR_SR_ReadMode(void);

#ifdef DDR_INTERACTIVE
HAL_StatusTypeDef HAL_DDR_ASS_Set_Clksrc(uint32_t clksrc);
void HAL_DDR_Convert_Case(const char *in_str, char *out_str, bool ToUpper);
bool HAL_DDR_Interactive(HAL_DDR_InteractStepTypeDef step);
HAL_StatusTypeDef HAL_DDR_Dump_Param(HAL_DDR_ConfigTypeDef *config,
                                     const char *name);
HAL_StatusTypeDef HAL_DDR_Dump_Reg(const char *name, bool save);
void HAL_DDR_Edit_Param(HAL_DDR_ConfigTypeDef *config, char *name,
                        char *string);
void HAL_DDR_Edit_Reg(char *name, char *string);
#endif

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/** @defgroup DDR_Private_Types DDR Private Types
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup DDR_Private_Variables DDR Private Variables
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup DDR_Private_Constants DDR Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup DDR_Private_Macros DDR Private Macros
  * @{
  */

/**
  * @}
  */
/* Private functions ---------------------------------------------------------*/
/** @defgroup DDR_Private_Functions DDR Private Functions
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* DDRCTRL & DDRPHYC */

#ifdef __cplusplus
}
#endif

#endif /* STM32MP1xxHAL_DDR_H */
