/**
  ******************************************************************************
  * @file    stm32mp257f_eval_stpmic.h
  * @author  MCD Application Team
  * @brief   stpmu driver functions used for ST internal validation
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
#ifndef STM32MP257F_EVAL_STPMIC_H
#define STM32MP257F_EVAL_STPMIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal.h"
#include "stpmic2.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP257F_EV1
  * @{
  */

/** @addtogroup STPMIC
  * @{
  */

/** @defgroup STPMIC_Exported_Types Exported Types
  * @{
  */
typedef enum
{
  VDDCPU = 0,
  VDDCORE,
  VDDGPU,
  VDDIO_PMIC,
  VDDA1V8_AON,
  VDD_EMMC,
  V1V8,
  VDD3V3_USB,
  VDD_DDR,
  VREF_DDR,
  VTT_DDR,
  VPP_DDR,
  VDD_SDCARD,
  VDDIO_SDCARD,
  V3V3
} board_regul_t;

typedef struct
{
  char                 boardRegulName[15]; /* board regulator name */
  board_regul_t        boardRegulId;  /* board regulator Id */
  PMIC_RegulId_TypeDef pmicRegul;  /* pmic regulator */
  uint8_t              control_reg1;
  uint8_t              control_reg2;
  uint8_t              alt_control_reg1;
  uint8_t              alt_control_reg2;
  uint8_t              pwr_control_reg;
  uint32_t             regulMin; /* regulator-min-mvolt */
  uint32_t             regulMax; /* regulator-max-mvolt */
} board_regul_struct_t;

/* Exported types ------------------------------------------------------------*/
/**
  * @}
  */

/** @defgroup STPMIC_Exported_Constants Exported Constants
  * @{
  */
/* Board Configuration ------------------------------------------------------------*/
/* Definition of PMIC <=> stm32mp2 Signals */
#if (UTIL_PMIC_INT_PIN == UTIL_PIN0)
#define PMIC_INTN_PIN                       GPIO_PIN_0
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(0)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN1)
#define PMIC_INTN_PIN                       GPIO_PIN_1
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(1)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN2)
#define PMIC_INTN_PIN                       GPIO_PIN_2
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(2)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN3)
#define PMIC_INTN_PIN                       GPIO_PIN_3
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(3)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN4)
#define PMIC_INTN_PIN                       GPIO_PIN_4
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(4)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN5)
#define PMIC_INTN_PIN                       GPIO_PIN_5
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(5)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN6)
#define PMIC_INTN_PIN                       GPIO_PIN_6
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(6)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN7)
#define PMIC_INTN_PIN                       GPIO_PIN_7
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(7)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN8)
#define PMIC_INTN_PIN                       GPIO_PIN_8
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(8)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN9)
#define PMIC_INTN_PIN                       GPIO_PIN_9
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(9)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN10)
#define PMIC_INTN_PIN                       GPIO_PIN_10
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(10)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN11)
#define PMIC_INTN_PIN                       GPIO_PIN_11
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(11)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN12)
#define PMIC_INTN_PIN                       GPIO_PIN_12
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(12)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN13)
#define PMIC_INTN_PIN                       GPIO_PIN_13
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(13)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN14)
#define PMIC_INTN_PIN                       GPIO_PIN_14
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(14)
#elif (UTIL_PMIC_INT_PIN == UTIL_PIN15)
#define PMIC_INTN_PIN                       GPIO_PIN_15
#define PMIC_INTN_PIN_RIF_RES               RESMGR_GPIO_PIN(15)
#else
  #error "unknown UTIL_PMIC_INT_PIN definition."
#endif

#if (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOA)
  #define PMIC_INTN_PORT                    GPIOA
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOA
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(90)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOB)
  #define PMIC_INTN_PORT                    GPIOB
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOB
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(91)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOC)
  #define PMIC_INTN_PORT                    GPIOC
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOC
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(92)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOD)
  #define PMIC_INTN_PORT                    GPIOD
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOD_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOD
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(93)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOE)
  #define PMIC_INTN_PORT                    GPIOE
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOE_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOE
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(94)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOF)
  #define PMIC_INTN_PORT                    GPIOF
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOF_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOF_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOF
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(95)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOG)
  #define PMIC_INTN_PORT                    GPIOG
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOG_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOG_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOG
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(96)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOH)
  #define PMIC_INTN_PORT                    GPIOH
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOH_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOH_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOH
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(97)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOI)
  #define PMIC_INTN_PORT                    GPIOI
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOI_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOI_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOI
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(98)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOJ)
  #define PMIC_INTN_PORT                    GPIOI
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOJ_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOJ_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOJ
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(99)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOK)
  #define PMIC_INTN_PORT                    GPIOK
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOK_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOK_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOK
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(100)
#elif (UTIL_PMIC_INT_GPIO_PORT == UTIL_GPIOZ)
  #define PMIC_INTN_PORT                    GPIOZ
  #define PMIC_INTN_CLK_ENABLE()            __HAL_RCC_GPIOZ_CLK_ENABLE()
  #define PMIC_INTN_CLK_DISABLE()           __HAL_RCC_GPIOZ_CLK_DISABLE()
  #define PMIC_INTN_RIF_RES                 RESMGR_RESOURCE_RIF_GPIOZ
  #define PMIC_INTN_RCC_RES                 RESMGR_RCC_RESOURCE(101)
#else
  #error "unknown UTIL_PMIC_INT_GPIO_PORT definition."
#endif

#ifdef USE_WAKEUP_PIN
#define PMIC_INTN_EXTI_IRQ          CPU1_WAKEUP_PIN_IRQn   /* CPU1_WAKEUP_PIN_IRQn */
#define BSP_PMIC_INTN_IRQHANDLER    CPU1_WAKEUP_PIN_IRQHandler
#else
#define PMIC_INTN_EXTI_IRQ          EXTI2_0_IRQn   /* CPU1_WAKEUP_PIN_IRQn */
#define BSP_PMIC_INTN_IRQHANDLER    EXTI2_0_IRQHandler
#endif /* USE_WAKEUP_PIN */

#define BSP_PMIC_PWRCTRL_PIN_ASSERT()   HAL_GPIO_WritePin(PMIC_PWRCTRL_PORT, \
                                                          PMIC_PWRCTRL_PIN, GPIO_PIN_RESET);
#define BSP_PMIC_PWRCTRL_PIN_PULL()     HAL_GPIO_WritePin(PMIC_PWRCTRL_PORT, \
                                                          PMIC_PWRCTRL_PIN, GPIO_PIN_SET);

/**
  * @brief  STPMIC2 product ID
  */
#define  STPMIC2_ID           0x20U /* Default PMIC product ID: 0x2X (X depends on PMIC variant) */


/**
  * @brief  STPMIC2 LOW POWER MODES
  */
#define  STPMIC2_RUN1_STOP1           0x0U /*  */
#define  STPMIC2_RUN2_STOP2           0x1U /*  */
#define  STPMIC2_LP_STOP1             0x2U /*  */
#define  STPMIC2_LP_STOP2             0x3U /*  */
#define  STPMIC2_LPLV_STOP1           0x4U /*  */
#define  STPMIC2_LPLV_STOP2           0x5U /*  */
#define  STPMIC2_STANDBY_DDR_SR       0x6U /*  */
#define  STPMIC2_STANDBY_DDR_OFF      0x7U /*  */


/**
  * @}
  */

/** @defgroup STPMIC_Private_Defines Private Defines
  * @{
  */
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/**
  * @}
  */

/** @defgroup STPMIC_Private_Variables Private Variables
  * @{
  */

/* Exported constants --------------------------------------------------------*/

/**
  * @}
  */

/** @defgroup STPMIC_Exported_Functions Exported Functions
  * @{
  */

/* Exported functions --------------------------------------------------------*/
uint32_t BSP_PMIC_Init(void);
uint32_t BSP_PMIC_DeInit(void);
uint32_t BSP_PMIC_Is_Device_Ready(void);
uint32_t BSP_PMIC_DDR_Power_Init();
#if defined (STPMIC2_DEBUG)
uint32_t BSP_PMIC_DumpRegs(void);
#endif /* defined (STPMIC2_DEBUG) */
uint32_t BSP_PMIC_Power_Mode_Init(void);
uint32_t BSP_PMIC_DDR_Power_Off(void);
uint32_t BSP_PMIC_REGU_Set_Off(board_regul_t regu);
uint32_t BSP_PMIC_REGU_Set_On(board_regul_t regu);
uint32_t BSP_PMIC_Set_Power_Mode(uint32_t mode);
void BSP_PMIC_INTn_Callback(PMIC_IRQn IRQn);
/**
  * @}
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
#ifdef __cplusplus
}
#endif

#endif /* STM32MP257F_EVAL_STPMIC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
