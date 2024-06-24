/**
  ******************************************************************************
  * @file    stm32mp2xx_hal_conf.h
  * @author  MCD Application Team
  * @brief   HAL configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32MP2xx_HAL_CONF_H
#define STM32MP2xx_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */
#define HAL_MODULE_ENABLED

#if defined (CORE_CA35)
  #define HAL_A35_WRAPPER_ENABLED
#endif /* CORE_CA35 */

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_ADC_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_BSEC_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#if defined (CORE_CM33) || defined (CORE_CM0PLUS)
  #define HAL_CORTEX_MODULE_ENABLED
#endif /* CORE_CM33 || CORE_CM0PLUS */

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_CRC_MODULE_ENABLED
//  #define HAL_CRYP_MODULE_ENABLED
//  #define HAL_CSI_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#if defined (CORE_CM33)
//  #define HAL_DCACHE_MODULE_ENABLED
#endif /* CORE_CM33 */

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_DCMI_MODULE_ENABLED
//  #define HAL_DCMIPP_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#if defined (CORE_CA35) || defined (CORE_CM33)
  #define HAL_DDR_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#define HAL_DMA_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_DSI_MODULE_ENABLED
//  #define HAL_ETH_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#define HAL_EXTI_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_FDCAN_MODULE_ENABLED
//  #define HAL_FLASH_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#define HAL_GPIO_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_HASH_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

//#define HAL_HSEM_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_I2S_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

/* #define HAL_I3C_MODULE_ENABLED */

#if defined (CORE_CM33)
//  #define HAL_ICACHE_MODULE_ENABLED
#endif /* CORE_CM33 */

//#define HAL_IPCC_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_IRDA_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

//#define HAL_IWDG_MODULE_ENABLED
//#define HAL_LPTIM_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_LTDC_MODULE_ENABLED
//  #define HAL_LVDS_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

//#define HAL_MDF_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_MMC_MODULE_ENABLED
//  #define HAL_NAND_MODULE_ENABLED
//  #define HAL_NOR_MODULE_ENABLED
//  #define HAL_OSPI_MODULE_ENABLED
//  #define HAL_OTFDEC_MODULE_ENABLED
//  #define HAL_PCD_MODULE_ENABLED
//  #define HAL_PKA_MODULE_ENABLED
//  #define HAL_PSSI_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#define HAL_PWR_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_RAMCFG_MODULE_ENABLED
  #define HAL_RCC_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#define HAL_RIF_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_RNG_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#define HAL_RTC_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
/*   #define HAL_SAES_MODULE_ENABLED */
//  #define HAL_SAI_MODULE_ENABLED
//  #define HAL_SD_MODULE_ENABLED
//  #define HAL_SMARTCARD_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

//#define HAL_SMBUS_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_SPDIFRX_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

//#define HAL_SPI_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
//  #define HAL_SRAM_MODULE_ENABLED
//  #define HAL_TIM_MODULE_ENABLED
//  #define HAL_TMPSENS_MODULE_ENABLED
//  #define HAL_UCPD_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

#define HAL_UART_MODULE_ENABLED

#if defined (CORE_CA35) || defined (CORE_CM33)
  #define HAL_USART_MODULE_ENABLED
#endif /* CORE_CA35 || CORE_CM33 */

//#define HAL_WWDG_MODULE_ENABLED

/* ######################### Oscillator Values adaptation ################### */
/* Define here oscillators values if you have to override default values set in BSP */

/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).
  */
#define HSE_VALUE             ((uint32_t) 40000000U)      /*!< Value of the External High Speed oscillator in Hz */

/**
  * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
  *        Timeout value
  */
#define HSE_STARTUP_TIMEOUT   ((uint32_t)100U)        /*!< Time out for HSE start up, in ms */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL).
  */
#define HSI_VALUE             ((uint32_t) 64000000U)      /*!< Value of the Internal High Speed oscillator in Hz*/

/**
  * @brief In the following line adjust the Internal High Speed oscillator (HSI) Startup
  *        Timeout value
  */
#define HSI_STARTUP_TIMEOUT   ((uint32_t)5000U)       /*!< Time out for HSI start up */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#define LSI_VALUE             ((uint32_t)32000U)      /*!< LSI Typical Value in Hz*/
                                                         /* The real value may vary depending on the variations */
                                                         /* in voltage and temperature. */
/**
  * @brief External Low Speed oscillator (LSE) value.
  */
#define LSE_VALUE             ((uint32_t)32768U)      /*!< Value of the External Low Speed oscillator in Hz*/

/**
  * @brief Time out for LSE start up value in ms.
  */
#define LSE_STARTUP_TIMEOUT   ((uint32_t)5000U)       /*!< Time out for LSE start up, in ms */

/**
  * @brief Internal  oscillator (CSI) default value.
  *        This value is the default CSI value after Reset.
  */
#define CSI_VALUE             ((uint32_t)4000000U)    /*!< Value of the Internal oscillator in Hz*/

/**
  * @brief External clock source for I2S peripheral
  *        This value is used by the I2S HAL module to compute the I2S clock source
  *        frequency, this source is inserted directly through I2S_CKIN pad.
  */
#define EXTERNAL_CLOCK_VALUE  ((uint32_t)12288000U)   /*!< Value of the External clock in Hz*/

/* ########################### System Configuration ######################### */
/* Define here system config values if you have to override default values set in BSP */

/* For CORE_CA35 */
// #define  TICK_INT_PRIORITY    (((uint32_t)1<<(GIC_PRIO_BITS-1)) - 1)  /*!< tick interrupt priority set by default */
                                                                      /*!< to GIC400 lowest (highest value) in secure mode */
/* For CORE_CM33, CORE_CM0PLUS */
// #define  TICK_INT_PRIORITY    (((uint32_t)1<<__NVIC_PRIO_BITS) - 1)   /*!< tick interrupt priority set by default */
                                                                      /*!< to NVIC lowest (highest value)         */
                                                                      /*  Warning: Must be set to higher priority for HAL_Delay() */
                                                                      /*  and HAL_GetTick() usage under interrupt context         */

// #define  USE_RTOS                     0U
// #define  PREFETCH_ENABLE              0U
// #define  INSTRUCTION_CACHE_ENABLE     0U
// #define  DATA_CACHE_ENABLE            0U

/* ########################## Assert Selection ############################## */
/* Define here assert config values if you have to override default values set in BSP */
// #define USE_FULL_ASSERT    1

#include "stm32mp2xx_hal_conf_system.h"
#include "stm32mp2xx_hal_conf_includes.h"

#ifdef __cplusplus
}
#endif

#endif /* STM32MP2xx_HAL_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
