/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_bus.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for hardware resources.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32MP13XX_DISCO_BUS_H
#define STM32MP13XX_DISCO_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif


 /* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_disco_conf.h"
#include "stm32mp13xx_disco_errno.h"
#include "stm32mp_util_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO_BUS
  * @{
  */

/** @defgroup STM32MP13XX_DISCO_BUS_Exported_Constants BUS Exported Constants
  * @{
  */


/* Exported constant IO ------------------------------------------------------*/

 /**
   * @brief AUDIO Slave I2C address
   */
 #define AUDIO_I2C_ADDRESS                ((uint16_t)0x36U)

/**
  * @brief STPMU1 I2C address
  * (NVM_SECTOR3_REGISTER_7 & 0x7F) << 1
  */
#define STPMU1_I2C_ADDRESS               ((0x33U & 0x7FU) << 1 )

/* I2C clock speed configuration (in Hz)
   WARNING:
   Make sure that this define is not already declared in other files (ie.
   stm32mp13xx_disco.h file). It can be used in parallel by other modules. */
#ifndef I2C_SPEED
 #define I2C_SPEED                        ((uint32_t)100000U)
#endif /* I2C_SPEED */

#if (UTIL_USE_PMIC)
  /* Definition for I2C clock resources */
  #if (UTIL_PMIC_I2C_PORT == UTIL_I2C1)
    #define BUS_I2C_INSTANCE                    I2C1
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C1_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()
    #define BUS_I2C_EV_IRQn                     I2C1_EV_IRQn
    #define BUS_I2C_ER_IRQn                     I2C1_ER_IRQn
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C2)
    #define BUS_I2C_INSTANCE                    I2C2
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C2_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C2_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()
    #define BUS_I2C_EV_IRQn                     I2C2_EV_IRQn
    #define BUS_I2C_ER_IRQn                     I2C2_ER_IRQn
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C3)
    #define BUS_I2C_INSTANCE                    I2C3
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C3_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C3_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C3_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C3_RELEASE_RESET()
    #define BUS_I2C_EV_IRQn                     I2C3_EV_IRQn
    #define BUS_I2C_ER_IRQn                     I2C3_ER_IRQn
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C4)
    #define BUS_I2C_INSTANCE                    I2C4
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C4_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C4_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C4_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C4_RELEASE_RESET()
    #define BUS_I2C_EV_IRQn                     I2C4_EV_IRQn
    #define BUS_I2C_ER_IRQn                     I2C4_ER_IRQn
  #elif (UTIL_PMIC_I2C_PORT == UTIL_I2C5)
    #define BUS_I2C_INSTANCE                    I2C5
    #define BUS_I2C_CLK_ENABLE()                __HAL_RCC_I2C5_CLK_ENABLE()
    #define BUS_I2C_CLK_DISABLE()               __HAL_RCC_I2C5_CLK_DISABLE()
    #define BUS_I2C_FORCE_RESET()               __HAL_RCC_I2C5_FORCE_RESET()
    #define BUS_I2C_RELEASE_RESET()             __HAL_RCC_I2C5_RELEASE_RESET()
    #define BUS_I2C_EV_IRQn                     I2C5_EV_IRQn
    #define BUS_I2C_ER_IRQn                     I2C5_ER_IRQn
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
  #else
    #error "unknown UTIL_PMIC_I2C_SDA_GPIO_PORT definition."
  #endif

  #define BUS_I2C_SDA_AF                      UTIL_PMIC_I2C_SDA_AF
#else /* UTIL_USE_PMIC */
  /* Definition for I2C clock resources */
  #define BUS_I2C_INSTANCE                       I2C4

  #define BUS_I2C_CLK_ENABLE()                   __HAL_RCC_I2C4_CLK_ENABLE()
  #define BUS_I2C_CLK_DISABLE()                  __HAL_RCC_I2C4_CLK_DISABLE()

  #define BUS_I2C_SCL_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
  #define BUS_I2C_SCL_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

  #define BUS_I2C_SDA_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()
  #define BUS_I2C_SDA_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOB_CLK_DISABLE()

  #define BUS_I2C_FORCE_RESET()                  __HAL_RCC_I2C4_FORCE_RESET()
  #define BUS_I2C_RELEASE_RESET()                __HAL_RCC_I2C4_RELEASE_RESET()

  /* Definition for I2C5 Pins */
  #define BUS_I2C_SCL_PIN                        GPIO_PIN_15
  #define BUS_I2C_SCL_GPIO_PORT                  GPIOE
  #define BUS_I2C_SCL_AF                         GPIO_AF6_I2C4

  #define BUS_I2C_SDA_PIN                        GPIO_PIN_9
  #define BUS_I2C_SDA_GPIO_PORT                  GPIOB
  #define BUS_I2C_SDA_AF                         GPIO_AF6_I2C4

  /* I2C interrupt requests */
  #define BUS_I2C_EV_IRQn                        I2C4_EV_IRQn
  #define BUS_I2C_ER_IRQn                        I2C4_ER_IRQn
#endif /* UTIL_USE_PMIC */

#ifndef BUS_I2C_FREQUENCY
   #define BUS_I2C_FREQUENCY  100000U /* Frequency of I2Cn = 100 KHz*/
#endif

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from Bus clock (HSI) = 64 MHz */

#ifndef BUS_I2Cx_TIMING
#define BUS_I2Cx_TIMING                      ((uint32_t)0x10805E89U)
#endif /* BUS_I2Cx_TIMING */

/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_BUS_Exported_Types BUS Exported Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
}BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/** @addtogroup STM32MP13XX_DISCO_BUS_Exported_Variables BUS Exported Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup  STM32MP13XX_DISCO_BUS_Exported_Functions BUS Exported Functions
  * @{
  */
int32_t BSP_I2C_Init(void);
int32_t BSP_I2C_DeInit(void);
int32_t BSP_I2C_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_Recv(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_Send(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_GetTick(void);

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
__weak HAL_StatusTypeDef MX_I2C_Init(I2C_HandleTypeDef *phI2c);

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

#endif
