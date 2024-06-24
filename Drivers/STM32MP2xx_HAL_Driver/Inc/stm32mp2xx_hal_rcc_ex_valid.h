/**
  ******************************************************************************
  * @file    stm32mp2xx_hal_rcc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of RCC HAL Extension module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020 STMicroelectronics</center></h2>
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
#ifndef __STM32MP2xx_HAL_RCC_EX_H
#define __STM32MP2xx_HAL_RCC_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal_def.h"

/** @defgroup RCCEx_TIMG1_Prescaler_Selection TIMG1 Prescaler Selection
  * @{
  */
#define RCC_TIMG1PRES_DEACTIVATED  0x0
#define RCC_TIMG1PRES_ACTIVATED    RCC_TIMG1PRER_TIMG1PRE

#define IS_RCC_TIMG1PRES(PRES)  (((PRES) == RCC_TIMG1PRES_DEACTIVATED)    || \
                                 ((PRES) == RCC_TIMG1PRES_ACTIVATED))
/**
  * @}
  */

/** @defgroup RCCEx_TIMG2_Prescaler_Selection TIMG2 Prescaler Selection
  * @{
  */
#define RCC_TIMG2PRES_DEACTIVATED  0x0
#define RCC_TIMG2PRES_ACTIVATED    RCC_TIMG2PRER_TIMG2PRE

#define IS_RCC_TIMG2PRES(PRES)  (((PRES) == RCC_TIMG2PRES_DEACTIVATED)    || \
                                 ((PRES) == RCC_TIMG2PRES_ACTIVATED))
/**
  * @}
  */

/** @defgroup RCCEx_RCC_BootCx RCC BootCx
  * @{
  */
#define RCC_BOOT_C1                     RCC_STBYBOOTCR_CPU1_HW_BEN
#define RCC_BOOT_C2                     RCC_STBYBOOTCR_CPU2_HW_BEN

#define IS_RCC_BOOT_CORE(CORE)          (((CORE) == RCC_BOOT_C1)  || \
                                         ((CORE) == RCC_BOOT_C2)  || \
                                         ((CORE) == (RCC_BOOT_C1 | RCC_BOOT_C2) ))
/**
  * @}
  */
/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Macros RCCEx Exported Macros
  * @{
  */

/**
  * @brief  Macro to set the prescaler for APB1 domain's timers
  * @note   Set and reset by software to control the clock frequency of all the timers connected to APB1 domain.
  *         It concerns TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM10, TIM11, TIM12, TIM13 and TIM14.
  * @param  __RCC_TIMG1PRES__: specifies the Timers clocks prescaler selection
  *         This parameter can be one of the following values:
  *         @arg RCC_TIMG1PRES_DEACTIVATED: The Timers kernel clock is equal to ck_icn_mcu if APB1DIV is corresponding to a division
  *                                         by 1 or 2, else it is equal to 2 x Fck_icn_apb1 (default after reset)
  *         @arg RCC_TIMG1PRES_ACTIVATED:   The Timers kernel clock is equal to ck_icn_mcu if APB1DIV is corresponding to division
  *                                         by 1, 2 or 4, else it is equal to 4 x Fck_icn_apb1
  */
#define __HAL_RCC_TIMG1PRES(__RCC_TIMG1PRES__) \
  do{  MODIFY_REG( RCC->TIMG1PRER, RCC_TIMG1PRER_TIMG1PRE , __RCC_TIMG1PRES__ );\
  } while(0)

/** @brief  Macro to get the APB1 timer clock prescaler.
  * @retval The APB1 timer clock prescaler. The returned value can be one
  *         of the following:
  *         - RCC_TIMG1PRES_DEACTIVATED: The Timers kernel clock is equal to ck_icn_mcu if APB1DIV is corresponding to a division
  *                                      by 1 or 2, else it is equal to 2 x Fck_icn_apb1 (default after reset)
  *         - RCC_TIMG1PRES_ACTIVATED:   The Timers kernel clock is equal to ck_icn_mcu if APB1DIV is corresponding to division
  *                                      by 1, 2 or 4, else it is equal to 4 x Fck_icn_apb1
  */
#define __HAL_RCC_GET_TIMG1PRES() ((uint32_t)(RCC->TIMG1PRER & RCC_TIMG1PRER_TIMG1PRE))

/**
  * @brief  Macro to set the prescaler for APB2 domain's timers
  * @note   Set and reset by software to control the clock frequency of all the timers connected to APB2 domain.
  *         It concerns TIM1, TIM8, TIM15, TIM16, TIM17 and TIM20.
  * @param  __RCC_TIMG2PRES__: specifies the timers clocks prescaler selection
  *         This parameter can be one of the following values:
  *         @arg RCC_TIMG2PRES_DEACTIVATED:  The Timers kernel clock is equal to ck_icn_mcu if APB2DIV is corresponding to a division
  *                                          by 1 or 2, else it is equal to 2 x Fck_icn_apb2 (default after reset)
  *         @arg RCC_TIMG2PRES_ACTIVATED:    The Timers kernel clock is equal to ck_icn_mcu if APB2DIV is corresponding to division
  *                                          by 1, 2 or 4, else it is equal to 4 x Fck_icn_apb2
  */
#define __HAL_RCC_TIMG2PRES(__RCC_TIMG2PRES__) \
  do{  MODIFY_REG( RCC->TIMG2PRER, RCC_TIMG2PRER_TIMG2PRE , __RCC_TIMG2PRES__ );\
  } while(0)

/** @brief  Macro to get the APB2 timer clock prescaler.
  * @retval The APB2 timer clock prescaler. The returned value can be one
  *         of the following:
  *         - RCC_TIMG2PRES_DEACTIVATED:  The Timers kernel clock is equal to ck_icn_mcu if APB2DIV is corresponding to a division
  *                                       by 1 or 2, else it is equal to 2 x Fck_icn_apb2 (default after reset)
  *         - RCC_TIMG2PRES_ACTIVATED:    The Timers kernel clock is equal to ck_icn_mcu if APB2DIV is corresponding to division
  *                                       by 1, 2 or 4, else it is equal to 4 x Fck_icn_apb2
  */
#define __HAL_RCC_GET_TIMG2PRES() ((uint32_t)(RCC->TIMG2PRER & RCC_TIMG2PRER_TIMG2PRE))

// #define USB_PHY_VALUE    ((uint32_t)48000000) /*!< Value of the USB_PHY_VALUE signal in Hz
// It is equal to rcc_hsphy_CLK_48M which is
// a constant value */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  * @{
  */

/**
  * @}
  */

/** @addtogroup RCCEx_Exported_Functions_Group2
  * @{
  */

// void HAL_RCCEx_EnableLSECSS(void);// FCh: Obsolete. Replace by HAL_RCC_EnableCSS()
// void HAL_RCCEx_DisableLSECSS(void); // FCh: Obsolete. LSECSS can't be disabled by SW.

#ifdef CORE_CA35
void HAL_RCCEx_EnableBootCore(uint32_t RCC_BootCx);
void HAL_RCCEx_DisableBootCore(uint32_t RCC_BootCx);
void HAL_RCCEx_HoldBootMCU(void);
void HAL_RCCEx_BootMCU(void);
#endif /* CORE_CA35 */

HAL_StatusTypeDef HAL_RCCEx_GetPLLParams(uint32_t fref, uint32_t fvco, RCC_PLLCfgTypeDef *pll);

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

#endif /* __STM32MP2xx_HAL_RCC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
