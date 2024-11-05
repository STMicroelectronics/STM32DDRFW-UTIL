/**
  ******************************************************************************
  * @file    stm32mp257f_disco.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32MP257F-DK:
  *          LEDs
  *          push-buttons
  *          COM ports
  *          hardware resources.
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
#ifndef STM32MP257F_DK
#define STM32MP257F_DK

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp257f_disco_conf.h"
#include "stm32mp257f_disco_errno.h"
#include "res_mgr.h"
#include "stm32mp_util_conf.h"

#if (USE_BSP_COM_FEATURE > 0)
#if (USE_COM_LOG > 0)
#ifndef __GNUC__
#include "stdio.h"
#endif /* __GNUC__ */
#endif /* USE_COM_LOG */
#endif /* USE_BSP_COM_FEATURE */

#if !defined (USE_BSP)
#define USE_BSP
#endif /* USE_BSP */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP257F_DK
  * @{
  */

/** @addtogroup LOW_LEVEL
  * @{
  */

/** @defgroup Exported_Types Exported Types
  * @{
  */
typedef enum
{
  LED1 = 0U,
  LED_RED = LED1,
  LED2 = 1U,
  LED_GREEN = LED2,
  LED3 = 2U,
  LED_ORANGE = LED3,
  LED4 = 3U,
  LED_BLUE = LED4,
  LEDn
} Led_TypeDef;

typedef enum
{
  BUTTON_WAKEUP = 0U,
  BUTTON_USER1,
  BUTTON_USER2,
  BUTTON_TAMPER,
  BUTTONn
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0U,
  BUTTON_MODE_EXTI = 1U
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  /* Define COM_VCP_CA35, COM_VCP_CM33, COM_CM0PLUS to match COM_USART[] table in stm32mp257f_eval.c
   * These values are to be used in examples
   */
  COM_VCP_CA35 = 0U,
  COM_VCP_CM33 = 1U,
  COM_CM0PLUS = 2U,
  /* Define COM1, COM2, COM3 aliases
   * These values are to be used in Validation applications
   * They allow to use same COM<n> whatever the used core
   * and avoid ifdefs in validation code
   */
#if defined(CORE_CA35)
  COM1 = COM_VCP_CA35,
  COM2 = COM_VCP_CM33,
  COM3 = COM_CM0PLUS,
#elif defined(CORE_CM33)
  COM1 = COM_VCP_CM33,
  COM2 = COM_VCP_CA35,
  COM3 = COM_CM0PLUS,
#elif defined(CORE_CM0PLUS)
  COM1 = COM_CM0PLUS,
  COM2 = COM_VCP_CA35,
  COM3 = COM_CM0PLUS,
#endif /* CORE_<...> */
  COMn
} COM_TypeDef;

typedef enum
{
  COM_STOPBITS_1     =   UART_STOPBITS_1,
  COM_STOPBITS_2     =   UART_STOPBITS_2,
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE     =  UART_PARITY_NONE,
  COM_PARITY_EVEN     =  UART_PARITY_EVEN,
  COM_PARITY_ODD      =  UART_PARITY_ODD,
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
} COM_WordLengthTypeDef;

typedef struct
{
  uint32_t              BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef   StopBits;
  COM_ParityTypeDef     Parity;
  COM_HwFlowCtlTypeDef  HwFlowCtl;
} COM_InitTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
} BSP_COM_Cb_t;
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
#endif /* USE_BSP_COM_FEATURE */

/**
  * @}
  */

/** @defgroup Exported_Constants Exported Constants
  * @{
  */
/**
  * @brief  Define for STM32MP257F DK board
  */
#if !defined (USE_STM32MP257F_DK)
#define USE_STM32MP257F_DK
#endif /* USE_STM32MP257F_DK */
/**
  * @brief STM32MP257F DK BSP Driver version number VX.Y.Z
  */
#define STM32MP257F_DK_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define STM32MP257F_DK_BSP_VERSION_SUB1   (0x01U) /*!< [23:16] sub1 version */
#define STM32MP257F_DK_BSP_VERSION_SUB2   (0x00U) /*!< [15:8]  sub2 version */
#define STM32MP257F_DK_BSP_VERSION_RC     (0x04U) /*!< [7:0]  release candidate */
#define STM32MP257F_DK_BSP_VERSION        ((STM32MP257F_DK_BSP_VERSION_MAIN << 24)\
                                           |(STM32MP257F_DK_BSP_VERSION_SUB1 << 16)\
                                           |(STM32MP257F_DK_BSP_VERSION_SUB2 << 8 )\
                                           |(STM32MP257F_DK_BSP_VERSION_RC))
#define STM32MP257F_DK_BSP_BOARD_NAME  "STM32MP257F-DK";
#define STM32MP257F_DK_BSP_BOARD_ID    "MB1605-A01";

/** @defgroup LOW_LEVEL_LED EVAL LOW LEVEL LED
  * @{
  */
#if defined (CORE_CA35) || defined (CORE_CM33)
#define LED1_GPIO_PORT                   GPIOH
#define LED1_PIN                         GPIO_PIN_4
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOH_CLK_DISABLE()
#define LED1_GPIO_IS_CLK_ENABLED()       __HAL_RCC_GPIOH_IS_CLK_ENABLED()
#define LED1_GPIO_RIF_RES_TYP_GPIO       RESMGR_RESOURCE_RIF_GPIOH
#define LED1_GPIO_RIF_RES_NUM_GPIO       RESMGR_GPIO_PIN(4)

#if defined (USE_STM32MP257F_DK)
#define LED2_GPIO_PORT                   GPIOH
#define LED2_PIN                         GPIO_PIN_5
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOH_CLK_DISABLE()
#define LED2_GPIO_IS_CLK_ENABLED()       __HAL_RCC_GPIOH_IS_CLK_ENABLED()
#define LED2_GPIO_RIF_RES_TYP_GPIO       RESMGR_RESOURCE_RIF_GPIOH
#define LED2_GPIO_RIF_RES_NUM_GPIO       RESMGR_GPIO_PIN(5)

#define LED3_GPIO_PORT                   GPIOH
#define LED3_PIN                         GPIO_PIN_6
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOH_CLK_DISABLE()
#define LED3_GPIO_IS_CLK_ENABLED()       __HAL_RCC_GPIOH_IS_CLK_ENABLED()
#define LED3_GPIO_RIF_RES_TYP_GPIO       RESMGR_RESOURCE_RIF_GPIOH
#define LED3_GPIO_RIF_RES_NUM_GPIO       RESMGR_GPIO_PIN(6)

#define LED4_GPIO_PORT                   GPIOH
#define LED4_PIN                         GPIO_PIN_7
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOH_CLK_DISABLE()
#define LED4_GPIO_IS_CLK_ENABLED()       __HAL_RCC_GPIOH_IS_CLK_ENABLED()
#define LED4_GPIO_RIF_RES_TYP_GPIO       RESMGR_RESOURCE_RIF_GPIOH
#define LED4_GPIO_RIF_RES_NUM_GPIO       RESMGR_GPIO_PIN(7)

#else
#error "Please define target board"
#endif /* USE_STM32MP257F_DK */
#elif defined (CORE_CM0PLUS)

/* Core M0 can't manage LED in DK Board */
#endif /* CORE_CA35 || CORE_CM33 */
/**
  * @}
  */

/** @defgroup LOW_LEVEL_BUTTON LOW LEVEL BUTTON
  * @{
  */
/* Button state */
#define BUTTON_RELEASED                    0U
#define BUTTON_PRESSED                     1U
/**
  * @brief Wakeup push-button
  */
#if defined (CORE_CA35) || defined (CORE_CM33)
#define BUTTON_WAKEUP_PIN                   GPIO_PIN_0
#define BUTTON_WAKEUP_GPIO_PORT             GPIOA
#define BUTTON_WAKEUP_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_WAKEUP_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUTTON_WAKEUP_GPIO_IS_CLK_ENABLE()  __HAL_RCC_GPIOA_IS_CLK_ENABLED()
#define BUTTON_WAKEUP_EXTI_IRQn             EXTI2_0_IRQn
#define BUTTON_WAKEUP_EXTI_LINE             EXTI2_LINE_0
#define BUTTON_WAKEUP_RIF_RES_TYP_GPIO      RESMGR_RESOURCE_RIF_GPIOA
#define BUTTON_WAKEUP_RIF_RES_NUM_GPIO      RESMGR_GPIO_PIN(0)
#endif /* CORE_CA35 || CORE_CM33 */
/**
  * @brief USER1 push-button
  */
#if defined (CORE_CA35) || defined (CORE_CM33)
#define BUTTON_USER1_PIN                   GPIO_PIN_5
#define BUTTON_USER1_GPIO_PORT             GPIOC
#define BUTTON_USER1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER1_GPIO_IS_CLK_ENABLE()  __HAL_RCC_GPIOC_IS_CLK_ENABLED()
#define BUTTON_USER1_EXTI_IRQn             EXTI2_5_IRQn
#define BUTTON_USER1_EXTI_IRQ_HANDLER      EXTI2_5_IRQHandler
#define BUTTON_USER1_EXTI_LINE             EXTI2_LINE_5
#define BUTTON_USER1_RIF_RES_TYP_GPIO      RESMGR_RESOURCE_RIF_GPIOC
#define BUTTON_USER1_RIF_RES_NUM_GPIO      RESMGR_GPIO_PIN(5)
#endif /* CORE_CA35 || CORE_CM33 */
/**
  * @brief USER2 push-button
  */
#if defined (CORE_CA35) || defined (CORE_CM33)
#define BUTTON_USER2_PIN                   GPIO_PIN_11
#define BUTTON_USER2_GPIO_PORT             GPIOC
#define BUTTON_USER2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER2_GPIO_IS_CLK_ENABLE()  __HAL_RCC_GPIOC_IS_CLK_ENABLED()
#define BUTTON_USER2_EXTI_IRQn             EXTI2_11_IRQn
#define BUTTON_USER2_EXTI_IRQ_HANDLER      EXTI2_11_IRQHandler
#define BUTTON_USER2_EXTI_LINE             EXTI2_LINE_11
#define BUTTON_USER2_RIF_RES_TYP_GPIO      RESMGR_RESOURCE_RIF_GPIOC
#define BUTTON_USER2_RIF_RES_NUM_GPIO      RESMGR_GPIO_PIN(11)
#endif /* CORE_CA35 || CORE_CM33 */
/**
  * @brief TAMPER push-button
  */
#if defined (CORE_CA35) || defined (CORE_CM33)
#define BUTTON_TAMPER_PIN                   GPIO_PIN_2
#define BUTTON_TAMPER_GPIO_PORT             GPIOZ
#define BUTTON_TAMPER_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOZ_CLK_ENABLE()
#define BUTTON_TAMPER_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOZ_CLK_DISABLE()
#define BUTTON_TAMPER_GPIO_IS_CLK_ENABLE()  __HAL_RCC_GPIOZ_IS_CLK_ENABLED()
#define BUTTON_TAMPER_EXTI_IRQn             EXTI2_2_IRQn
#define BUTTON_TAMPER_EXTI_IRQ_HANDLER      EXTI2_2_IRQHandler
#define BUTTON_TAMPER_EXTI_LINE             EXTI2_LINE_2
#define BUTTON_TAMPER_RIF_RES_TYP_GPIO      RESMGR_RESOURCE_RIF_GPIOZ
#define BUTTON_TAMPER_RIF_RES_NUM_GPIO      RESMGR_GPIO_PIN(2)
#endif /* CORE_CA35 || CORE_CM33 */
/**
  * @}
  */

/** @defgroup DK_LOW_LEVEL_COM LOW LEVEL COM
  * @{
  */
#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief Definition for COM port1, connected to USART2
  */
#if defined (CORE_CA35) || defined (CORE_CM33)

/*********************** COM_CA35 ******************************/
#if (UTIL_UART_INSTANCE == UTIL_USART1)
  #define COM_CA35_UART                       USART1
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_USART1_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_USART1_ID
#elif (UTIL_UART_INSTANCE == UTIL_USART2)
  #define COM_CA35_UART                       USART2
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_USART2_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_USART2_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_USART2_ID
#elif (UTIL_UART_INSTANCE == UTIL_USART3)
  #define COM_CA35_UART                       USART3
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_USART3_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_USART3_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_USART3_ID
#elif (UTIL_UART_INSTANCE == UTIL_UART4)
  #define COM_CA35_UART                       UART4
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_UART4_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_UART4_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_UART4_ID
#elif (UTIL_UART_INSTANCE == UTIL_UART5)
  #define COM_CA35_UART                       UART5
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_UART5_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_UART5_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_UART5_ID
#elif (UTIL_UART_INSTANCE == UTIL_USART6)
  #define COM_CA35_UART                       USART6
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_USART6_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_USART6_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_USART6_ID
#elif (UTIL_UART_INSTANCE == UTIL_UART7)
  #define COM_CA35_UART                       UART7
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_UART7_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_UART7_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_UART7_ID
#elif (UTIL_UART_INSTANCE == UTIL_UART8)
  #define COM_CA35_UART                       UART8
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_UART8_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_UART8_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_UART8_ID
#elif (UTIL_UART_INSTANCE == UTIL_UART9)
  #define COM_CA35_UART                       UART9
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_UART9_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_UART9_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_UART9_ID
#elif (UTIL_UART_INSTANCE == UTIL_LPUART1)
  #define COM_CA35_UART                       LPUART1
  #define COM_CA35_CLK_ENABLE()             __HAL_RCC_LPUART1_CLK_ENABLE()
  #define COM_CA35_CLK_DISABLE()            __HAL_RCC_LPUART1_CLK_DISABLE()
  #define COM_CA35_RIF_RES_NUM_UART         RESMGR_RIFSC_LPUART1_ID
#else
  #error "unknown UTIL_UART_INSTANCE definition."
#endif

#if (UTIL_UART_TX_PIN == UTIL_PIN0)
#define COM_CA35_TX_PIN                     GPIO_PIN_0
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(0)
#elif (UTIL_UART_TX_PIN == UTIL_PIN1)
#define COM_CA35_TX_PIN                     GPIO_PIN_1
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(1)
#elif (UTIL_UART_TX_PIN == UTIL_PIN2)
#define COM_CA35_TX_PIN                     GPIO_PIN_2
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(2)
#elif (UTIL_UART_TX_PIN == UTIL_PIN3)
#define COM_CA35_TX_PIN                     GPIO_PIN_3
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(3)
#elif (UTIL_UART_TX_PIN == UTIL_PIN4)
#define COM_CA35_TX_PIN                     GPIO_PIN_4
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(4)
#elif (UTIL_UART_TX_PIN == UTIL_PIN5)
#define COM_CA35_TX_PIN                     GPIO_PIN_5
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(5)
#elif (UTIL_UART_TX_PIN == UTIL_PIN6)
#define COM_CA35_TX_PIN                     GPIO_PIN_6
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(6)
#elif (UTIL_UART_TX_PIN == UTIL_PIN7)
#define COM_CA35_TX_PIN                     GPIO_PIN_7
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(7)
#elif (UTIL_UART_TX_PIN == UTIL_PIN8)
#define COM_CA35_TX_PIN                     GPIO_PIN_8
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(8)
#elif (UTIL_UART_TX_PIN == UTIL_PIN9)
#define COM_CA35_TX_PIN                     GPIO_PIN_9
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(9)
#elif (UTIL_UART_TX_PIN == UTIL_PIN10)
#define COM_CA35_TX_PIN                     GPIO_PIN_10
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(10)
#elif (UTIL_UART_TX_PIN == UTIL_PIN11)
#define COM_CA35_TX_PIN                     GPIO_PIN_11
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(11)
#elif (UTIL_UART_TX_PIN == UTIL_PIN12)
#define COM_CA35_TX_PIN                     GPIO_PIN_12
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(12)
#elif (UTIL_UART_TX_PIN == UTIL_PIN13)
#define COM_CA35_TX_PIN                     GPIO_PIN_13
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(13)
#elif (UTIL_UART_TX_PIN == UTIL_PIN14)
#define COM_CA35_TX_PIN                     GPIO_PIN_14
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(14)
#elif (UTIL_UART_TX_PIN == UTIL_PIN15)
#define COM_CA35_TX_PIN                     GPIO_PIN_15
#define COM_CA35_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(15)
#else
  #error "unknown UTIL_UART_TX_PIN definition."
#endif

#if (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOA)
  #define COM_CA35_TX_GPIO_PORT             GPIOA
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(90)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOB)
  #define COM_CA35_TX_GPIO_PORT             GPIOB
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(91)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOC)
  #define COM_CA35_TX_GPIO_PORT             GPIOC
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(92)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOD)
  #define COM_CA35_TX_GPIO_PORT             GPIOD
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(93)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOE)
  #define COM_CA35_TX_GPIO_PORT             GPIOE
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(94)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOF)
  #define COM_CA35_TX_GPIO_PORT             GPIOF
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(95)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOG)
  #define COM_CA35_TX_GPIO_PORT             GPIOG
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(96)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOH)
  #define COM_CA35_TX_GPIO_PORT             GPIOH
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOH_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOH_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(97)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOI)
  #define COM_CA35_TX_GPIO_PORT             GPIOI
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOI_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(98)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOJ)
  #define COM_CA35_TX_GPIO_PORT             GPIOJ
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOJ_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOJ_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(99)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOK)
  #define COM_CA35_TX_GPIO_PORT             GPIOK
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOK_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOK_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(100)
#elif (UTIL_UART_TX_GPIO_PORT == UTIL_GPIOZ)
  #define COM_CA35_TX_GPIO_PORT             GPIOZ
  #define COM_CA35_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOZ_CLK_ENABLE()
  #define COM_CA35_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOZ_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_TX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_TX_PIN           RESMGR_RCC_RESOURCE(101)
#else
  #error "unknown UTIL_UART_TX_GPIO_PORT definition."
#endif

#define COM_CA35_TX_AF                      UTIL_UART_TX_AF

#if (UTIL_UART_RX_PIN == UTIL_PIN0)
#define COM_CA35_RX_PIN                     GPIO_PIN_0
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(0)
#elif (UTIL_UART_RX_PIN == UTIL_PIN1)
#define COM_CA35_RX_PIN                     GPIO_PIN_1
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(1)
#elif (UTIL_UART_RX_PIN == UTIL_PIN2)
#define COM_CA35_RX_PIN                     GPIO_PIN_2
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(2)
#elif (UTIL_UART_RX_PIN == UTIL_PIN3)
#define COM_CA35_RX_PIN                     GPIO_PIN_3
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(3)
#elif (UTIL_UART_RX_PIN == UTIL_PIN4)
#define COM_CA35_RX_PIN                     GPIO_PIN_4
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(4)
#elif (UTIL_UART_RX_PIN == UTIL_PIN5)
#define COM_CA35_RX_PIN                     GPIO_PIN_5
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(5)
#elif (UTIL_UART_RX_PIN == UTIL_PIN6)
#define COM_CA35_RX_PIN                     GPIO_PIN_6
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(6)
#elif (UTIL_UART_RX_PIN == UTIL_PIN7)
#define COM_CA35_RX_PIN                     GPIO_PIN_7
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(7)
#elif (UTIL_UART_RX_PIN == UTIL_PIN8)
#define COM_CA35_RX_PIN                     GPIO_PIN_8
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(8)
#elif (UTIL_UART_RX_PIN == UTIL_PIN9)
#define COM_CA35_RX_PIN                     GPIO_PIN_9
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(9)
#elif (UTIL_UART_RX_PIN == UTIL_PIN10)
#define COM_CA35_RX_PIN                     GPIO_PIN_10
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(10)
#elif (UTIL_UART_RX_PIN == UTIL_PIN11)
#define COM_CA35_RX_PIN                     GPIO_PIN_11
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(11)
#elif (UTIL_UART_RX_PIN == UTIL_PIN12)
#define COM_CA35_RX_PIN                     GPIO_PIN_12
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(12)
#elif (UTIL_UART_RX_PIN == UTIL_PIN13)
#define COM_CA35_RX_PIN                     GPIO_PIN_13
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(13)
#elif (UTIL_UART_RX_PIN == UTIL_PIN14)
#define COM_CA35_RX_PIN                     GPIO_PIN_14
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(14)
#elif (UTIL_UART_RX_PIN == UTIL_PIN15)
#define COM_CA35_RX_PIN                     GPIO_PIN_15
#define COM_CA35_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(15)
#else
  #error "unknown UTIL_UART_RX_PIN definition."
#endif

#if (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOA)
  #define COM_CA35_RX_GPIO_PORT             GPIOA
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(90)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOB)
  #define COM_CA35_RX_GPIO_PORT             GPIOB
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(91)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOC)
  #define COM_CA35_RX_GPIO_PORT             GPIOC
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(92)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOD)
  #define COM_CA35_RX_GPIO_PORT             GPIOD
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(93)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOE)
  #define COM_CA35_RX_GPIO_PORT             GPIOE
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(94)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOF)
  #define COM_CA35_RX_GPIO_PORT             GPIOF
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(95)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOG)
  #define COM_CA35_RX_GPIO_PORT             GPIOG
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(96)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOH)
  #define COM_CA35_RX_GPIO_PORT             GPIOH
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOH_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOH_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(97)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOI)
  #define COM_CA35_RX_GPIO_PORT             GPIOI
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOI_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(98)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOJ)
  #define COM_CA35_RX_GPIO_PORT             GPIOJ
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOJ_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOJ_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(99)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOK)
  #define COM_CA35_RX_GPIO_PORT             GPIOK
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOK_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOK_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(100)
#elif (UTIL_UART_RX_GPIO_PORT == UTIL_GPIOZ)
  #define COM_CA35_RX_GPIO_PORT             GPIOZ
  #define COM_CA35_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOZ_CLK_ENABLE()
  #define COM_CA35_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOZ_CLK_DISABLE()
  #define COM_CA35_RIF_RES_TYP_RX_PIN       RESMGR_RESOURCE_RIF_GPIOA
  #define COM_CA35_RCC_RES_RX_PIN           RESMGR_RCC_RESOURCE(101)
#else
  #error "unknown UTIL_UART_RX_GPIO_PORT definition."
#endif

#define COM_CA35_RX_AF                      UTIL_UART_RX_AF

/*********************** COM_CM33 ******************************/
#define COM_CM33_UART                       UART5 /* VALID1/3 */
#define COM_CM33_CLK_ENABLE()               __HAL_RCC_UART5_CLK_ENABLE()
#define COM_CM33_CLK_DISABLE()              __HAL_RCC_UART5_CLK_DISABLE()

#define COM_CM33_TX_PIN                     GPIO_PIN_9
#define COM_CM33_TX_GPIO_PORT               GPIOG
#define COM_CM33_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define COM_CM33_TX_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOG_CLK_DISABLE()
#define COM_CM33_TX_AF                      GPIO_AF5_UART5

#define COM_CM33_RX_PIN                     GPIO_PIN_10
#define COM_CM33_RX_GPIO_PORT               GPIOG
#define COM_CM33_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define COM_CM33_RX_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOG_CLK_DISABLE()
#define COM_CM33_RX_AF                      GPIO_AF5_UART5


#define COM_CM33_RIF_RES_TYP_UART           RESMGR_RESOURCE_RIFSC
#define COM_CM33_RIF_RES_NUM_UART           RESMGR_RIFSC_UART5_ID

#define COM_CM33_RIF_RES_TYP_TX_PIN         RESMGR_RESOURCE_RIF_GPIOG
#define COM_CM33_RIF_RES_NUM_TX_PIN         RESMGR_GPIO_PIN(9)

#define COM_CM33_RIF_RES_TYP_RX_PIN         RESMGR_RESOURCE_RIF_GPIOG
#define COM_CM33_RIF_RES_NUM_RX_PIN         RESMGR_GPIO_PIN(10)

#define COM_CM33_TX_GPIO_IS_CLK_ENABLED()   __HAL_RCC_GPIOG_IS_CLK_ENABLED()
#define COM_CM33_RX_GPIO_IS_CLK_ENABLED()   __HAL_RCC_GPIOG_IS_CLK_ENABLED()


#endif /* CORE_CA35 || CORE_CM33 */

/*********************** COM_CM0PLUS ******************************/
#define COM_CM0PLUS_UART                    LPUART1
#define COM_CM0PLUS_CLK_ENABLE()            __HAL_RCC_LPUART1_CLK_ENABLE()
#define COM_CM0PLUS_CLK_DISABLE()           __HAL_RCC_LPUART1_CLK_DISABLE()

#define COM_CM0PLUS_TX_PIN                  GPIO_PIN_7
#define COM_CM0PLUS_TX_GPIO_PORT            GPIOZ
#define COM_CM0PLUS_TX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOZ_CLK_ENABLE()
#define COM_CM0PLUS_TX_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOZ_CLK_DISABLE()
#define COM_CM0PLUS_TX_AF                   GPIO_AF6_LPUART1

#define COM_CM0PLUS_RX_PIN                  GPIO_PIN_8
#define COM_CM0PLUS_RX_GPIO_PORT            GPIOZ
#define COM_CM0PLUS_RX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOZ_CLK_ENABLE()
#define COM_CM0PLUS_RX_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOZ_CLK_DISABLE()
#define COM_CM0PLUS_RX_AF                   GPIO_AF6_LPUART1

#define COM_POLL_TIMEOUT              1000
#define COM1_RX_TIMEOUT               HAL_MAX_DELAY
#define MX_UART_InitTypeDef COM_InitTypeDef
#endif /* USE_BSP_COM_FEATURE */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup Exported_Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
extern USART_TypeDef *COM_USART[];
#endif /* USE_BSP_COM_FEATURE */

/**
  * @}
  */

/** @addtogroup Exported_Functions
  * @{
  */
int32_t BSP_GetVersion(void);
const uint8_t *BSP_GetBoardName(void);
const uint8_t *BSP_GetBoardID(void);

int32_t BSP_LED_Init(Led_TypeDef Led);
int32_t BSP_LED_DeInit(Led_TypeDef Led);
int32_t BSP_LED_On(Led_TypeDef Led);
int32_t BSP_LED_Off(Led_TypeDef Led);
int32_t BSP_LED_Toggle(Led_TypeDef Led);
int32_t BSP_LED_GetState(Led_TypeDef Led);
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t BSP_PB_DeInit(Button_TypeDef Button);
int32_t BSP_PB_GetState(Button_TypeDef Button);
void    BSP_PB_Callback(Button_TypeDef Button);
#if (USE_BSP_COM_FEATURE > 0)
int32_t  BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t  BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG > 0)
int32_t  BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif /* USE_COM_LOG */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_USART_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
#endif /* USE_BSP_COM_FEATURE */

void BSP_PB_IRQHandler(Button_TypeDef Button);

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

#endif /* STM32257CXX_EVX */
