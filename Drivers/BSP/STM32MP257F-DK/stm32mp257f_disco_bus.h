/**
  ******************************************************************************
  * @file    stm32mp257f_disco_bus.h
  * @author  MCD Application Team
  * @brief   This file is the header of stm32mp257f_disco_bus.c file
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
#ifndef STM32MP257F_DK_BUS_H
#define STM32MP257F_DK_BUS_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "stm32mp257f_disco_conf.h"
#include "stm32mp_util_conf.h"

#if defined(BSP_USE_CMSIS_OS)
#include "cmsis_os.h"
#endif /* BSP_USE_CMSIS_OS */
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP257F_DK
  * @{
  */

/** @addtogroup STM32MP257F_DK_BUS
  * @{
  */
#if defined (CORE_CA35) || defined (CORE_CM33)
/** @defgroup STM32MP257F_DK_BUS_Exported_Types BUS Exported Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS > 0)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS > 0) */

/**
  * @}
  */
/** @defgroup STM32MP257F_DK_BUS_Exported_Constants BUS Exported Constants
  * @{
  */
#if (UTIL_USE_PMIC)
  /* Definition for I2C clock resources */
  #if (UTIL_PMIC_I2C_PORT == UTIL_I2C1)
    #define BUS_I2C                             I2C1
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C1_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C2)
    #define BUS_I2C                             I2C2
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C2_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C2_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C3)
    #define BUS_I2C                             I2C3
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C3_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C3_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C3_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C3_RELEASE_RESET()
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C4)
    #define BUS_I2C                             I2C4
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C4_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C4_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C4_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C4_RELEASE_RESET()
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C5)
    #define BUS_I2C                             I2C5
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C5_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C5_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C5_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C5_RELEASE_RESET()
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C6)
    #define BUS_I2C                             I2C6
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C6_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C6_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C6_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C6_RELEASE_RESET()
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C7)
    #define BUS_I2C                             I2C7
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C7_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C7_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C7_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C7_RELEASE_RESET()
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C8)
    #define BUS_I2C                             I2C8
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C8_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C8_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C8_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C8_RELEASE_RESET()
  #else
    #error "unknown UTIL_PMIC_I2C_PORT definition."
  #endif

  #define BUS_I2C_SCL_PIN                       UTIL_PMIC_I2C_SCL_PIN

  #if (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOA)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOA
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOB)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOB
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOB_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOC)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOC
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOC_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOD)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOD
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOD_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOE)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOE
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOE_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOF)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOF
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOF_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOG)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOG
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOG_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOH)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOH
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOH_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOI)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOI
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOI_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOJ)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOJ
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOJ_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOJ_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOK)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOK
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOK_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOK_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SCL_GPIO_PORT == UTIL_GPIOZ)
    #define BUS_I2C_SCL_GPIO_PORT               GPIOZ
    #define BUS_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOZ_CLK_ENABLE()
    #define BUS_I2C_SCL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOZ_CLK_DISABLE()
  #else
    #error "unknown UTIL_PMIC_I2C_SCL_GPIO_PORT definition."
  #endif

  #define BUS_I2C_SCL_AF                        UTIL_PMIC_I2C_SCL_AF

  #define BUS_I2C_SDA_PIN                       UTIL_PMIC_I2C_SDA_PIN

  #if (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOA)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOA
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOB)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOB
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOB_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOC)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOC
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOC_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOD)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOD
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOD_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOE)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOE
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOE_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOF)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOF
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOF_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOG)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOG
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOG_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOH)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOH
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOH_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOI)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOI
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOI_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOJ)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOJ
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOJ_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOJ_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOK)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOK
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOK_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOK_CLK_DISABLE()
  #elif (UTIL_PMIC_I2C_SDA_GPIO_PORT == UTIL_GPIOZ)
    #define BUS_I2C_SDA_GPIO_PORT               GPIOZ
    #define BUS_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOZ_CLK_ENABLE()
    #define BUS_I2C_SDA_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOZ_CLK_DISABLE()
  #else
    #error "unknown UTIL_PMIC_I2C_SDA_GPIO_PORT definition."
  #endif

  #define BUS_I2C_SDA_AF                      UTIL_PMIC_I2C_SDA_AF
#else /* UTIL_USE_PMIC */
  /* Definition for I2C clock resources */
  #define BUS_I2C                               I2C7

  #define BUS_I2C_CLK_ENABLE()                  __HAL_RCC_I2C7_CLK_ENABLE()
  #define BUS_I2C_CLK_DISABLE()                 __HAL_RCC_I2C7_CLK_DISABLE()

  #define BUS_I2C_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
  #define BUS_I2C_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOD_CLK_DISABLE()

  #define BUS_I2C_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
  #define BUS_I2C_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOD_CLK_DISABLE()

  #define BUS_I2C_FORCE_RESET()                 __HAL_RCC_I2C7_FORCE_RESET()
  #define BUS_I2C_RELEASE_RESET()               __HAL_RCC_I2C7_RELEASE_RESET()

  /* Definition for I2C Pins */
  #define BUS_I2C_SCL_PIN                       GPIO_PIN_15
  #define BUS_I2C_SCL_GPIO_PORT                 GPIOD
  #define BUS_I2C_SCL_AF                        GPIO_AF10_I2C7

  #define BUS_I2C_SDA_PIN                       GPIO_PIN_14
  #define BUS_I2C_SDA_GPIO_PORT                 GPIOD
  #define BUS_I2C_SDA_AF                        GPIO_AF10_I2C7
#endif /* UTIL_USE_PMIC */

#ifndef BUS_I2C_FREQUENCY
  #define BUS_I2C_FREQUENCY  400000U /* Frequency of I2C = 400 KHz*/
#endif /* BUS_I2C_FREQUENCY */
/**
  * @}
  */

/** @addtogroup STM32MP257F_DK_BUS_Exported_Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c;
/**
  * @}
  */

/** @addtogroup STM32MP257F_DK_BUS_Exported_Functions
  * @{
  */
int32_t BSP_GetTick(void);

/* Function prototypes for I2C */
int32_t BSP_I2C_Init(void);
int32_t BSP_I2C_DeInit(void);
int32_t BSP_I2C_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_IsReady(uint16_t DevAddr, uint32_t Trials);

#if (USE_HAL_I2C_REGISTER_CALLBACKS > 0)
int32_t BSP_I2C_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
__weak HAL_StatusTypeDef MX_I2C_Init(I2C_HandleTypeDef *hI2c, uint32_t timing);
#endif /* CORE_CA35 || CORE_CM33 */
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

#endif /* STM32MP257F_DK_BUS_H */
