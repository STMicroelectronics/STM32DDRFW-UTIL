/**
  ******************************************************************************
  * @file    stm32mp2xx_hal_rcc.h
  * @author  MCD Application Team
  * @brief   Header file of RCC HAL module.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    HAL RCC RIF driver contains a set of following user APIs :
      (+) API to set and get configuration of Security/Privilege/(Static or Dynamic) CID access filtering to RCC Local Resources
      (+) API to take and release semaphore used for Dynamic CID access to RCC Local Resources

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2021 STMicroelectronics</center></h2>
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

/**
  * @brief This HAL is based on following reference documents :
  *
  *        - "Panther_RCC (Reset and Clock Control) SOC IP user specification"
  *           Revision 1.0.25 - January 15th, 2021
  *          (file c8amba_rcc_UserSpec_Panther_1_0_25.pdf)
  *
  *        File has been organised in the following order :
  *        - clocks management
  *        - resets management
  *        - interrupts management
  */

/* Define to prevent recursive inclusion */
#ifndef __STM32MP2xx_HAL_RCC_H
#define __STM32MP2xx_HAL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32mp2xx_ll_rcc.h"
#include "stm32mp2xx_hal_def.h"

/** @addtogroup STM32MP2xx_HAL_Driver
  * @{
  */

/******************************************************************************
  * Clock frequency measurement
  ******************************************************************************/
#define APPLY_BZ121873 /* see BZ 121873 description */

/******************************************************************************
  * CLOCKS management
  ******************************************************************************/

/* Clocks identifiers list (see [RCC_Spec] "Table 16." and "Table 20.")
   Note: the following construction allows
         to easily generate strings for clock names if required */
#if ! defined(STM32MP2XX_ASSY3_1_0)

#define ALL_CLOCK_IDS(CLKID) \
  CLKID(RCC_CK_NONE) \
  /* Oscillators: managed with \
     - HAL_RCC_SetOscCfg()
     - HAL_RCC_GetOscCfg() */ \
  CLKID(RCC_CK_LSI) /* ID 1 */ \
  CLKID(RCC_CK_LSE) \
  CLKID(RCC_CK_CSI) \
  CLKID(RCC_CK_HSE) \
  CLKID(RCC_CK_HSI) \
  CLKID(RCC_CK_CSI_KER) \
  CLKID(RCC_CK_HSE_KER) \
  CLKID(RCC_CK_HSI_KER) \
  /* PLL4 to 8 ref clocks, then 1 to 3 \
     Note: index order */ \
  CLKID(RCC_CK_PLL4REF) /* ID 9 = MUXSEL0 to GFG0/PLL4 */ \
  CLKID(RCC_CK_PLL5REF) /* MUXSEL1 to GFG1/PLL5 */ \
  CLKID(RCC_CK_PLL6REF) /* MUXSEL2 to GFG2/PLL6 */ \
  CLKID(RCC_CK_PLL7REF) /* MUXSEL3 to GFG3/PLL7 */ \
  CLKID(RCC_CK_PLL8REF) /* MUXSEL4 to GFG4/PLL8 */ \
  CLKID(RCC_CK_PLL1REF) /* MUXSEL5 to CA35 PLL */ \
  CLKID(RCC_CK_PLL2REF) /* MUXSEL6 to DDR PLL */ \
  CLKID(RCC_CK_PLL3REF) /* MUXSEL7 to GPU PLL */ \
  /* RCC external PLLs */ \
  CLKID(RCC_CK_PLL1) /* ID 17 = CA35 PLL output */ \
  CLKID(RCC_CK_PLL2) /* DDR PLL output */ \
  CLKID(RCC_CK_PLL3) /* GPU PLL output */ \
  /* Flexiclockgen GFGs/PLLs outputs in GFG number order */ \
  CLKID(RCC_CK_PLL4) /* ID 20 = GFG0 */ \
  CLKID(RCC_CK_PLL5) \
  CLKID(RCC_CK_PLL6) \
  CLKID(RCC_CK_PLL7) \
  CLKID(RCC_CK_PLL8) /* GFG4 */ \
  /* Flexiclockgen outputs (in channel order). \
     Managed with
     - HAL_RCC_SetClockCfg()
     - HAL_RCC_GetClockCfg() */ \
  CLKID(RCC_CK_ICN_MCU)     /* ID 25 [channel 0] ck_icn_hs_mcu, ck_icn_ls_mcu */ \
  CLKID(RCC_CK_ICN_SDMMC)   /* ck_icn_p_dlybsd1, ck_icn_p_dlybsd2, ck_icn_p_dlybsd3, \
                                 ck_icn_m_sdmmc1, ck_icn_m_sdmmc2, ck_icn_m_sdmmc3 */ \
  CLKID(RCC_CK_ICN_DDR)     /* ck_icn_s_ddr1, ck_icn_s_ddr2, ck_icn_m_cpu1 */ \
  CLKID(RCC_CK_ICN_DISPLAY) /* ck_icn_m_ltdc, ck_icn_m_dcmipp */ \
  CLKID(RCC_CK_ICN_HSL)     /* ck_icn_m_pcie, ck_icn_m_usb3drd, ck_icn_m_usb2ehci, \
                                 ck_icn_m_usb2ohci, ck_icn_m_eth1, ck_icn_m_eth2 */ \
  CLKID(RCC_CK_ICN_NIC)     /* ck_icn_s_bootrom, ck_icn_s_gic400 */ \
  CLKID(RCC_CK_ICN_VID)     /* ck_icn_m_vdec, ck_icn_m_venc */ \
  CLKID(RCC_CK_KER_LPTIM12)    /* ck_ker_lptim1, ck_ker_lptim2 */ \
  CLKID(RCC_CK_KER_USART24)    /* ck_ker_usart2, ck_ker_usart4 */ \
  CLKID(RCC_CK_KER_USART35)    /* ck_ker_usart3, ck_ker_usart5 */ \
  CLKID(RCC_CK_KER_SPI23)      /* [channel 10] ck_ker_spi2, ck_ker_spi3 */ \
  CLKID(RCC_CK_KER_SPDIFRX) \
  CLKID(RCC_CK_KER_I2C12I3C12) /* ck_ker_i2c1, ck_ker_i2c2, ck_ker_i3c1, ck_ker_i3c2 */ \
  CLKID(RCC_CK_KER_I2C35I3C3)  /* ck_ker_i2c3, ck_ker_i2c5, ck_ker_i3c3 */ \
  CLKID(RCC_CK_KER_I2C46)      /* ck_ker_i2c4, ck_ker_i2c6 */ \
  CLKID(RCC_CK_KER_I2C7) \
  CLKID(RCC_CK_KER_SPI1) \
  CLKID(RCC_CK_KER_SPI45)      /* ck_ker_spi4, ck_ker_spi5 */ \
  CLKID(RCC_CK_KER_SPI67)      /* ck_ker_spi6, ck_ker_spi7 */ \
  CLKID(RCC_CK_KER_USART1) \
  CLKID(RCC_CK_KER_USART6)     /* [channel 20] */ \
  CLKID(RCC_CK_KER_UART78)     /* ck_ker_uart7, ck_ker_uart8 */ \
  CLKID(RCC_CK_KER_UART9) \
  CLKID(RCC_CK_KER_SAI1MDF1)   /* ck_ker_sai1, ck_ker_mdf1 */ \
  CLKID(RCC_CK_KER_SAI2) \
  CLKID(RCC_CK_KER_SAI34)      /* ck_ker_sai3, ck_ker_sai4 */ \
  CLKID(RCC_CK_KER_FDCAN)      /* ck_ker_fdcan */ \
  CLKID(RCC_CK_KER_DSIBLANE)   /* ck_ker_dsilane (DSI lanebyte or pixel clock) */ \
  CLKID(RCC_CK_KER_DSIPHY) \
  CLKID(RCC_CK_KER_CSI2)       /* ck_ker_csi2 (DCMIPP + CSI2HOST) */ \
  CLKID(RCC_CK_KER_CSI2TXESC)  /* [channel 30] */ \
  CLKID(RCC_CK_KER_CSI2PHY) \
  CLKID(RCC_CK_KER_LVDSPHY) \
  CLKID(RCC_CK_KER_STGEN) \
  CLKID(RCC_CK_KER_USB3PCIEPHY) \
  CLKID(RCC_CK_KER_UCPD1) \
  CLKID(RCC_CK_KER_I3C4) \
  CLKID(RCC_CK_KER_SPI8) \
  CLKID(RCC_CK_KER_I2C8) \
  CLKID(RCC_CK_KER_LPUART1) \
  CLKID(RCC_CK_KER_LPTIM3)     /* [channel 40] */ \
  CLKID(RCC_CK_KER_LPTIM45)    /* ck_ker_lptim4, ck_ker_lptim5 */ \
  CLKID(RCC_CK_KER_ADF1) \
  CLKID(RCC_CK_KER_TSDBG) \
  CLKID(RCC_CK_KER_TPIU) \
  CLKID(RCC_CK_SYS_ATB)        /* ck_sys_atb, ck_icn_m_etr */ \
  CLKID(RCC_CK_KER_ADC12) \
  CLKID(RCC_CK_KER_ADC3) \
  CLKID(RCC_CK_KER_OSPI1) \
  CLKID(RCC_CK_KER_OSPI2) \
  CLKID(RCC_CK_KER_FMC)        /* [channel 50] */ \
  CLKID(RCC_CK_KER_SDMMC1) \
  CLKID(RCC_CK_KER_SDMMC2) \
  CLKID(RCC_CK_KER_SDMMC3) \
  CLKID(RCC_CK_KER_ETH1_ETHSW) /* ck_ker_eth1, ck_ker_ethsw */ \
  CLKID(RCC_CK_KER_ETH2) \
  CLKID(RCC_CK_KER_ETHPTP)     /* ck_ker_eth1_ptp, ck_ker_eth2_ptp */ \
  CLKID(RCC_CK_KER_USB2PHY1) \
  CLKID(RCC_CK_KER_USB2PHY2) \
  CLKID(RCC_CK_ICN_M_GPU) \
  CLKID(RCC_CK_KER_ETHSWREF)   /* [channel 60] */ \
  CLKID(RCC_CK_MCO1)           /* Flexgen output. Not MCO1 pad */ \
  CLKID(RCC_CK_MCO2)           /* Flexgen output. Not MCO2 pad */ \
  CLKID(RCC_CK_CPU1EXT2F)      /* CA35 EXT2F clock */ \
  /* Internal bus clocks. \
     Managed with
     - HAL_RCC_SetClockCfg()
     - HAL_RCC_GetClockCfg() */ \
  CLKID(RCC_CK_ICN_APB1)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APB2)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APB3)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APB4)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APBDBG) /* Sourced from RCC_CK_LS_ICN_MCU */ \
  /* Misc. \
     Managed with
     - HAL_RCC_SetClockCfg()
     - HAL_RCC_GetClockCfg() */ \
  CLKID(RCC_CK_RTC)  /* ck_ker_rtc (RTC/AWU) */ \
  CLKID(RCC_CK_SPDIF) \
  CLKID(RCC_CK_I2S) \
  CLKID(RCC_CK_OBS0) /* Obs clock 0 from flexiclockgen */ \
  CLKID(RCC_CK_OBS1) /* Obs clock 1 from flexiclockgen */ \
  /* CLKID(RCC_CK_TEMP) : ck_ker_tmpsens ? */ \
  CLKID(RCC_CK_LAST) /* Always keep this last marker, AND LET NEXT LINE BLANK !!! */ \

#else /* ! defined(STM32MP2XX_ASSY3_1_0) */

#define ALL_CLOCK_IDS(CLKID) \
  CLKID(RCC_CK_NONE) \
  /* Oscillators: managed with \
     - HAL_RCC_SetOscCfg()
     - HAL_RCC_GetOscCfg() */ \
  CLKID(RCC_CK_LSI) /* ID 1 */ \
  CLKID(RCC_CK_LSE) \
  CLKID(RCC_CK_CSI) \
  CLKID(RCC_CK_HSE) \
  CLKID(RCC_CK_HSI) \
  CLKID(RCC_CK_CSI_KER) \
  CLKID(RCC_CK_HSE_KER) \
  CLKID(RCC_CK_HSI_KER) \
  /* PLL4 to 8 ref clocks, then 1 to 3 \
     Note: index order */ \
  CLKID(RCC_CK_PLL4REF) /* ID 9 = MUXSEL0 to GFG0/PLL4 */ \
  CLKID(RCC_CK_PLL5REF) /* MUXSEL1 to GFG1/PLL5 */ \
  CLKID(RCC_CK_PLL6REF) /* MUXSEL2 to GFG2/PLL6 */ \
  CLKID(RCC_CK_PLL7REF) /* MUXSEL3 to GFG3/PLL7 */ \
  CLKID(RCC_CK_PLL8REF) /* MUXSEL4 to GFG4/PLL8 */ \
  CLKID(RCC_CK_PLL1REF) /* MUXSEL5 to CA35 PLL */ \
  CLKID(RCC_CK_PLL2REF) /* MUXSEL6 to DDR PLL */ \
  CLKID(RCC_CK_PLL3REF) /* MUXSEL7 to GPU PLL */ \
  /* RCC external PLLs */ \
  CLKID(RCC_CK_PLL1) /* ID 17 = CA35 PLL output */ \
  CLKID(RCC_CK_PLL2) /* DDR PLL output */ \
  CLKID(RCC_CK_PLL3) /* GPU PLL output */ \
  /* Flexiclockgen GFGs/PLLs outputs in GFG number order */ \
  CLKID(RCC_CK_PLL4) /* ID 20 = GFG0 */ \
  CLKID(RCC_CK_PLL5) \
  CLKID(RCC_CK_PLL6) \
  CLKID(RCC_CK_PLL7) \
  CLKID(RCC_CK_PLL8) /* GFG4 */ \
  /* Flexiclockgen outputs (in channel order). \
     Managed with
     - HAL_RCC_SetClockCfg()
     - HAL_RCC_GetClockCfg() */ \
  CLKID(RCC_CK_ICN_MCU)     /* ID 25 [channel 0] ck_icn_hs_mcu, ck_icn_ls_mcu */ \
  CLKID(RCC_CK_ICN_SDMMC)   /* ck_icn_p_dlybsd1, ck_icn_p_dlybsd2, ck_icn_p_dlybsd3, \
                                 ck_icn_m_sdmmc1, ck_icn_m_sdmmc2, ck_icn_m_sdmmc3 */ \
  CLKID(RCC_CK_ICN_DDR)     /* ck_icn_s_ddr1, ck_icn_s_ddr2, ck_icn_m_cpu1 */ \
  CLKID(RCC_CK_ICN_DISPLAY) /* ck_icn_m_ltdc, ck_icn_m_dcmipp */ \
  CLKID(RCC_CK_ICN_HSL)     /* ck_icn_m_pcie, ck_icn_m_usb3drd, ck_icn_m_usb2ehci, \
                                 ck_icn_m_usb2ohci, ck_icn_m_eth1, ck_icn_m_eth2 */ \
  CLKID(RCC_CK_ICN_NIC)     /* ck_icn_s_bootrom, ck_icn_s_gic400 */ \
  CLKID(RCC_CK_ICN_VID)     /* ck_icn_m_vdec, ck_icn_m_venc */ \
  CLKID(RCC_CK_KER_LPTIM12)    /* ck_ker_lptim1, ck_ker_lptim2 */ \
  CLKID(RCC_CK_KER_USART24)    /* ck_ker_usart2, ck_ker_usart4 */ \
  CLKID(RCC_CK_KER_USART35)    /* ck_ker_usart3, ck_ker_usart5 */ \
  CLKID(RCC_CK_KER_SPI23)      /* [channel 10] ck_ker_spi2, ck_ker_spi3 */ \
  CLKID(RCC_CK_KER_SPDIFRX) \
  CLKID(RCC_CK_KER_I2C12I3C12) /* ck_ker_i2c1, ck_ker_i2c2, ck_ker_i3c1, ck_ker_i3c2 */ \
  CLKID(RCC_CK_KER_I2C35I3C3)  /* ck_ker_i2c3, ck_ker_i2c5, ck_ker_i3c3 */ \
  CLKID(RCC_CK_KER_I2C46)      /* ck_ker_i2c4, ck_ker_i2c6 */ \
  CLKID(RCC_CK_KER_I2C7) \
  CLKID(RCC_CK_KER_SPI1) \
  CLKID(RCC_CK_KER_SPI45)      /* ck_ker_spi4, ck_ker_spi5 */ \
  CLKID(RCC_CK_KER_SPI67)      /* ck_ker_spi6, ck_ker_spi7 */ \
  CLKID(RCC_CK_KER_USART1) \
  CLKID(RCC_CK_KER_USART6)     /* [channel 20] */ \
  CLKID(RCC_CK_KER_UART78)     /* ck_ker_uart7, ck_ker_uart8 */ \
  CLKID(RCC_CK_KER_UART9) \
  CLKID(RCC_CK_KER_SAI1MDF1)   /* ck_ker_sai1, ck_ker_mdf1 */ \
  CLKID(RCC_CK_KER_SAI2) \
  CLKID(RCC_CK_KER_SAI34)      /* ck_ker_sai3, ck_ker_sai4 */ \
  CLKID(RCC_CK_KER_FDCAN)      /* ck_ker_fdcan */ \
  CLKID(RCC_CK_KER_DSIBLANE)   /* ck_ker_dsilane (DSI lanebyte or pixel clock) */ \
  CLKID(RCC_CK_KER_DSIPHY) \
  CLKID(RCC_CK_KER_CSI2)       /* ck_ker_csi2 (DCMIPP + CSI2HOST) */ \
  CLKID(RCC_CK_KER_CSI2TXESC)  /* [channel 30] */ \
  CLKID(RCC_CK_KER_CSI2PHY) \
  CLKID(RCC_CK_KER_LVDSPHY) \
  CLKID(RCC_CK_KER_STGEN) \
  CLKID(RCC_CK_KER_USB3PCIEPHY) \
  CLKID(RCC_CK_KER_UCPD1) \
  CLKID(RCC_CK_KER_I3C4) \
  CLKID(RCC_CK_KER_SPI8) \
  CLKID(RCC_CK_KER_I2C8) \
  CLKID(RCC_CK_KER_LPUART1) \
  CLKID(RCC_CK_KER_LPTIM3)     /* [channel 40] */ \
  CLKID(RCC_CK_KER_LPTIM45)    /* ck_ker_lptim4, ck_ker_lptim5 */ \
  CLKID(RCC_CK_KER_MDF2) \
  CLKID(RCC_CK_KER_TSDBG) \
  CLKID(RCC_CK_KER_TPIU) \
  CLKID(RCC_CK_SYS_ATB)        /* ck_sys_atb, ck_icn_m_etr */ \
  CLKID(RCC_CK_KER_ADC12) \
  CLKID(RCC_CK_KER_ADC3) \
  CLKID(RCC_CK_KER_OSPI1) \
  CLKID(RCC_CK_KER_OSPI2) \
  CLKID(RCC_CK_KER_FMC)        /* [channel 50] */ \
  CLKID(RCC_CK_KER_SDMMC1) \
  CLKID(RCC_CK_KER_SDMMC2) \
  CLKID(RCC_CK_KER_SDMMC3) \
  CLKID(RCC_CK_KER_ETH1_ETHSW) /* ck_ker_eth1, ck_ker_ethsw */ \
  CLKID(RCC_CK_KER_ETH2) \
  CLKID(RCC_CK_KER_ETHPTP)     /* ck_ker_eth1_ptp, ck_ker_eth2_ptp */ \
  CLKID(RCC_CK_KER_USB2PHY1) \
  CLKID(RCC_CK_KER_USB2PHY2) \
  CLKID(RCC_CK_ICN_M_GPU) \
  CLKID(RCC_CK_KER_ETHSWREF)   /* [channel 60] */ \
  CLKID(RCC_CK_MCO1)           /* Flexgen output. Not MCO1 pad */ \
  CLKID(RCC_CK_MCO2)           /* Flexgen output. Not MCO2 pad */ \
  CLKID(RCC_CK_CPU1EXT2F)      /* CA35 EXT2F clock */ \
  /* Internal bus clocks. \
     Managed with
     - HAL_RCC_SetClockCfg()
     - HAL_RCC_GetClockCfg() */ \
  CLKID(RCC_CK_ICN_APB1)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APB2)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APB3)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APB4)   /* Sourced from RCC_CK_LS_ICN_MCU */ \
  CLKID(RCC_CK_ICN_APBDBG) /* Sourced from RCC_CK_LS_ICN_MCU */ \
  /* Misc. \
     Managed with
     - HAL_RCC_SetClockCfg()
     - HAL_RCC_GetClockCfg() */ \
  CLKID(RCC_CK_RTC)  /* ck_ker_rtc (RTC/AWU) */ \
  CLKID(RCC_CK_SPDIF) \
  CLKID(RCC_CK_I2S) \
  CLKID(RCC_CK_OBS0) /* Obs clock 0 from flexiclockgen */ \
  CLKID(RCC_CK_OBS1) /* Obs clock 1 from flexiclockgen */ \
  /* CLKID(RCC_CK_TEMP) : ck_ker_tmpsens ? */ \
  CLKID(RCC_CK_LAST) /* Always keep this last marker, AND LET NEXT LINE BLANK !!! */ \

#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */

#define GENERATE_ENUM(ENUM) ENUM,

typedef enum HAL_RCC_ClockIds
{
  ALL_CLOCK_IDS(GENERATE_ENUM)
} RCC_ClockIdTypeDef;

/* For application who needs clock names (strings), the following
   code can be used

   #define GENERATE_STRING(STRING) #STRING,
   static const char *HAL_RCC_ClockNames[] = {
       ALL_CLOCK_IDS(GENERATE_STRING)
   };
  */

#define HSE_TIMEOUT_VALUE          HSE_STARTUP_TIMEOUT
#define HSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define CSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define PLL_TIMEOUT_VALUE          ((uint32_t)2)    /* PLL max lock time : 2 ticks stands for real value in range [1ms;2ms] */
#define LSI_TIMEOUT_VALUE          ((uint32_t)1)    /* 3 LSI cycles = 3/32K */
#define RCC_LSE_TIMEOUT_VALUE      ((uint32_t)200)  /* 4096 LSE cycles = 4096/32K */
#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)1000) /* 1 s    */

enum
{
  RCC_OSCSTATE_UNDEF, // Undefined state
  RCC_OSCSTATE_OFF,
  RCC_OSCSTATE_ON,
  RCC_OSCSTATE_BYPASS, // ON + bypass
  RCC_OSCSTATE_DIGBYPASS, // ON + bypass + digital bypass
};

#define RCC_VALID_OSCSTATE(S) ((S == RCC_OSCSTATE_OFF) || (S == RCC_OSCSTATE_ON) || \
                               (S == RCC_OSCSTATE_BYPASS) || (S == RCC_OSCSTATE_DIGBYPASS))

#define RCC_OSCDRIVE_LOW        0 // !< LSE low drive
#define RCC_OSCDRIVE_MEDIUMLOW  1 // !< LSE medium low drive
#define RCC_OSCDRIVE_MEDIUMHIGH 2 // !< LSE medium high drive
#define RCC_OSCDRIVE_HIGH       3 // !< LSE highest drive

/* Oscillators config structure */
typedef struct
{
  uint16_t id; // Clock ID from @ref RCC_Clocks
  uint16_t state; //!< Oscillator state from @ref RCC_OSCSTATE_xxx
  uint32_t calibrationvalue;  /*!< The calibration trimming value.
                                      This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x3F */

  uint32_t divvalue; // Div value (for RCC_CK_CSI only)
  uint32_t drivevalue; // Drive value of @ref RCC_OSCDRIVE_xxx (for RCC_CK_LSE only)
} RCC_OscCfgTypeDef;

/** @brief  Macro to adjust the Internal High Speed oscillator (HSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal HSI RC.
  * @param  __HSICalibrationValue__: specifies the calibration trimming value
  *         HSITRIM. HSITRIM represents a signed value. HSITRIM field is added
  *         to the engineering Option Bytes loaded during reset phase
  *         (bsec_hsi_cal[11:0]) in order to form the calibration trimming value.
  *         HSICAL[11:0] = bsec_hsi_cal[11:0] + HSITRIM[6:0]
  *         This parameter must be a number between 0 and 0x7F:
  *         0x40: bsec_hsi_cal[11:0] - 64
  *         0x41: bsec_hsi_cal[11:0] - 63
  *         ...
  *         0x0: bsec_hsi_cal[11:0] (default after reset)
  *         ...
  *         0x3E: bsec_hsi_cal[11:0] + 62
  *         0x3F: bsec_hsi_cal[11:0] + 63
  */
#define __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(__HSICalibrationValue__) \
  MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, \
             (uint32_t)(__HSICalibrationValue__ & 0x7f) << RCC_HSICFGR_HSITRIM_Pos)

/** @brief  Macro Adjusts the Internal  oscillator (CSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal CSI RC.
  *         Please refer to the datasheet of the product for for more details on how to
  *         calibrate the CSI.
  * @param  __CSICalibrationValue__: specifies the calibration trimming value.
  *         This parameter must be a number between 0 and 0x1F.
  */
#define __HAL_RCC_CSI_CALIBRATIONVALUE_ADJUST(__CSICalibrationValue__) \
  MODIFY_REG(RCC->CSICFGR, RCC_CSICFGR_CSITRIM, \
             (uint32_t)(__CSICalibrationValue__) << RCC_CSICFGR_CSITRIM_Pos)

/* Possible PLL states (OR combination)
   Note: this coding is done so that value 0 means undefined.
   Note: 'bypass' can be selected whatever VCO enabled or not. */
#define RCC_PLLSTATE_ON     (1 << 0)    // PLL is ON
#define RCC_PLLSTATE_OFF    (1 << 1)    // PLL is OFF
#define RCC_PLLSTATE_BYPASS (1 << 2)    // VCO bypassed (clock coming from PLL input ref)

/* Possible PLL flags (OR combination)
 */
#define RCC_PLLFLAG_FRACTIONAL  (1 << 0) // flag[0]: 1=frac, 0=integer
#define RCC_PLLFLAG_DAC_ON      (1 << 1) // flag[1]=DACEN: 1=frac noise cancelling enabled, 0=disabled
#define RCC_PLLFLAG_SSCG_ON     (1 << 2) // flag[2]: 1=SSCG enabled, 0=disabled
#define RCC_PLLFLAG_SSCG_DOWN   (1 << 3) // flag[3]: 1=downspread, 0=center

/* PLLs config structure
   Suitable to handle PLLTS16FFCLAFRAC2 */
typedef struct
{
  uint16_t id;            // PLL output clock ID (RCC_CK_PLLx)
  uint16_t source;        // PLL ref clock ID
  uint8_t state;          // PLL state from @ref RCC_PLLSTATE_xxx
  uint8_t flags;          // OR combination of @ref RCC_PLLFLAG_xxx
  uint32_t frefdiv;       // FREFDIV[5:0]
  uint32_t fbdiv;         // FBDIV[11:0]
  uint32_t frac_in;       // FRACIN[23:0]
  uint32_t sscg_moddepth; // Modulation depth adjustment (SPREAD[4:0])
  uint32_t sscg_modfreq;  // Modulation frequency adjustment (DIVVAL[3:0])
  uint32_t postdiv1;      // FOUTPOSTDIV1 (from 1 to 7, 0=undef)
  uint32_t postdiv2;      // FOUTPOSTDIV2 (from 1 to 7, 0=undef)
} RCC_PLLCfgTypeDef;

enum
{
  RCC_CLKSTATE_UNDEF, // Undefined state
  RCC_CLKSTATE_OFF,
  RCC_CLKSTATE_ON
};

/* Standard clocks config structure (neither osc, nor PLL) */
typedef struct
{
  uint16_t id;        // Clock ID
  uint16_t source;    // Source clock ID
  uint16_t state;     // Clock state (RCC_CLKSTATE_xxx)
  uint32_t divvalue;  // Real divider value (0=undef)
} RCC_ClkCfgTypeDef;

typedef struct
{
  uint16_t id;        // Mux ID
  uint16_t source;    // Mux input selection
} RCC_MuxCfgTypeDef;

HAL_StatusTypeDef       HAL_RCC_GetOscCfg(RCC_OscCfgTypeDef *osc);
HAL_StatusTypeDef       HAL_RCC_SetOscCfg(RCC_OscCfgTypeDef *osc);

HAL_StatusTypeDef       HAL_RCC_GetPllCfg(RCC_PLLCfgTypeDef *pll);
HAL_StatusTypeDef       HAL_RCC_SetPllCfg(RCC_PLLCfgTypeDef *pll);

HAL_StatusTypeDef       HAL_RCC_GetClockCfg(RCC_ClkCfgTypeDef *clk);
HAL_StatusTypeDef       HAL_RCC_SetClockCfg(RCC_ClkCfgTypeDef *clk);

HAL_StatusTypeDef       HAL_RCC_SetCA35SSCfg(RCC_MuxCfgTypeDef *mux);
HAL_StatusTypeDef       HAL_RCC_GetCA35SSCfg(RCC_MuxCfgTypeDef *mux);

HAL_StatusTypeDef       HAL_RCC_SetBusClockCfg(RCC_ClkCfgTypeDef *clk);
HAL_StatusTypeDef       HAL_RCC_GetBusClockCfg(RCC_ClkCfgTypeDef *clk);

HAL_StatusTypeDef       HAL_RCC_SetPeriphClockCfg(RCC_ClkCfgTypeDef *clk);
HAL_StatusTypeDef       HAL_RCC_GetPeriphClockCfg(RCC_ClkCfgTypeDef *clk);

void                    HAL_RCC_EnableHSECSS(void);
void                    HAL_RCC_EnableLSECSS(void);

uint32_t                HAL_RCC_GetFreq(uint32_t clk_id); // Get clock freq
uint32_t                HAL_RCC_GetFreqM(uint32_t clk_id); // Get clock measured freq
uint16_t                HAL_RCC_GetSource(uint32_t clk_id); // Get clock source ID
int                     HAL_RCC_IsEnabled(uint32_t clk_id); // Returns 1 if enabled, 0 else
int                     HAL_RCC_IsRunning(uint32_t clk_id); // Returns 1 if running, 0 else
int                     HAL_RCC_IsReady(uint32_t clk_id); // Returns 1 if ready (OSC or PLL cases), 0 else
HAL_StatusTypeDef       HAL_RCC_Xable(uint32_t clk_id, int enable); // Enable or disable clock

uint32_t                HAL_RCC_GetBusClockFreq(uint32_t clk_id);
uint32_t                HAL_RCC_GetPeriphClockFreq(uint32_t clk_id);
uint32_t                HAL_RCC_GetPllClockFreq(uint32_t clk_id);
uint32_t                HAL_RCC_GetOscClockFreq(uint32_t clk_id);
uint32_t                HAL_RCC_GetCA35SSFreq(void);
uint32_t                HAL_RCC_GetCM33SSFreq(void);
uint32_t                HAL_RCC_GetDDRFreq(void);
uint32_t                HAL_RCC_GetGPUFreq(void);

uint32_t                HAL_RCC_MeasureClockFreq(uint32_t clk_id);

/* Reroute 'src1_id' child clocks to use 'src2_id' instead.
   Returns: 0=OK, -1=ERROR */
int                     HAL_RCC_RerouteChilds(uint16_t src1_id, uint16_t src2_id);

void                    HAL_RCC_EnableCSS(uint16_t clk_id);

/* Clock enable macros */
#define __HAL_RCC_VDERAM_CLK_ENABLE() SET_BIT(RCC->VDERAMCFGR, RCC_VDERAMCFGR_VDERAMEN)
#define __HAL_RCC_SERC_CLK_ENABLE() SET_BIT(RCC->SERCCFGR, RCC_SERCCFGR_SERCEN)
#define __HAL_RCC_OSPIIOM_CLK_ENABLE() SET_BIT(RCC->OSPIIOMCFGR, RCC_OSPIIOMCFGR_OSPIIOMEN)
#define __HAL_RCC_ETHSW_CLK_ENABLE() SET_BIT(RCC->ETHSWCFGR, RCC_ETHSWCFGR_ETHSWMACEN|\
                                             RCC_ETHSWCFGR_ETHSWEN|RCC_ETHSWCFGR_ETHSWREFEN)
#define __HAL_RCC_VDEC_CLK_ENABLE() SET_BIT(RCC->VDECCFGR, RCC_VDECCFGR_VDECEN)
#define __HAL_RCC_VENC_CLK_ENABLE() SET_BIT(RCC->VENCCFGR, RCC_VENCCFGR_VENCEN)
#define __HAL_RCC_C3_CLK_ENABLE() SET_BIT(RCC->C3CFGR, RCC_C3CFGR_C3EN)
#define __HAL_RCC_SYSRAM_CLK_ENABLE() SET_BIT(RCC->SYSRAMCFGR, RCC_SYSRAMCFGR_SYSRAMEN)
#define __HAL_RCC_SRAM1_CLK_ENABLE() SET_BIT(RCC->SRAM1CFGR, RCC_SRAM1CFGR_SRAM1EN)
#define __HAL_RCC_SRAM2_CLK_ENABLE() SET_BIT(RCC->SRAM2CFGR, RCC_SRAM2CFGR_SRAM2EN)
#define __HAL_RCC_RETRAM_CLK_ENABLE() SET_BIT(RCC->RETRAMCFGR, RCC_RETRAMCFGR_RETRAMEN)
#define __HAL_RCC_BKPSRAM_CLK_ENABLE() SET_BIT(RCC->BKPSRAMCFGR, RCC_BKPSRAMCFGR_BKPSRAMEN)
#define __HAL_RCC_LPSRAM1_CLK_ENABLE() SET_BIT(RCC->LPSRAM1CFGR, RCC_LPSRAM1CFGR_LPSRAM1EN)
#define __HAL_RCC_LPSRAM2_CLK_ENABLE() SET_BIT(RCC->LPSRAM2CFGR, RCC_LPSRAM2CFGR_LPSRAM2EN)
#define __HAL_RCC_LPSRAM3_CLK_ENABLE() SET_BIT(RCC->LPSRAM3CFGR, RCC_LPSRAM3CFGR_LPSRAM3EN)
#define __HAL_RCC_OSPI1_CLK_ENABLE() SET_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OSPI1EN)
#define __HAL_RCC_OSPI2_CLK_ENABLE() SET_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OSPI2EN)
#define __HAL_RCC_STM500_CLK_ENABLE() SET_BIT(RCC->STM500CFGR, RCC_STM500CFGR_STM500EN)
#define __HAL_RCC_ETR_CLK_ENABLE() SET_BIT(RCC->ETRCFGR, RCC_ETRCFGR_ETREN)
#define __HAL_RCC_GPIOA_CLK_ENABLE() SET_BIT(RCC->GPIOACFGR, RCC_GPIOACFGR_GPIOxEN)
#define __HAL_RCC_GPIOB_CLK_ENABLE() SET_BIT(RCC->GPIOBCFGR, RCC_GPIOBCFGR_GPIOxEN)
#define __HAL_RCC_GPIOC_CLK_ENABLE() SET_BIT(RCC->GPIOCCFGR, RCC_GPIOCCFGR_GPIOxEN)
#define __HAL_RCC_GPIOD_CLK_ENABLE() SET_BIT(RCC->GPIODCFGR, RCC_GPIODCFGR_GPIOxEN)
#define __HAL_RCC_GPIOE_CLK_ENABLE() SET_BIT(RCC->GPIOECFGR, RCC_GPIOECFGR_GPIOxEN)
#define __HAL_RCC_GPIOF_CLK_ENABLE() SET_BIT(RCC->GPIOFCFGR, RCC_GPIOFCFGR_GPIOxEN)
#define __HAL_RCC_GPIOG_CLK_ENABLE() SET_BIT(RCC->GPIOGCFGR, RCC_GPIOGCFGR_GPIOxEN)
#define __HAL_RCC_GPIOH_CLK_ENABLE() SET_BIT(RCC->GPIOHCFGR, RCC_GPIOHCFGR_GPIOxEN)
#define __HAL_RCC_GPIOI_CLK_ENABLE() SET_BIT(RCC->GPIOICFGR, RCC_GPIOICFGR_GPIOxEN)
#define __HAL_RCC_GPIOJ_CLK_ENABLE() SET_BIT(RCC->GPIOJCFGR, RCC_GPIOJCFGR_GPIOxEN)
#define __HAL_RCC_GPIOK_CLK_ENABLE() SET_BIT(RCC->GPIOKCFGR, RCC_GPIOKCFGR_GPIOxEN)
#define __HAL_RCC_GPIOZ_CLK_ENABLE() SET_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZEN)
#define __HAL_RCC_HPDMA1_CLK_ENABLE() SET_BIT(RCC->HPDMA1CFGR, RCC_HPDMA1CFGR_HPDMA1EN)
#define __HAL_RCC_HPDMA2_CLK_ENABLE() SET_BIT(RCC->HPDMA2CFGR, RCC_HPDMA2CFGR_HPDMA2EN)
#define __HAL_RCC_HPDMA3_CLK_ENABLE() SET_BIT(RCC->HPDMA3CFGR, RCC_HPDMA3CFGR_HPDMA3EN)
#define __HAL_RCC_LPDMA_CLK_ENABLE() SET_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMAEN)
#define __HAL_RCC_HSEM_CLK_ENABLE() SET_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMEN)
#define __HAL_RCC_IPCC1_CLK_ENABLE() SET_BIT(RCC->IPCC1CFGR, RCC_IPCC1CFGR_IPCC1EN)
#define __HAL_RCC_IPCC2_CLK_ENABLE() SET_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2EN)
#define __HAL_RCC_RTC_CLK_ENABLE() SET_BIT(RCC->RTCCFGR, RCC_RTCCFGR_RTCEN)
#define __HAL_RCC_SYSCPU1_CLK_ENABLE() SET_BIT(RCC->SYSCPU1CFGR, RCC_SYSCPU1CFGR_SYSCPU1EN)
#define __HAL_RCC_IS2M_CLK_ENABLE() SET_BIT(RCC->IS2MCFGR, RCC_IS2MCFGR_IS2MEN)
#define __HAL_RCC_TIM1_CLK_ENABLE() SET_BIT(RCC->TIM1CFGR, RCC_TIM1CFGR_TIM1EN)
#define __HAL_RCC_TIM2_CLK_ENABLE() SET_BIT(RCC->TIM2CFGR, RCC_TIM2CFGR_TIM2EN)
#define __HAL_RCC_TIM3_CLK_ENABLE() SET_BIT(RCC->TIM3CFGR, RCC_TIM3CFGR_TIM3EN)
#define __HAL_RCC_TIM4_CLK_ENABLE() SET_BIT(RCC->TIM4CFGR, RCC_TIM4CFGR_TIM4EN)
#define __HAL_RCC_TIM5_CLK_ENABLE() SET_BIT(RCC->TIM5CFGR, RCC_TIM5CFGR_TIM5EN)
#define __HAL_RCC_TIM6_CLK_ENABLE() SET_BIT(RCC->TIM6CFGR, RCC_TIM6CFGR_TIM6EN)
#define __HAL_RCC_TIM7_CLK_ENABLE() SET_BIT(RCC->TIM7CFGR, RCC_TIM7CFGR_TIM7EN)
#define __HAL_RCC_TIM8_CLK_ENABLE() SET_BIT(RCC->TIM8CFGR, RCC_TIM8CFGR_TIM8EN)
#define __HAL_RCC_TIM10_CLK_ENABLE() SET_BIT(RCC->TIM10CFGR, RCC_TIM10CFGR_TIM10EN)
#define __HAL_RCC_TIM11_CLK_ENABLE() SET_BIT(RCC->TIM11CFGR, RCC_TIM11CFGR_TIM11EN)
#define __HAL_RCC_TIM12_CLK_ENABLE() SET_BIT(RCC->TIM12CFGR, RCC_TIM12CFGR_TIM12EN)
#define __HAL_RCC_TIM13_CLK_ENABLE() SET_BIT(RCC->TIM13CFGR, RCC_TIM13CFGR_TIM13EN)
#define __HAL_RCC_TIM14_CLK_ENABLE() SET_BIT(RCC->TIM14CFGR, RCC_TIM14CFGR_TIM14EN)
#define __HAL_RCC_TIM15_CLK_ENABLE() SET_BIT(RCC->TIM15CFGR, RCC_TIM15CFGR_TIM15EN)
#define __HAL_RCC_TIM16_CLK_ENABLE() SET_BIT(RCC->TIM16CFGR, RCC_TIM16CFGR_TIM16EN)
#define __HAL_RCC_TIM17_CLK_ENABLE() SET_BIT(RCC->TIM17CFGR, RCC_TIM17CFGR_TIM17EN)
#define __HAL_RCC_TIM20_CLK_ENABLE() SET_BIT(RCC->TIM20CFGR, RCC_TIM20CFGR_TIM20EN)
#define __HAL_RCC_LPTIM1_CLK_ENABLE() SET_BIT(RCC->LPTIM1CFGR, RCC_LPTIM1CFGR_LPTIM1EN)
#define __HAL_RCC_LPTIM2_CLK_ENABLE() SET_BIT(RCC->LPTIM2CFGR, RCC_LPTIM2CFGR_LPTIM2EN)
#define __HAL_RCC_LPTIM3_CLK_ENABLE() SET_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3EN)
#define __HAL_RCC_LPTIM4_CLK_ENABLE() SET_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4EN)
#define __HAL_RCC_LPTIM5_CLK_ENABLE() SET_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5EN)
#define __HAL_RCC_SPI1_CLK_ENABLE() SET_BIT(RCC->SPI1CFGR, RCC_SPI1CFGR_SPI1EN)
#define __HAL_RCC_SPI2_CLK_ENABLE() SET_BIT(RCC->SPI2CFGR, RCC_SPI2CFGR_SPI2EN)
#define __HAL_RCC_SPI3_CLK_ENABLE() SET_BIT(RCC->SPI3CFGR, RCC_SPI3CFGR_SPI3EN)
#define __HAL_RCC_SPI4_CLK_ENABLE() SET_BIT(RCC->SPI4CFGR, RCC_SPI4CFGR_SPI4EN)
#define __HAL_RCC_SPI5_CLK_ENABLE() SET_BIT(RCC->SPI5CFGR, RCC_SPI5CFGR_SPI5EN)
#define __HAL_RCC_SPI6_CLK_ENABLE() SET_BIT(RCC->SPI6CFGR, RCC_SPI6CFGR_SPI6EN)
#define __HAL_RCC_SPI7_CLK_ENABLE() SET_BIT(RCC->SPI7CFGR, RCC_SPI7CFGR_SPI7EN)
#define __HAL_RCC_SPI8_CLK_ENABLE() SET_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8EN)
#define __HAL_RCC_SPDIFRX_CLK_ENABLE() SET_BIT(RCC->SPDIFRXCFGR, RCC_SPDIFRXCFGR_SPDIFRXEN)
#define __HAL_RCC_USART1_CLK_ENABLE() SET_BIT(RCC->USART1CFGR, RCC_USART1CFGR_USART1EN)
#define __HAL_RCC_USART2_CLK_ENABLE() SET_BIT(RCC->USART2CFGR, RCC_USART2CFGR_USART2EN)
#define __HAL_RCC_USART3_CLK_ENABLE() SET_BIT(RCC->USART3CFGR, RCC_USART3CFGR_USART3EN)
#define __HAL_RCC_UART4_CLK_ENABLE() SET_BIT(RCC->UART4CFGR, RCC_UART4CFGR_UART4EN)
#define __HAL_RCC_UART5_CLK_ENABLE() SET_BIT(RCC->UART5CFGR, RCC_UART5CFGR_UART5EN)
#define __HAL_RCC_USART6_CLK_ENABLE() SET_BIT(RCC->USART6CFGR, RCC_USART6CFGR_USART6EN)
#define __HAL_RCC_UART7_CLK_ENABLE() SET_BIT(RCC->UART7CFGR, RCC_UART7CFGR_UART7EN)
#define __HAL_RCC_UART8_CLK_ENABLE() SET_BIT(RCC->UART8CFGR, RCC_UART8CFGR_UART8EN)
#define __HAL_RCC_UART9_CLK_ENABLE() SET_BIT(RCC->UART9CFGR, RCC_UART9CFGR_UART9EN)
#define __HAL_RCC_LPUART1_CLK_ENABLE() SET_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1EN)
#define __HAL_RCC_I2C1_CLK_ENABLE() SET_BIT(RCC->I2C1CFGR, RCC_I2C1CFGR_I2C1EN)
#define __HAL_RCC_I2C2_CLK_ENABLE() SET_BIT(RCC->I2C2CFGR, RCC_I2C2CFGR_I2C2EN)
#define __HAL_RCC_I2C3_CLK_ENABLE() SET_BIT(RCC->I2C3CFGR, RCC_I2C3CFGR_I2C3EN)
#define __HAL_RCC_I2C4_CLK_ENABLE() SET_BIT(RCC->I2C4CFGR, RCC_I2C4CFGR_I2C4EN)
#define __HAL_RCC_I2C5_CLK_ENABLE() SET_BIT(RCC->I2C5CFGR, RCC_I2C5CFGR_I2C5EN)
#define __HAL_RCC_I2C6_CLK_ENABLE() SET_BIT(RCC->I2C6CFGR, RCC_I2C6CFGR_I2C6EN)
#define __HAL_RCC_I2C7_CLK_ENABLE() SET_BIT(RCC->I2C7CFGR, RCC_I2C7CFGR_I2C7EN)
#define __HAL_RCC_I2C8_CLK_ENABLE() SET_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8EN)
#define __HAL_RCC_SAI1_CLK_ENABLE() SET_BIT(RCC->SAI1CFGR, RCC_SAI1CFGR_SAI1EN)
#define __HAL_RCC_SAI2_CLK_ENABLE() SET_BIT(RCC->SAI2CFGR, RCC_SAI2CFGR_SAI2EN)
#define __HAL_RCC_SAI3_CLK_ENABLE() SET_BIT(RCC->SAI3CFGR, RCC_SAI3CFGR_SAI3EN)
#define __HAL_RCC_SAI4_CLK_ENABLE() SET_BIT(RCC->SAI4CFGR, RCC_SAI4CFGR_SAI4EN)
#define __HAL_RCC_MDF1_CLK_ENABLE() SET_BIT(RCC->MDF1CFGR, RCC_MDF1CFGR_MDF1EN)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_CLK_ENABLE() SET_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1EN)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_CLK_ENABLE() SET_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2EN)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_FDCAN_CLK_ENABLE() SET_BIT(RCC->FDCANCFGR, RCC_FDCANCFGR_FDCANEN)
#define __HAL_RCC_HDP_CLK_ENABLE() SET_BIT(RCC->HDPCFGR, RCC_HDPCFGR_HDPEN)
#define __HAL_RCC_ADC12_CLK_ENABLE() SET_BIT(RCC->ADC12CFGR, RCC_ADC12CFGR_ADC12EN)
#define __HAL_RCC_ADC3_CLK_ENABLE() SET_BIT(RCC->ADC3CFGR, RCC_ADC3CFGR_ADC3EN)
#define __HAL_RCC_ETH1_CLK_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1EN)
#define __HAL_RCC_ETH1MAC_CLK_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1MACEN)
#define __HAL_RCC_ETH1TX_CLK_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1TXEN)
#define __HAL_RCC_ETH1RX_CLK_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1RXEN)
#define __HAL_RCC_ETH2_CLK_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2EN)
#define __HAL_RCC_ETH2MAC_CLK_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2MACEN)
#define __HAL_RCC_ETH2TX_CLK_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2TXEN)
#define __HAL_RCC_ETH2RX_CLK_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2RXEN)
#define __HAL_RCC_USB2_CLK_ENABLE() SET_BIT(RCC->USB2CFGR, RCC_USB2CFGR_USB2EN)
#define __HAL_RCC_USB2PHY1_CLK_ENABLE() SET_BIT(RCC->USB2PHY1CFGR, RCC_USB2PHY1CFGR_USB2PHY1EN)
#define __HAL_RCC_USB2PHY2_CLK_ENABLE() SET_BIT(RCC->USB2PHY2CFGR, RCC_USB2PHY2CFGR_USB2PHY2EN)
#define __HAL_RCC_USB3DRD_CLK_ENABLE() SET_BIT(RCC->USB3DRDCFGR, RCC_USB3DRDCFGR_USB3DRDEN)
#define __HAL_RCC_USB3PCIEPHY_CLK_ENABLE() SET_BIT(RCC->USB3PCIEPHYCFGR, RCC_USB3PCIEPHYCFGR_USB3PCIEPHYEN)
#define __HAL_RCC_PCIE_CLK_ENABLE() SET_BIT(RCC->PCIECFGR, RCC_PCIECFGR_PCIEEN)
#define __HAL_RCC_UCPD1_CLK_ENABLE() SET_BIT(RCC->UCPD1CFGR, RCC_UCPD1CFGR_UCPD1EN)
#define __HAL_RCC_FMC_CLK_ENABLE() SET_BIT(RCC->FMCCFGR, RCC_FMCCFGR_FMCEN)
#define __HAL_RCC_SDMMC1_CLK_ENABLE() SET_BIT(RCC->SDMMC1CFGR, RCC_SDMMC1CFGR_SDMMC1EN)
#define __HAL_RCC_SDMMC2_CLK_ENABLE() SET_BIT(RCC->SDMMC2CFGR, RCC_SDMMC2CFGR_SDMMC2EN)
#define __HAL_RCC_SDMMC3_CLK_ENABLE() SET_BIT(RCC->SDMMC3CFGR, RCC_SDMMC3CFGR_SDMMC3EN)
#define __HAL_RCC_GPU_CLK_ENABLE() SET_BIT(RCC->GPUCFGR, RCC_GPUCFGR_GPUEN)
#define __HAL_RCC_LTDC_CLK_ENABLE() SET_BIT(RCC->LTDCCFGR, RCC_LTDCCFGR_LTDCEN)
#define __HAL_RCC_DSI_CLK_ENABLE() SET_BIT(RCC->DSICFGR, RCC_DSICFGR_DSIEN)
#define __HAL_RCC_LVDS_CLK_ENABLE() SET_BIT(RCC->LVDSCFGR, RCC_LVDSCFGR_LVDSEN)
#define __HAL_RCC_CSI2_CLK_ENABLE() SET_BIT(RCC->CSI2CFGR, RCC_CSI2CFGR_CSI2EN)
#define __HAL_RCC_DCMIPP_CLK_ENABLE() SET_BIT(RCC->DCMIPPCFGR, RCC_DCMIPPCFGR_DCMIPPEN)
#define __HAL_RCC_CCI_CLK_ENABLE() SET_BIT(RCC->CCICFGR, RCC_CCICFGR_CCIEN)
#define __HAL_RCC_RNG_CLK_ENABLE() SET_BIT(RCC->RNGCFGR, RCC_RNGCFGR_RNGEN)
#define __HAL_RCC_PKA_CLK_ENABLE() SET_BIT(RCC->PKACFGR, RCC_PKACFGR_PKAEN)
#define __HAL_RCC_SAES_CLK_ENABLE() SET_BIT(RCC->SAESCFGR, RCC_SAESCFGR_SAESEN)
#define __HAL_RCC_HASH_CLK_ENABLE() SET_BIT(RCC->HASHCFGR, RCC_HASHCFGR_HASHEN)
#define __HAL_RCC_CRYP1_CLK_ENABLE() SET_BIT(RCC->CRYP1CFGR, RCC_CRYP1CFGR_CRYP1EN)
#define __HAL_RCC_CRYP2_CLK_ENABLE() SET_BIT(RCC->CRYP2CFGR, RCC_CRYP2CFGR_CRYP2EN)
#define __HAL_RCC_IWDG1_CLK_ENABLE() SET_BIT(RCC->IWDG1CFGR, RCC_IWDG1CFGR_IWDG1EN)
#define __HAL_RCC_IWDG2_CLK_ENABLE() SET_BIT(RCC->IWDG2CFGR, RCC_IWDG2CFGR_IWDG2EN)
#define __HAL_RCC_IWDG3_CLK_ENABLE() SET_BIT(RCC->IWDG3CFGR, RCC_IWDG3CFGR_IWDG3EN)
#define __HAL_RCC_IWDG4_CLK_ENABLE() SET_BIT(RCC->IWDG4CFGR, RCC_IWDG4CFGR_IWDG4EN)
#define __HAL_RCC_IWDG5_CLK_ENABLE() SET_BIT(RCC->IWDG5CFGR, RCC_IWDG5CFGR_IWDG5EN)
#define __HAL_RCC_WWDG1_CLK_ENABLE() SET_BIT(RCC->WWDG1CFGR, RCC_WWDG1CFGR_WWDG1EN)
#define __HAL_RCC_WWDG2_CLK_ENABLE() SET_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2EN)
#define __HAL_RCC_BUSPERFM_CLK_ENABLE() SET_BIT(RCC->BUSPERFMCFGR, RCC_BUSPERFMCFGR_BUSPERFMEN)
#define __HAL_RCC_VREF_CLK_ENABLE() SET_BIT(RCC->VREFCFGR, RCC_VREFCFGR_VREFEN)
#define __HAL_RCC_TMPSENS_CLK_ENABLE() SET_BIT(RCC->TMPSENSCFGR, RCC_TMPSENSCFGR_TMPSENSEN)
#define __HAL_RCC_CRC_CLK_ENABLE() SET_BIT(RCC->CRCCFGR, RCC_CRCCFGR_CRCEN)
#define __HAL_RCC_BSEC_CLK_ENABLE() SET_BIT(RCC->BSECCFGR, RCC_BSECCFGR_BSECEN)
#define __HAL_RCC_GICV2M_CLK_ENABLE() SET_BIT(RCC->GICV2MCFGR, RCC_GICV2MCFGR_GICV2MEN)
#define __HAL_RCC_I3C1_CLK_ENABLE() SET_BIT(RCC->I3C1CFGR, RCC_I3C1CFGR_I3C1EN)
#define __HAL_RCC_I3C2_CLK_ENABLE() SET_BIT(RCC->I3C2CFGR, RCC_I3C2CFGR_I3C2EN)
#define __HAL_RCC_I3C3_CLK_ENABLE() SET_BIT(RCC->I3C3CFGR, RCC_I3C3CFGR_I3C3EN)
#define __HAL_RCC_I3C4_CLK_ENABLE() SET_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4EN)

/* Clock disable macros */
#define __HAL_RCC_VDERAM_CLK_DISABLE() CLEAR_BIT(RCC->VDERAMCFGR, RCC_VDERAMCFGR_VDERAMEN)
#define __HAL_RCC_SERC_CLK_DISABLE() CLEAR_BIT(RCC->SERCCFGR, RCC_SERCCFGR_SERCEN)
#define __HAL_RCC_OSPIIOM_CLK_DISABLE() CLEAR_BIT(RCC->OSPIIOMCFGR, RCC_OSPIIOMCFGR_OSPIIOMEN)
#define __HAL_RCC_ETHSW_CLK_DISABLE() CLEAR_BIT(RCC->ETHSWCFGR, RCC_ETHSWCFGR_ETHSWMACEN|\
                                                RCC_ETHSWCFGR_ETHSWEN|RCC_ETHSWCFGR_ETHSWREFEN)
#define __HAL_RCC_VDEC_CLK_DISABLE() CLEAR_BIT(RCC->VDECCFGR, RCC_VDECCFGR_VDECEN)
#define __HAL_RCC_VENC_CLK_DISABLE() CLEAR_BIT(RCC->VENCCFGR, RCC_VENCCFGR_VENCEN)
#define __HAL_RCC_C3_CLK_DISABLE() CLEAR_BIT(RCC->C3CFGR, RCC_C3CFGR_C3EN)
#define __HAL_RCC_SYSRAM_CLK_DISABLE() CLEAR_BIT(RCC->SYSRAMCFGR, RCC_SYSRAMCFGR_SYSRAMEN)
#define __HAL_RCC_SRAM1_CLK_DISABLE() CLEAR_BIT(RCC->SRAM1CFGR, RCC_SRAM1CFGR_SRAM1EN)
#define __HAL_RCC_SRAM2_CLK_DISABLE() CLEAR_BIT(RCC->SRAM2CFGR, RCC_SRAM2CFGR_SRAM2EN)
#define __HAL_RCC_RETRAM_CLK_DISABLE() CLEAR_BIT(RCC->RETRAMCFGR, RCC_RETRAMCFGR_RETRAMEN)
#define __HAL_RCC_BKPSRAM_CLK_DISABLE() CLEAR_BIT(RCC->BKPSRAMCFGR, RCC_BKPSRAMCFGR_BKPSRAMEN)
#define __HAL_RCC_LPSRAM1_CLK_DISABLE() CLEAR_BIT(RCC->LPSRAM1CFGR, RCC_LPSRAM1CFGR_LPSRAM1EN)
#define __HAL_RCC_LPSRAM2_CLK_DISABLE() CLEAR_BIT(RCC->LPSRAM2CFGR, RCC_LPSRAM2CFGR_LPSRAM2EN)
#define __HAL_RCC_LPSRAM3_CLK_DISABLE() CLEAR_BIT(RCC->LPSRAM3CFGR, RCC_LPSRAM3CFGR_LPSRAM3EN)
#define __HAL_RCC_OSPI1_CLK_DISABLE() CLEAR_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OSPI1EN)
#define __HAL_RCC_OSPI2_CLK_DISABLE() CLEAR_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OSPI2EN)
#define __HAL_RCC_STM500_CLK_DISABLE() CLEAR_BIT(RCC->STM500CFGR, RCC_STM500CFGR_STM500EN)
#define __HAL_RCC_ETR_CLK_DISABLE() CLEAR_BIT(RCC->ETRCFGR, RCC_ETRCFGR_ETREN)
#define __HAL_RCC_GPIOA_CLK_DISABLE() CLEAR_BIT(RCC->GPIOACFGR, RCC_GPIOACFGR_GPIOxEN)
#define __HAL_RCC_GPIOB_CLK_DISABLE() CLEAR_BIT(RCC->GPIOBCFGR, RCC_GPIOBCFGR_GPIOxEN)
#define __HAL_RCC_GPIOC_CLK_DISABLE() CLEAR_BIT(RCC->GPIOCCFGR, RCC_GPIOCCFGR_GPIOxEN)
#define __HAL_RCC_GPIOD_CLK_DISABLE() CLEAR_BIT(RCC->GPIODCFGR, RCC_GPIODCFGR_GPIOxEN)
#define __HAL_RCC_GPIOE_CLK_DISABLE() CLEAR_BIT(RCC->GPIOECFGR, RCC_GPIOECFGR_GPIOxEN)
#define __HAL_RCC_GPIOF_CLK_DISABLE() CLEAR_BIT(RCC->GPIOFCFGR, RCC_GPIOFCFGR_GPIOxEN)
#define __HAL_RCC_GPIOG_CLK_DISABLE() CLEAR_BIT(RCC->GPIOGCFGR, RCC_GPIOGCFGR_GPIOxEN)
#define __HAL_RCC_GPIOH_CLK_DISABLE() CLEAR_BIT(RCC->GPIOHCFGR, RCC_GPIOHCFGR_GPIOxEN)
#define __HAL_RCC_GPIOI_CLK_DISABLE() CLEAR_BIT(RCC->GPIOICFGR, RCC_GPIOICFGR_GPIOxEN)
#define __HAL_RCC_GPIOJ_CLK_DISABLE() CLEAR_BIT(RCC->GPIOJCFGR, RCC_GPIOJCFGR_GPIOxEN)
#define __HAL_RCC_GPIOK_CLK_DISABLE() CLEAR_BIT(RCC->GPIOKCFGR, RCC_GPIOKCFGR_GPIOxEN)
#define __HAL_RCC_GPIOZ_CLK_DISABLE() CLEAR_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZEN)
#define __HAL_RCC_HPDMA1_CLK_DISABLE() CLEAR_BIT(RCC->HPDMA1CFGR, RCC_HPDMA1CFGR_HPDMA1EN)
#define __HAL_RCC_HPDMA2_CLK_DISABLE() CLEAR_BIT(RCC->HPDMA2CFGR, RCC_HPDMA2CFGR_HPDMA2EN)
#define __HAL_RCC_HPDMA3_CLK_DISABLE() CLEAR_BIT(RCC->HPDMA3CFGR, RCC_HPDMA3CFGR_HPDMA3EN)
#define __HAL_RCC_LPDMA_CLK_DISABLE() CLEAR_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMAEN)
#define __HAL_RCC_HSEM_CLK_DISABLE() CLEAR_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMEN)
#define __HAL_RCC_IPCC1_CLK_DISABLE() CLEAR_BIT(RCC->IPCC1CFGR, RCC_IPCC1CFGR_IPCC1EN)
#define __HAL_RCC_IPCC2_CLK_DISABLE() CLEAR_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2EN)
#define __HAL_RCC_RTC_CLK_DISABLE() CLEAR_BIT(RCC->RTCCFGR, RCC_RTCCFGR_RTCEN)
#define __HAL_RCC_SYSCPU1_CLK_DISABLE() CLEAR_BIT(RCC->SYSCPU1CFGR, RCC_SYSCPU1CFGR_SYSCPU1EN)
#define __HAL_RCC_IS2M_CLK_DISABLE() CLEAR_BIT(RCC->IS2MCFGR, RCC_IS2MCFGR_IS2MEN)
#define __HAL_RCC_TIM1_CLK_DISABLE() CLEAR_BIT(RCC->TIM1CFGR, RCC_TIM1CFGR_TIM1EN)
#define __HAL_RCC_TIM2_CLK_DISABLE() CLEAR_BIT(RCC->TIM2CFGR, RCC_TIM2CFGR_TIM2EN)
#define __HAL_RCC_TIM3_CLK_DISABLE() CLEAR_BIT(RCC->TIM3CFGR, RCC_TIM3CFGR_TIM3EN)
#define __HAL_RCC_TIM4_CLK_DISABLE() CLEAR_BIT(RCC->TIM4CFGR, RCC_TIM4CFGR_TIM4EN)
#define __HAL_RCC_TIM5_CLK_DISABLE() CLEAR_BIT(RCC->TIM5CFGR, RCC_TIM5CFGR_TIM5EN)
#define __HAL_RCC_TIM6_CLK_DISABLE() CLEAR_BIT(RCC->TIM6CFGR, RCC_TIM6CFGR_TIM6EN)
#define __HAL_RCC_TIM7_CLK_DISABLE() CLEAR_BIT(RCC->TIM7CFGR, RCC_TIM7CFGR_TIM7EN)
#define __HAL_RCC_TIM8_CLK_DISABLE() CLEAR_BIT(RCC->TIM8CFGR, RCC_TIM8CFGR_TIM8EN)
#define __HAL_RCC_TIM10_CLK_DISABLE() CLEAR_BIT(RCC->TIM10CFGR, RCC_TIM10CFGR_TIM10EN)
#define __HAL_RCC_TIM11_CLK_DISABLE() CLEAR_BIT(RCC->TIM11CFGR, RCC_TIM11CFGR_TIM11EN)
#define __HAL_RCC_TIM12_CLK_DISABLE() CLEAR_BIT(RCC->TIM12CFGR, RCC_TIM12CFGR_TIM12EN)
#define __HAL_RCC_TIM13_CLK_DISABLE() CLEAR_BIT(RCC->TIM13CFGR, RCC_TIM13CFGR_TIM13EN)
#define __HAL_RCC_TIM14_CLK_DISABLE() CLEAR_BIT(RCC->TIM14CFGR, RCC_TIM14CFGR_TIM14EN)
#define __HAL_RCC_TIM15_CLK_DISABLE() CLEAR_BIT(RCC->TIM15CFGR, RCC_TIM15CFGR_TIM15EN)
#define __HAL_RCC_TIM16_CLK_DISABLE() CLEAR_BIT(RCC->TIM16CFGR, RCC_TIM16CFGR_TIM16EN)
#define __HAL_RCC_TIM17_CLK_DISABLE() CLEAR_BIT(RCC->TIM17CFGR, RCC_TIM17CFGR_TIM17EN)
#define __HAL_RCC_TIM20_CLK_DISABLE() CLEAR_BIT(RCC->TIM20CFGR, RCC_TIM20CFGR_TIM20EN)
#define __HAL_RCC_LPTIM1_CLK_DISABLE() CLEAR_BIT(RCC->LPTIM1CFGR, RCC_LPTIM1CFGR_LPTIM1EN)
#define __HAL_RCC_LPTIM2_CLK_DISABLE() CLEAR_BIT(RCC->LPTIM2CFGR, RCC_LPTIM2CFGR_LPTIM2EN)
#define __HAL_RCC_LPTIM3_CLK_DISABLE() CLEAR_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3EN)
#define __HAL_RCC_LPTIM4_CLK_DISABLE() CLEAR_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4EN)
#define __HAL_RCC_LPTIM5_CLK_DISABLE() CLEAR_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5EN)
#define __HAL_RCC_SPI1_CLK_DISABLE() CLEAR_BIT(RCC->SPI1CFGR, RCC_SPI1CFGR_SPI1EN)
#define __HAL_RCC_SPI2_CLK_DISABLE() CLEAR_BIT(RCC->SPI2CFGR, RCC_SPI2CFGR_SPI2EN)
#define __HAL_RCC_SPI3_CLK_DISABLE() CLEAR_BIT(RCC->SPI3CFGR, RCC_SPI3CFGR_SPI3EN)
#define __HAL_RCC_SPI4_CLK_DISABLE() CLEAR_BIT(RCC->SPI4CFGR, RCC_SPI4CFGR_SPI4EN)
#define __HAL_RCC_SPI5_CLK_DISABLE() CLEAR_BIT(RCC->SPI5CFGR, RCC_SPI5CFGR_SPI5EN)
#define __HAL_RCC_SPI6_CLK_DISABLE() CLEAR_BIT(RCC->SPI6CFGR, RCC_SPI6CFGR_SPI6EN)
#define __HAL_RCC_SPI7_CLK_DISABLE() CLEAR_BIT(RCC->SPI7CFGR, RCC_SPI7CFGR_SPI7EN)
#define __HAL_RCC_SPI8_CLK_DISABLE() CLEAR_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8EN)
#define __HAL_RCC_SPDIFRX_CLK_DISABLE() CLEAR_BIT(RCC->SPDIFRXCFGR, RCC_SPDIFRXCFGR_SPDIFRXEN)
#define __HAL_RCC_USART1_CLK_DISABLE() CLEAR_BIT(RCC->USART1CFGR, RCC_USART1CFGR_USART1EN)
#define __HAL_RCC_USART2_CLK_DISABLE() CLEAR_BIT(RCC->USART2CFGR, RCC_USART2CFGR_USART2EN)
#define __HAL_RCC_USART3_CLK_DISABLE() CLEAR_BIT(RCC->USART3CFGR, RCC_USART3CFGR_USART3EN)
#define __HAL_RCC_UART4_CLK_DISABLE() CLEAR_BIT(RCC->UART4CFGR, RCC_UART4CFGR_UART4EN)
#define __HAL_RCC_UART5_CLK_DISABLE() CLEAR_BIT(RCC->UART5CFGR, RCC_UART5CFGR_UART5EN)
#define __HAL_RCC_USART6_CLK_DISABLE() CLEAR_BIT(RCC->USART6CFGR, RCC_USART6CFGR_USART6EN)
#define __HAL_RCC_UART7_CLK_DISABLE() CLEAR_BIT(RCC->UART7CFGR, RCC_UART7CFGR_UART7EN)
#define __HAL_RCC_UART8_CLK_DISABLE() CLEAR_BIT(RCC->UART8CFGR, RCC_UART8CFGR_UART8EN)
#define __HAL_RCC_UART9_CLK_DISABLE() CLEAR_BIT(RCC->UART9CFGR, RCC_UART9CFGR_UART9EN)
#define __HAL_RCC_LPUART1_CLK_DISABLE() CLEAR_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1EN)
#define __HAL_RCC_I2C1_CLK_DISABLE() CLEAR_BIT(RCC->I2C1CFGR, RCC_I2C1CFGR_I2C1EN)
#define __HAL_RCC_I2C2_CLK_DISABLE() CLEAR_BIT(RCC->I2C2CFGR, RCC_I2C2CFGR_I2C2EN)
#define __HAL_RCC_I2C3_CLK_DISABLE() CLEAR_BIT(RCC->I2C3CFGR, RCC_I2C3CFGR_I2C3EN)
#define __HAL_RCC_I2C4_CLK_DISABLE() CLEAR_BIT(RCC->I2C4CFGR, RCC_I2C4CFGR_I2C4EN)
#define __HAL_RCC_I2C5_CLK_DISABLE() CLEAR_BIT(RCC->I2C5CFGR, RCC_I2C5CFGR_I2C5EN)
#define __HAL_RCC_I2C6_CLK_DISABLE() CLEAR_BIT(RCC->I2C6CFGR, RCC_I2C6CFGR_I2C6EN)
#define __HAL_RCC_I2C7_CLK_DISABLE() CLEAR_BIT(RCC->I2C7CFGR, RCC_I2C7CFGR_I2C7EN)
#define __HAL_RCC_I2C8_CLK_DISABLE() CLEAR_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8EN)
#define __HAL_RCC_SAI1_CLK_DISABLE() CLEAR_BIT(RCC->SAI1CFGR, RCC_SAI1CFGR_SAI1EN)
#define __HAL_RCC_SAI2_CLK_DISABLE() CLEAR_BIT(RCC->SAI2CFGR, RCC_SAI2CFGR_SAI2EN)
#define __HAL_RCC_SAI3_CLK_DISABLE() CLEAR_BIT(RCC->SAI3CFGR, RCC_SAI3CFGR_SAI3EN)
#define __HAL_RCC_SAI4_CLK_DISABLE() CLEAR_BIT(RCC->SAI4CFGR, RCC_SAI4CFGR_SAI4EN)
#define __HAL_RCC_MDF1_CLK_DISABLE() CLEAR_BIT(RCC->MDF1CFGR, RCC_MDF1CFGR_MDF1EN)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_CLK_DISABLE() CLEAR_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1EN)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_CLK_DISABLE() CLEAR_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2EN)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_FDCAN_CLK_DISABLE() CLEAR_BIT(RCC->FDCANCFGR, RCC_FDCANCFGR_FDCANEN)
#define __HAL_RCC_HDP_CLK_DISABLE() CLEAR_BIT(RCC->HDPCFGR, RCC_HDPCFGR_HDPEN)
#define __HAL_RCC_ADC12_CLK_DISABLE() CLEAR_BIT(RCC->ADC12CFGR, RCC_ADC12CFGR_ADC12EN)
#define __HAL_RCC_ADC3_CLK_DISABLE() CLEAR_BIT(RCC->ADC3CFGR, RCC_ADC3CFGR_ADC3EN)
#define __HAL_RCC_ETH1_CLK_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1EN)
#define __HAL_RCC_ETH1MAC_CLK_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1MACEN)
#define __HAL_RCC_ETH1TX_CLK_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1TXEN)
#define __HAL_RCC_ETH1RX_CLK_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1RXEN)
#define __HAL_RCC_ETH2_CLK_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2EN)
#define __HAL_RCC_ETH2MAC_CLK_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2MACEN)
#define __HAL_RCC_ETH2TX_CLK_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2TXEN)
#define __HAL_RCC_ETH2RX_CLK_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2RXEN)
#define __HAL_RCC_USB2_CLK_DISABLE() CLEAR_BIT(RCC->USB2CFGR, RCC_USB2CFGR_USB2EN)
#define __HAL_RCC_USB2PHY1_CLK_DISABLE() CLEAR_BIT(RCC->USB2PHY1CFGR, RCC_USB2PHY1CFGR_USB2PHY1EN)
#define __HAL_RCC_USB2PHY2_CLK_DISABLE() CLEAR_BIT(RCC->USB2PHY2CFGR, RCC_USB2PHY2CFGR_USB2PHY2EN)
#define __HAL_RCC_USB3DRD_CLK_DISABLE() CLEAR_BIT(RCC->USB3DRDCFGR, RCC_USB3DRDCFGR_USB3DRDEN)
#define __HAL_RCC_USB3PCIEPHY_CLK_DISABLE() CLEAR_BIT(RCC->USB3PCIEPHYCFGR, RCC_USB3PCIEPHYCFGR_USB3PCIEPHYEN)
#define __HAL_RCC_PCIE_CLK_DISABLE() CLEAR_BIT(RCC->PCIECFGR, RCC_PCIECFGR_PCIEEN)
#define __HAL_RCC_UCPD1_CLK_DISABLE() CLEAR_BIT(RCC->UCPD1CFGR, RCC_UCPD1CFGR_UCPD1EN)
#define __HAL_RCC_FMC_CLK_DISABLE() CLEAR_BIT(RCC->FMCCFGR, RCC_FMCCFGR_FMCEN)
#define __HAL_RCC_SDMMC1_CLK_DISABLE() CLEAR_BIT(RCC->SDMMC1CFGR, RCC_SDMMC1CFGR_SDMMC1EN)
#define __HAL_RCC_SDMMC2_CLK_DISABLE() CLEAR_BIT(RCC->SDMMC2CFGR, RCC_SDMMC2CFGR_SDMMC2EN)
#define __HAL_RCC_SDMMC3_CLK_DISABLE() CLEAR_BIT(RCC->SDMMC3CFGR, RCC_SDMMC3CFGR_SDMMC3EN)
#define __HAL_RCC_GPU_CLK_DISABLE() CLEAR_BIT(RCC->GPUCFGR, RCC_GPUCFGR_GPUEN)
#define __HAL_RCC_LTDC_CLK_DISABLE() CLEAR_BIT(RCC->LTDCCFGR, RCC_LTDCCFGR_LTDCEN)
#define __HAL_RCC_DSI_CLK_DISABLE() CLEAR_BIT(RCC->DSICFGR, RCC_DSICFGR_DSIEN)
#define __HAL_RCC_LVDS_CLK_DISABLE() CLEAR_BIT(RCC->LVDSCFGR, RCC_LVDSCFGR_LVDSEN)
#define __HAL_RCC_CSI2_CLK_DISABLE() CLEAR_BIT(RCC->CSI2CFGR, RCC_CSI2CFGR_CSI2EN)
#define __HAL_RCC_DCMIPP_CLK_DISABLE() CLEAR_BIT(RCC->DCMIPPCFGR, RCC_DCMIPPCFGR_DCMIPPEN)
#define __HAL_RCC_CCI_CLK_DISABLE() CLEAR_BIT(RCC->CCICFGR, RCC_CCICFGR_CCIEN)
#define __HAL_RCC_RNG_CLK_DISABLE() CLEAR_BIT(RCC->RNGCFGR, RCC_RNGCFGR_RNGEN)
#define __HAL_RCC_PKA_CLK_DISABLE() CLEAR_BIT(RCC->PKACFGR, RCC_PKACFGR_PKAEN)
#define __HAL_RCC_SAES_CLK_DISABLE() CLEAR_BIT(RCC->SAESCFGR, RCC_SAESCFGR_SAESEN)
#define __HAL_RCC_HASH_CLK_DISABLE() CLEAR_BIT(RCC->HASHCFGR, RCC_HASHCFGR_HASHEN)
#define __HAL_RCC_CRYP1_CLK_DISABLE() CLEAR_BIT(RCC->CRYP1CFGR, RCC_CRYP1CFGR_CRYP1EN)
#define __HAL_RCC_CRYP2_CLK_DISABLE() CLEAR_BIT(RCC->CRYP2CFGR, RCC_CRYP2CFGR_CRYP2EN)
#define __HAL_RCC_IWDG1_CLK_DISABLE() CLEAR_BIT(RCC->IWDG1CFGR, RCC_IWDG1CFGR_IWDG1EN)
#define __HAL_RCC_IWDG2_CLK_DISABLE() CLEAR_BIT(RCC->IWDG2CFGR, RCC_IWDG2CFGR_IWDG2EN)
#define __HAL_RCC_IWDG3_CLK_DISABLE() CLEAR_BIT(RCC->IWDG3CFGR, RCC_IWDG3CFGR_IWDG3EN)
#define __HAL_RCC_IWDG4_CLK_DISABLE() CLEAR_BIT(RCC->IWDG4CFGR, RCC_IWDG4CFGR_IWDG4EN)
#define __HAL_RCC_IWDG5_CLK_DISABLE() CLEAR_BIT(RCC->IWDG5CFGR, RCC_IWDG5CFGR_IWDG5EN)
#define __HAL_RCC_WWDG1_CLK_DISABLE() CLEAR_BIT(RCC->WWDG1CFGR, RCC_WWDG1CFGR_WWDG1EN)
#define __HAL_RCC_WWDG2_CLK_DISABLE() CLEAR_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2EN)
#define __HAL_RCC_BUSPERFM_CLK_DISABLE() CLEAR_BIT(RCC->BUSPERFMCFGR, RCC_BUSPERFMCFGR_BUSPERFMEN)
#define __HAL_RCC_VREF_CLK_DISABLE() CLEAR_BIT(RCC->VREFCFGR, RCC_VREFCFGR_VREFEN)
#define __HAL_RCC_TMPSENS_CLK_DISABLE() CLEAR_BIT(RCC->TMPSENSCFGR, RCC_TMPSENSCFGR_TMPSENSEN)
#define __HAL_RCC_CRC_CLK_DISABLE() CLEAR_BIT(RCC->CRCCFGR, RCC_CRCCFGR_CRCEN)
#define __HAL_RCC_BSEC_CLK_DISABLE() CLEAR_BIT(RCC->BSECCFGR, RCC_BSECCFGR_BSECEN)
#define __HAL_RCC_GICV2M_CLK_DISABLE() CLEAR_BIT(RCC->GICV2MCFGR, RCC_GICV2MCFGR_GICV2MEN)
#define __HAL_RCC_I3C1_CLK_DISABLE() CLEAR_BIT(RCC->I3C1CFGR, RCC_I3C1CFGR_I3C1EN)
#define __HAL_RCC_I3C2_CLK_DISABLE() CLEAR_BIT(RCC->I3C2CFGR, RCC_I3C2CFGR_I3C2EN)
#define __HAL_RCC_I3C3_CLK_DISABLE() CLEAR_BIT(RCC->I3C3CFGR, RCC_I3C3CFGR_I3C3EN)
#define __HAL_RCC_I3C4_CLK_DISABLE() CLEAR_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4EN)

/* Clock sleep enable macros */
#define __HAL_RCC_VDERAM_CLK_SLEEP_ENABLE() SET_BIT(RCC->VDERAMCFGR, RCC_VDERAMCFGR_VDERAMLPEN)
#define __HAL_RCC_SERC_CLK_SLEEP_ENABLE() SET_BIT(RCC->SERCCFGR, RCC_SERCCFGR_SERCLPEN)
#define __HAL_RCC_OSPIIOM_CLK_SLEEP_ENABLE() SET_BIT(RCC->OSPIIOMCFGR, RCC_OSPIIOMCFGR_OSPIIOMLPEN)
#define __HAL_RCC_ETHSW_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETHSWCFGR, RCC_ETHSWCFGR_ETHSWMACLPEN|\
                                                   RCC_ETHSWCFGR_ETHSWLPEN|RCC_ETHSWCFGR_ETHSWREFLPEN)
#define __HAL_RCC_VDEC_CLK_SLEEP_ENABLE() SET_BIT(RCC->VDECCFGR, RCC_VDECCFGR_VDECLPEN)
#define __HAL_RCC_VENC_CLK_SLEEP_ENABLE() SET_BIT(RCC->VENCCFGR, RCC_VENCCFGR_VENCLPEN)
#define __HAL_RCC_C3_CLK_SLEEP_ENABLE() SET_BIT(RCC->C3CFGR, RCC_C3CFGR_C3LPEN)
#define __HAL_RCC_SYSRAM_CLK_SLEEP_ENABLE() SET_BIT(RCC->SYSRAMCFGR, RCC_SYSRAMCFGR_SYSRAMLPEN)
#define __HAL_RCC_SRAM1_CLK_SLEEP_ENABLE() SET_BIT(RCC->SRAM1CFGR, RCC_SRAM1CFGR_SRAM1LPEN)
#define __HAL_RCC_SRAM2_CLK_SLEEP_ENABLE() SET_BIT(RCC->SRAM2CFGR, RCC_SRAM2CFGR_SRAM2LPEN)
#define __HAL_RCC_RETRAM_CLK_SLEEP_ENABLE() SET_BIT(RCC->RETRAMCFGR, RCC_RETRAMCFGR_RETRAMLPEN)
#define __HAL_RCC_BKPSRAM_CLK_SLEEP_ENABLE() SET_BIT(RCC->BKPSRAMCFGR, RCC_BKPSRAMCFGR_BKPSRAMLPEN)
#define __HAL_RCC_LPSRAM1_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPSRAM1CFGR, RCC_LPSRAM1CFGR_LPSRAM1LPEN)
#define __HAL_RCC_LPSRAM2_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPSRAM2CFGR, RCC_LPSRAM2CFGR_LPSRAM2LPEN)
#define __HAL_RCC_LPSRAM3_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPSRAM3CFGR, RCC_LPSRAM3CFGR_LPSRAM3LPEN)
#define __HAL_RCC_OSPI1_CLK_SLEEP_ENABLE() SET_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OSPI1LPEN)
#define __HAL_RCC_OSPI2_CLK_SLEEP_ENABLE() SET_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OSPI2LPEN)
#define __HAL_RCC_STM500_CLK_SLEEP_ENABLE() SET_BIT(RCC->STM500CFGR, RCC_STM500CFGR_STM500LPEN)
#define __HAL_RCC_ETR_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETRCFGR, RCC_ETRCFGR_ETRLPEN)
#define __HAL_RCC_GPIOA_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOACFGR, RCC_GPIOACFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOB_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOBCFGR, RCC_GPIOBCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOC_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOCCFGR, RCC_GPIOCCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOD_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIODCFGR, RCC_GPIODCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOE_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOECFGR, RCC_GPIOECFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOF_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOFCFGR, RCC_GPIOFCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOG_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOGCFGR, RCC_GPIOGCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOH_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOHCFGR, RCC_GPIOHCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOI_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOICFGR, RCC_GPIOICFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOJ_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOJCFGR, RCC_GPIOJCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOK_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOKCFGR, RCC_GPIOKCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOZ_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZLPEN)
#define __HAL_RCC_HPDMA1_CLK_SLEEP_ENABLE() SET_BIT(RCC->HPDMA1CFGR, RCC_HPDMA1CFGR_HPDMA1LPEN)
#define __HAL_RCC_HPDMA2_CLK_SLEEP_ENABLE() SET_BIT(RCC->HPDMA2CFGR, RCC_HPDMA2CFGR_HPDMA2LPEN)
#define __HAL_RCC_HPDMA3_CLK_SLEEP_ENABLE() SET_BIT(RCC->HPDMA3CFGR, RCC_HPDMA3CFGR_HPDMA3LPEN)
#define __HAL_RCC_LPDMA_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMALPEN)
#define __HAL_RCC_HSEM_CLK_SLEEP_ENABLE() SET_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMLPEN)
#define __HAL_RCC_IPCC1_CLK_SLEEP_ENABLE() SET_BIT(RCC->IPCC1CFGR, RCC_IPCC1CFGR_IPCC1LPEN)
#define __HAL_RCC_IPCC2_CLK_SLEEP_ENABLE() SET_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2LPEN)
#define __HAL_RCC_RTC_CLK_SLEEP_ENABLE() SET_BIT(RCC->RTCCFGR, RCC_RTCCFGR_RTCLPEN)
#define __HAL_RCC_SYSCPU1_CLK_SLEEP_ENABLE() SET_BIT(RCC->SYSCPU1CFGR, RCC_SYSCPU1CFGR_SYSCPU1LPEN)
#define __HAL_RCC_IS2M_CLK_SLEEP_ENABLE() SET_BIT(RCC->IS2MCFGR, RCC_IS2MCFGR_IS2MLPEN)
#define __HAL_RCC_TIM1_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM1CFGR, RCC_TIM1CFGR_TIM1LPEN)
#define __HAL_RCC_TIM2_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM2CFGR, RCC_TIM2CFGR_TIM2LPEN)
#define __HAL_RCC_TIM3_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM3CFGR, RCC_TIM3CFGR_TIM3LPEN)
#define __HAL_RCC_TIM4_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM4CFGR, RCC_TIM4CFGR_TIM4LPEN)
#define __HAL_RCC_TIM5_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM5CFGR, RCC_TIM5CFGR_TIM5LPEN)
#define __HAL_RCC_TIM6_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM6CFGR, RCC_TIM6CFGR_TIM6LPEN)
#define __HAL_RCC_TIM7_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM7CFGR, RCC_TIM7CFGR_TIM7LPEN)
#define __HAL_RCC_TIM8_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM8CFGR, RCC_TIM8CFGR_TIM8LPEN)
#define __HAL_RCC_TIM10_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM10CFGR, RCC_TIM10CFGR_TIM10LPEN)
#define __HAL_RCC_TIM11_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM11CFGR, RCC_TIM11CFGR_TIM11LPEN)
#define __HAL_RCC_TIM12_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM12CFGR, RCC_TIM12CFGR_TIM12LPEN)
#define __HAL_RCC_TIM13_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM13CFGR, RCC_TIM13CFGR_TIM13LPEN)
#define __HAL_RCC_TIM14_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM14CFGR, RCC_TIM14CFGR_TIM14LPEN)
#define __HAL_RCC_TIM15_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM15CFGR, RCC_TIM15CFGR_TIM15LPEN)
#define __HAL_RCC_TIM16_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM16CFGR, RCC_TIM16CFGR_TIM16LPEN)
#define __HAL_RCC_TIM17_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM17CFGR, RCC_TIM17CFGR_TIM17LPEN)
#define __HAL_RCC_TIM20_CLK_SLEEP_ENABLE() SET_BIT(RCC->TIM20CFGR, RCC_TIM20CFGR_TIM20LPEN)
#define __HAL_RCC_LPTIM1_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPTIM1CFGR, RCC_LPTIM1CFGR_LPTIM1LPEN)
#define __HAL_RCC_LPTIM2_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPTIM2CFGR, RCC_LPTIM2CFGR_LPTIM2LPEN)
#define __HAL_RCC_LPTIM3_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3LPEN)
#define __HAL_RCC_LPTIM4_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4LPEN)
#define __HAL_RCC_LPTIM5_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5LPEN)
#define __HAL_RCC_SPI1_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI1CFGR, RCC_SPI1CFGR_SPI1LPEN)
#define __HAL_RCC_SPI2_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI2CFGR, RCC_SPI2CFGR_SPI2LPEN)
#define __HAL_RCC_SPI3_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI3CFGR, RCC_SPI3CFGR_SPI3LPEN)
#define __HAL_RCC_SPI4_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI4CFGR, RCC_SPI4CFGR_SPI4LPEN)
#define __HAL_RCC_SPI5_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI5CFGR, RCC_SPI5CFGR_SPI5LPEN)
#define __HAL_RCC_SPI6_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI6CFGR, RCC_SPI6CFGR_SPI6LPEN)
#define __HAL_RCC_SPI7_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI7CFGR, RCC_SPI7CFGR_SPI7LPEN)
#define __HAL_RCC_SPI8_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8LPEN)
#define __HAL_RCC_SPDIFRX_CLK_SLEEP_ENABLE() SET_BIT(RCC->SPDIFRXCFGR, RCC_SPDIFRXCFGR_SPDIFRXLPEN)
#define __HAL_RCC_USART1_CLK_SLEEP_ENABLE() SET_BIT(RCC->USART1CFGR, RCC_USART1CFGR_USART1LPEN)
#define __HAL_RCC_USART2_CLK_SLEEP_ENABLE() SET_BIT(RCC->USART2CFGR, RCC_USART2CFGR_USART2LPEN)
#define __HAL_RCC_USART3_CLK_SLEEP_ENABLE() SET_BIT(RCC->USART3CFGR, RCC_USART3CFGR_USART3LPEN)
#define __HAL_RCC_UART4_CLK_SLEEP_ENABLE() SET_BIT(RCC->UART4CFGR, RCC_UART4CFGR_UART4LPEN)
#define __HAL_RCC_UART5_CLK_SLEEP_ENABLE() SET_BIT(RCC->UART5CFGR, RCC_UART5CFGR_UART5LPEN)
#define __HAL_RCC_USART6_CLK_SLEEP_ENABLE() SET_BIT(RCC->USART6CFGR, RCC_USART6CFGR_USART6LPEN)
#define __HAL_RCC_UART7_CLK_SLEEP_ENABLE() SET_BIT(RCC->UART7CFGR, RCC_UART7CFGR_UART7LPEN)
#define __HAL_RCC_UART8_CLK_SLEEP_ENABLE() SET_BIT(RCC->UART8CFGR, RCC_UART8CFGR_UART8LPEN)
#define __HAL_RCC_UART9_CLK_SLEEP_ENABLE() SET_BIT(RCC->UART9CFGR, RCC_UART9CFGR_UART9LPEN)
#define __HAL_RCC_LPUART1_CLK_SLEEP_ENABLE() SET_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1LPEN)
#define __HAL_RCC_I2C1_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C1CFGR, RCC_I2C1CFGR_I2C1LPEN)
#define __HAL_RCC_I2C2_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C2CFGR, RCC_I2C2CFGR_I2C2LPEN)
#define __HAL_RCC_I2C3_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C3CFGR, RCC_I2C3CFGR_I2C3LPEN)
#define __HAL_RCC_I2C4_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C4CFGR, RCC_I2C4CFGR_I2C4LPEN)
#define __HAL_RCC_I2C5_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C5CFGR, RCC_I2C5CFGR_I2C5LPEN)
#define __HAL_RCC_I2C6_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C6CFGR, RCC_I2C6CFGR_I2C6LPEN)
#define __HAL_RCC_I2C7_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C7CFGR, RCC_I2C7CFGR_I2C7LPEN)
#define __HAL_RCC_I2C8_CLK_SLEEP_ENABLE() SET_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8LPEN)
#define __HAL_RCC_SAI1_CLK_SLEEP_ENABLE() SET_BIT(RCC->SAI1CFGR, RCC_SAI1CFGR_SAI1LPEN)
#define __HAL_RCC_SAI2_CLK_SLEEP_ENABLE() SET_BIT(RCC->SAI2CFGR, RCC_SAI2CFGR_SAI2LPEN)
#define __HAL_RCC_SAI3_CLK_SLEEP_ENABLE() SET_BIT(RCC->SAI3CFGR, RCC_SAI3CFGR_SAI3LPEN)
#define __HAL_RCC_SAI4_CLK_SLEEP_ENABLE() SET_BIT(RCC->SAI4CFGR, RCC_SAI4CFGR_SAI4LPEN)
#define __HAL_RCC_MDF1_CLK_SLEEP_ENABLE() SET_BIT(RCC->MDF1CFGR, RCC_MDF1CFGR_MDF1LPEN)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_CLK_SLEEP_ENABLE() SET_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1LPEN)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_CLK_SLEEP_ENABLE() SET_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2LPEN)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_FDCAN_CLK_SLEEP_ENABLE() SET_BIT(RCC->FDCANCFGR, RCC_FDCANCFGR_FDCANLPEN)
#define __HAL_RCC_ADC12_CLK_SLEEP_ENABLE() SET_BIT(RCC->ADC12CFGR, RCC_ADC12CFGR_ADC12LPEN)
#define __HAL_RCC_ADC3_CLK_SLEEP_ENABLE() SET_BIT(RCC->ADC3CFGR, RCC_ADC3CFGR_ADC3LPEN)
#define __HAL_RCC_ETH1_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1LPEN)
#define __HAL_RCC_ETH1MAC_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1MACLPEN)
#define __HAL_RCC_ETH1TX_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1TXLPEN)
#define __HAL_RCC_ETH1RX_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1RXLPEN)
#define __HAL_RCC_ETH2_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2LPEN)
#define __HAL_RCC_ETH2MAC_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2MACLPEN)
#define __HAL_RCC_ETH2TX_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2TXLPEN)
#define __HAL_RCC_ETH2RX_CLK_SLEEP_ENABLE() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2RXLPEN)
#define __HAL_RCC_USB2_CLK_SLEEP_ENABLE() SET_BIT(RCC->USB2CFGR, RCC_USB2CFGR_USB2LPEN)
#define __HAL_RCC_USB2PHY1_CLK_SLEEP_ENABLE() SET_BIT(RCC->USB2PHY1CFGR, RCC_USB2PHY1CFGR_USB2PHY1LPEN)
#define __HAL_RCC_USB2PHY2_CLK_SLEEP_ENABLE() SET_BIT(RCC->USB2PHY2CFGR, RCC_USB2PHY2CFGR_USB2PHY2LPEN)
#define __HAL_RCC_USB3DRD_CLK_SLEEP_ENABLE() SET_BIT(RCC->USB3DRDCFGR, RCC_USB3DRDCFGR_USB3DRDLPEN)
#define __HAL_RCC_USB3PCIEPHY_CLK_SLEEP_ENABLE() SET_BIT(RCC->USB3PCIEPHYCFGR, RCC_USB3PCIEPHYCFGR_USB3PCIEPHYLPEN)
#define __HAL_RCC_PCIE_CLK_SLEEP_ENABLE() SET_BIT(RCC->PCIECFGR, RCC_PCIECFGR_PCIELPEN)
#define __HAL_RCC_UCPD1_CLK_SLEEP_ENABLE() SET_BIT(RCC->UCPD1CFGR, RCC_UCPD1CFGR_UCPD1LPEN)
#define __HAL_RCC_FMC_CLK_SLEEP_ENABLE() SET_BIT(RCC->FMCCFGR, RCC_FMCCFGR_FMCLPEN)
#define __HAL_RCC_SDMMC1_CLK_SLEEP_ENABLE() SET_BIT(RCC->SDMMC1CFGR, RCC_SDMMC1CFGR_SDMMC1LPEN)
#define __HAL_RCC_SDMMC2_CLK_SLEEP_ENABLE() SET_BIT(RCC->SDMMC2CFGR, RCC_SDMMC2CFGR_SDMMC2LPEN)
#define __HAL_RCC_SDMMC3_CLK_SLEEP_ENABLE() SET_BIT(RCC->SDMMC3CFGR, RCC_SDMMC3CFGR_SDMMC3LPEN)
#define __HAL_RCC_GPU_CLK_SLEEP_ENABLE() SET_BIT(RCC->GPUCFGR, RCC_GPUCFGR_GPULPEN)
#define __HAL_RCC_LTDC_CLK_SLEEP_ENABLE() SET_BIT(RCC->LTDCCFGR, RCC_LTDCCFGR_LTDCLPEN)
#define __HAL_RCC_DSI_CLK_SLEEP_ENABLE() SET_BIT(RCC->DSICFGR, RCC_DSICFGR_DSILPEN)
#define __HAL_RCC_LVDS_CLK_SLEEP_ENABLE() SET_BIT(RCC->LVDSCFGR, RCC_LVDSCFGR_LVDSLPEN)
#define __HAL_RCC_CSI2_CLK_SLEEP_ENABLE() SET_BIT(RCC->CSI2CFGR, RCC_CSI2CFGR_CSI2LPEN)
#define __HAL_RCC_DCMIPP_CLK_SLEEP_ENABLE() SET_BIT(RCC->DCMIPPCFGR, RCC_DCMIPPCFGR_DCMIPPLPEN)
#define __HAL_RCC_CCI_CLK_SLEEP_ENABLE() SET_BIT(RCC->CCICFGR, RCC_CCICFGR_CCILPEN)
#define __HAL_RCC_RNG_CLK_SLEEP_ENABLE() SET_BIT(RCC->RNGCFGR, RCC_RNGCFGR_RNGLPEN)
#define __HAL_RCC_PKA_CLK_SLEEP_ENABLE() SET_BIT(RCC->PKACFGR, RCC_PKACFGR_PKALPEN)
#define __HAL_RCC_SAES_CLK_SLEEP_ENABLE() SET_BIT(RCC->SAESCFGR, RCC_SAESCFGR_SAESLPEN)
#define __HAL_RCC_HASH_CLK_SLEEP_ENABLE() SET_BIT(RCC->HASHCFGR, RCC_HASHCFGR_HASHLPEN)
#define __HAL_RCC_CRYP1_CLK_SLEEP_ENABLE() SET_BIT(RCC->CRYP1CFGR, RCC_CRYP1CFGR_CRYP1LPEN)
#define __HAL_RCC_CRYP2_CLK_SLEEP_ENABLE() SET_BIT(RCC->CRYP2CFGR, RCC_CRYP2CFGR_CRYP2LPEN)
#define __HAL_RCC_IWDG1_CLK_SLEEP_ENABLE() SET_BIT(RCC->IWDG1CFGR, RCC_IWDG1CFGR_IWDG1LPEN)
#define __HAL_RCC_IWDG2_CLK_SLEEP_ENABLE() SET_BIT(RCC->IWDG2CFGR, RCC_IWDG2CFGR_IWDG2LPEN)
#define __HAL_RCC_IWDG3_CLK_SLEEP_ENABLE() SET_BIT(RCC->IWDG3CFGR, RCC_IWDG3CFGR_IWDG3LPEN)
#define __HAL_RCC_IWDG4_CLK_SLEEP_ENABLE() SET_BIT(RCC->IWDG4CFGR, RCC_IWDG4CFGR_IWDG4LPEN)
#define __HAL_RCC_IWDG5_CLK_SLEEP_ENABLE() SET_BIT(RCC->IWDG5CFGR, RCC_IWDG5CFGR_IWDG5LPEN)
#define __HAL_RCC_WWDG1_CLK_SLEEP_ENABLE() SET_BIT(RCC->WWDG1CFGR, RCC_WWDG1CFGR_WWDG1LPEN)
#define __HAL_RCC_WWDG2_CLK_SLEEP_ENABLE() SET_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2LPEN)
#define __HAL_RCC_BUSPERFM_CLK_SLEEP_ENABLE() SET_BIT(RCC->BUSPERFMCFGR, RCC_BUSPERFMCFGR_BUSPERFMLPEN)
#define __HAL_RCC_VREF_CLK_SLEEP_ENABLE() SET_BIT(RCC->VREFCFGR, RCC_VREFCFGR_VREFLPEN)
#define __HAL_RCC_TMPSENS_CLK_SLEEP_ENABLE() SET_BIT(RCC->TMPSENSCFGR, RCC_TMPSENSCFGR_TMPSENSLPEN)
#define __HAL_RCC_CRC_CLK_SLEEP_ENABLE() SET_BIT(RCC->CRCCFGR, RCC_CRCCFGR_CRCLPEN)
#define __HAL_RCC_BSEC_CLK_SLEEP_ENABLE() SET_BIT(RCC->BSECCFGR, RCC_BSECCFGR_BSECLPEN)
#define __HAL_RCC_GICV2M_CLK_SLEEP_ENABLE() SET_BIT(RCC->GICV2MCFGR, RCC_GICV2MCFGR_GICV2MLPEN)
#define __HAL_RCC_I3C1_CLK_SLEEP_ENABLE() SET_BIT(RCC->I3C1CFGR, RCC_I3C1CFGR_I3C1LPEN)
#define __HAL_RCC_I3C2_CLK_SLEEP_ENABLE() SET_BIT(RCC->I3C2CFGR, RCC_I3C2CFGR_I3C2LPEN)
#define __HAL_RCC_I3C3_CLK_SLEEP_ENABLE() SET_BIT(RCC->I3C3CFGR, RCC_I3C3CFGR_I3C3LPEN)
#define __HAL_RCC_I3C4_CLK_SLEEP_ENABLE() SET_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4LPEN)

/* Clock sleep disable macros */
#define __HAL_RCC_VDERAM_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->VDERAMCFGR, RCC_VDERAMCFGR_VDERAMLPEN)
#define __HAL_RCC_SERC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SERCCFGR, RCC_SERCCFGR_SERCLPEN)
#define __HAL_RCC_OSPIIOM_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->OSPIIOMCFGR, RCC_OSPIIOMCFGR_OSPIIOMLPEN)
#define __HAL_RCC_ETHSW_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETHSWCFGR, RCC_ETHSWCFGR_ETHSWMACLPEN|\
                                                      RCC_ETHSWCFGR_ETHSWLPEN|RCC_ETHSWCFGR_ETHSWREFLPEN)
#define __HAL_RCC_VDEC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->VDECCFGR, RCC_VDECCFGR_VDECLPEN)
#define __HAL_RCC_VENC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->VENCCFGR, RCC_VENCCFGR_VENCLPEN)
#define __HAL_RCC_C3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->C3CFGR, RCC_C3CFGR_C3LPEN)
#define __HAL_RCC_SYSRAM_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SYSRAMCFGR, RCC_SYSRAMCFGR_SYSRAMLPEN)
#define __HAL_RCC_SRAM1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SRAM1CFGR, RCC_SRAM1CFGR_SRAM1LPEN)
#define __HAL_RCC_SRAM2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SRAM2CFGR, RCC_SRAM2CFGR_SRAM2LPEN)
#define __HAL_RCC_RETRAM_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->RETRAMCFGR, RCC_RETRAMCFGR_RETRAMLPEN)
#define __HAL_RCC_BKPSRAM_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->BKPSRAMCFGR, RCC_BKPSRAMCFGR_BKPSRAMLPEN)
#define __HAL_RCC_LPSRAM1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPSRAM1CFGR, RCC_LPSRAM1CFGR_LPSRAM1LPEN)
#define __HAL_RCC_LPSRAM2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPSRAM2CFGR, RCC_LPSRAM2CFGR_LPSRAM2LPEN)
#define __HAL_RCC_LPSRAM3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPSRAM3CFGR, RCC_LPSRAM3CFGR_LPSRAM3LPEN)
#define __HAL_RCC_OSPI1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OSPI1LPEN)
#define __HAL_RCC_OSPI2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OSPI2LPEN)
#define __HAL_RCC_STM500_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->STM500CFGR, RCC_STM500CFGR_STM500LPEN)
#define __HAL_RCC_ETR_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETRCFGR, RCC_ETRCFGR_ETRLPEN)
#define __HAL_RCC_GPIOA_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOACFGR, RCC_GPIOACFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOB_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOBCFGR, RCC_GPIOBCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOCCFGR, RCC_GPIOCCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOD_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIODCFGR, RCC_GPIODCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOE_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOECFGR, RCC_GPIOECFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOF_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOFCFGR, RCC_GPIOFCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOG_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOGCFGR, RCC_GPIOGCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOH_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOHCFGR, RCC_GPIOHCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOI_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOICFGR, RCC_GPIOICFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOJ_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOJCFGR, RCC_GPIOJCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOK_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOKCFGR, RCC_GPIOKCFGR_GPIOxLPEN)
#define __HAL_RCC_GPIOZ_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZLPEN)
#define __HAL_RCC_HPDMA1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->HPDMA1CFGR, RCC_HPDMA1CFGR_HPDMA1LPEN)
#define __HAL_RCC_HPDMA2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->HPDMA2CFGR, RCC_HPDMA2CFGR_HPDMA2LPEN)
#define __HAL_RCC_HPDMA3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->HPDMA3CFGR, RCC_HPDMA3CFGR_HPDMA3LPEN)
#define __HAL_RCC_LPDMA_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMALPEN)
#define __HAL_RCC_HSEM_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMLPEN)
#define __HAL_RCC_IPCC1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IPCC1CFGR, RCC_IPCC1CFGR_IPCC1LPEN)
#define __HAL_RCC_IPCC2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2LPEN)
#define __HAL_RCC_RTC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->RTCCFGR, RCC_RTCCFGR_RTCLPEN)
#define __HAL_RCC_SYSCPU1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SYSCPU1CFGR, RCC_SYSCPU1CFGR_SYSCPU1LPEN)
#define __HAL_RCC_IS2M_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IS2MCFGR, RCC_IS2MCFGR_IS2MLPEN)
#define __HAL_RCC_TIM1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM1CFGR, RCC_TIM1CFGR_TIM1LPEN)
#define __HAL_RCC_TIM2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM2CFGR, RCC_TIM2CFGR_TIM2LPEN)
#define __HAL_RCC_TIM3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM3CFGR, RCC_TIM3CFGR_TIM3LPEN)
#define __HAL_RCC_TIM4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM4CFGR, RCC_TIM4CFGR_TIM4LPEN)
#define __HAL_RCC_TIM5_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM5CFGR, RCC_TIM5CFGR_TIM5LPEN)
#define __HAL_RCC_TIM6_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM6CFGR, RCC_TIM6CFGR_TIM6LPEN)
#define __HAL_RCC_TIM7_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM7CFGR, RCC_TIM7CFGR_TIM7LPEN)
#define __HAL_RCC_TIM8_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM8CFGR, RCC_TIM8CFGR_TIM8LPEN)
#define __HAL_RCC_TIM10_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM10CFGR, RCC_TIM10CFGR_TIM10LPEN)
#define __HAL_RCC_TIM11_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM11CFGR, RCC_TIM11CFGR_TIM11LPEN)
#define __HAL_RCC_TIM12_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM12CFGR, RCC_TIM12CFGR_TIM12LPEN)
#define __HAL_RCC_TIM13_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM13CFGR, RCC_TIM13CFGR_TIM13LPEN)
#define __HAL_RCC_TIM14_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM14CFGR, RCC_TIM14CFGR_TIM14LPEN)
#define __HAL_RCC_TIM15_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM15CFGR, RCC_TIM15CFGR_TIM15LPEN)
#define __HAL_RCC_TIM16_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM16CFGR, RCC_TIM16CFGR_TIM16LPEN)
#define __HAL_RCC_TIM17_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM17CFGR, RCC_TIM17CFGR_TIM17LPEN)
#define __HAL_RCC_TIM20_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TIM20CFGR, RCC_TIM20CFGR_TIM20LPEN)
#define __HAL_RCC_LPTIM1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPTIM1CFGR, RCC_LPTIM1CFGR_LPTIM1LPEN)
#define __HAL_RCC_LPTIM2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPTIM2CFGR, RCC_LPTIM2CFGR_LPTIM2LPEN)
#define __HAL_RCC_LPTIM3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3LPEN)
#define __HAL_RCC_LPTIM4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4LPEN)
#define __HAL_RCC_LPTIM5_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5LPEN)
#define __HAL_RCC_SPI1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI1CFGR, RCC_SPI1CFGR_SPI1LPEN)
#define __HAL_RCC_SPI2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI2CFGR, RCC_SPI2CFGR_SPI2LPEN)
#define __HAL_RCC_SPI3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI3CFGR, RCC_SPI3CFGR_SPI3LPEN)
#define __HAL_RCC_SPI4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI4CFGR, RCC_SPI4CFGR_SPI4LPEN)
#define __HAL_RCC_SPI5_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI5CFGR, RCC_SPI5CFGR_SPI5LPEN)
#define __HAL_RCC_SPI6_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI6CFGR, RCC_SPI6CFGR_SPI6LPEN)
#define __HAL_RCC_SPI7_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI7CFGR, RCC_SPI7CFGR_SPI7LPEN)
#define __HAL_RCC_SPI8_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8LPEN)
#define __HAL_RCC_SPDIFRX_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SPDIFRXCFGR, RCC_SPDIFRXCFGR_SPDIFRXLPEN)
#define __HAL_RCC_USART1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USART1CFGR, RCC_USART1CFGR_USART1LPEN)
#define __HAL_RCC_USART2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USART2CFGR, RCC_USART2CFGR_USART2LPEN)
#define __HAL_RCC_USART3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USART3CFGR, RCC_USART3CFGR_USART3LPEN)
#define __HAL_RCC_UART4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->UART4CFGR, RCC_UART4CFGR_UART4LPEN)
#define __HAL_RCC_UART5_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->UART5CFGR, RCC_UART5CFGR_UART5LPEN)
#define __HAL_RCC_USART6_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USART6CFGR, RCC_USART6CFGR_USART6LPEN)
#define __HAL_RCC_UART7_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->UART7CFGR, RCC_UART7CFGR_UART7LPEN)
#define __HAL_RCC_UART8_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->UART8CFGR, RCC_UART8CFGR_UART8LPEN)
#define __HAL_RCC_UART9_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->UART9CFGR, RCC_UART9CFGR_UART9LPEN)
#define __HAL_RCC_LPUART1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1LPEN)
#define __HAL_RCC_I2C1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C1CFGR, RCC_I2C1CFGR_I2C1LPEN)
#define __HAL_RCC_I2C2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C2CFGR, RCC_I2C2CFGR_I2C2LPEN)
#define __HAL_RCC_I2C3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C3CFGR, RCC_I2C3CFGR_I2C3LPEN)
#define __HAL_RCC_I2C4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C4CFGR, RCC_I2C4CFGR_I2C4LPEN)
#define __HAL_RCC_I2C5_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C5CFGR, RCC_I2C5CFGR_I2C5LPEN)
#define __HAL_RCC_I2C6_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C6CFGR, RCC_I2C6CFGR_I2C6LPEN)
#define __HAL_RCC_I2C7_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C7CFGR, RCC_I2C7CFGR_I2C7LPEN)
#define __HAL_RCC_I2C8_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8LPEN)
#define __HAL_RCC_SAI1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SAI1CFGR, RCC_SAI1CFGR_SAI1LPEN)
#define __HAL_RCC_SAI2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SAI2CFGR, RCC_SAI2CFGR_SAI2LPEN)
#define __HAL_RCC_SAI3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SAI3CFGR, RCC_SAI3CFGR_SAI3LPEN)
#define __HAL_RCC_SAI4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SAI4CFGR, RCC_SAI4CFGR_SAI4LPEN)
#define __HAL_RCC_MDF1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->MDF1CFGR, RCC_MDF1CFGR_MDF1LPEN)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1LPEN)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2LPEN)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_FDCAN_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->FDCANCFGR, RCC_FDCANCFGR_FDCANLPEN)
#define __HAL_RCC_ADC12_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ADC12CFGR, RCC_ADC12CFGR_ADC12LPEN)
#define __HAL_RCC_ADC3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ADC3CFGR, RCC_ADC3CFGR_ADC3LPEN)
#define __HAL_RCC_ETH1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1LPEN)
#define __HAL_RCC_ETH1MAC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1MACLPEN)
#define __HAL_RCC_ETH1TX_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1TXLPEN)
#define __HAL_RCC_ETH1RX_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1RXLPEN)
#define __HAL_RCC_ETH2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2LPEN)
#define __HAL_RCC_ETH2MAC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2MACLPEN)
#define __HAL_RCC_ETH2TX_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2TXLPEN)
#define __HAL_RCC_ETH2RX_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2RXLPEN)
#define __HAL_RCC_USB2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USB2CFGR, RCC_USB2CFGR_USB2LPEN)
#define __HAL_RCC_USB2PHY1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USB2PHY1CFGR, RCC_USB2PHY1CFGR_USB2PHY1LPEN)
#define __HAL_RCC_USB2PHY2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USB2PHY2CFGR, RCC_USB2PHY2CFGR_USB2PHY2LPEN)
#define __HAL_RCC_USB3DRD_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USB3DRDCFGR, RCC_USB3DRDCFGR_USB3DRDLPEN)
#define __HAL_RCC_USB3PCIEPHY_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->USB3PCIEPHYCFGR, RCC_USB3PCIEPHYCFGR_USB3PCIEPHYLPEN)
#define __HAL_RCC_PCIE_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->PCIECFGR, RCC_PCIECFGR_PCIELPEN)
#define __HAL_RCC_UCPD1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->UCPD1CFGR, RCC_UCPD1CFGR_UCPD1LPEN)
#define __HAL_RCC_FMC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->FMCCFGR, RCC_FMCCFGR_FMCLPEN)
#define __HAL_RCC_SDMMC1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SDMMC1CFGR, RCC_SDMMC1CFGR_SDMMC1LPEN)
#define __HAL_RCC_SDMMC2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SDMMC2CFGR, RCC_SDMMC2CFGR_SDMMC2LPEN)
#define __HAL_RCC_SDMMC3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SDMMC3CFGR, RCC_SDMMC3CFGR_SDMMC3LPEN)
#define __HAL_RCC_GPU_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GPUCFGR, RCC_GPUCFGR_GPULPEN)
#define __HAL_RCC_LTDC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LTDCCFGR, RCC_LTDCCFGR_LTDCLPEN)
#define __HAL_RCC_DSI_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->DSICFGR, RCC_DSICFGR_DSILPEN)
#define __HAL_RCC_LVDS_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->LVDSCFGR, RCC_LVDSCFGR_LVDSLPEN)
#define __HAL_RCC_CSI2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->CSI2CFGR, RCC_CSI2CFGR_CSI2LPEN)
#define __HAL_RCC_DCMIPP_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->DCMIPPCFGR, RCC_DCMIPPCFGR_DCMIPPLPEN)
#define __HAL_RCC_CCI_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->CCICFGR, RCC_CCICFGR_CCILPEN)
#define __HAL_RCC_RNG_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->RNGCFGR, RCC_RNGCFGR_RNGLPEN)
#define __HAL_RCC_PKA_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->PKACFGR, RCC_PKACFGR_PKALPEN)
#define __HAL_RCC_SAES_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->SAESCFGR, RCC_SAESCFGR_SAESLPEN)
#define __HAL_RCC_HASH_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->HASHCFGR, RCC_HASHCFGR_HASHLPEN)
#define __HAL_RCC_CRYP1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->CRYP1CFGR, RCC_CRYP1CFGR_CRYP1LPEN)
#define __HAL_RCC_CRYP2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->CRYP2CFGR, RCC_CRYP2CFGR_CRYP2LPEN)
#define __HAL_RCC_IWDG1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IWDG1CFGR, RCC_IWDG1CFGR_IWDG1LPEN)
#define __HAL_RCC_IWDG2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IWDG2CFGR, RCC_IWDG2CFGR_IWDG2LPEN)
#define __HAL_RCC_IWDG3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IWDG3CFGR, RCC_IWDG3CFGR_IWDG3LPEN)
#define __HAL_RCC_IWDG4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IWDG4CFGR, RCC_IWDG4CFGR_IWDG4LPEN)
#define __HAL_RCC_IWDG5_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->IWDG5CFGR, RCC_IWDG5CFGR_IWDG5LPEN)
#define __HAL_RCC_WWDG1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->WWDG1CFGR, RCC_WWDG1CFGR_WWDG1LPEN)
#define __HAL_RCC_WWDG2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2LPEN)
#define __HAL_RCC_BUSPERFM_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->BUSPERFMCFGR, RCC_BUSPERFMCFGR_BUSPERFMLPEN)
#define __HAL_RCC_VREF_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->VREFCFGR, RCC_VREFCFGR_VREFLPEN)
#define __HAL_RCC_TMPSENS_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->TMPSENSCFGR, RCC_TMPSENSCFGR_TMPSENSLPEN)
#define __HAL_RCC_CRC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->CRCCFGR, RCC_CRCCFGR_CRCLPEN)
#define __HAL_RCC_BSEC_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->BSECCFGR, RCC_BSECCFGR_BSECLPEN)
#define __HAL_RCC_GICV2M_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->GICV2MCFGR, RCC_GICV2MCFGR_GICV2MLPEN)
#define __HAL_RCC_I3C1_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I3C1CFGR, RCC_I3C1CFGR_I3C1LPEN)
#define __HAL_RCC_I3C2_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I3C2CFGR, RCC_I3C2CFGR_I3C2LPEN)
#define __HAL_RCC_I3C3_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I3C3CFGR, RCC_I3C3CFGR_I3C3LPEN)
#define __HAL_RCC_I3C4_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4LPEN)

/* Clock autonomous mode enable macros */
#define __HAL_RCC_C3_CLKAM_ENABLE() SET_BIT(RCC->C3CFGR, RCC_C3CFGR_C3AMEN)
#define __HAL_RCC_LPSRAM1_CLKAM_ENABLE() SET_BIT(RCC->LPSRAM1CFGR, RCC_LPSRAM1CFGR_LPSRAM1AMEN)
#define __HAL_RCC_LPSRAM2_CLKAM_ENABLE() SET_BIT(RCC->LPSRAM2CFGR, RCC_LPSRAM2CFGR_LPSRAM2AMEN)
#define __HAL_RCC_LPSRAM3_CLKAM_ENABLE() SET_BIT(RCC->LPSRAM3CFGR, RCC_LPSRAM3CFGR_LPSRAM3AMEN)
#define __HAL_RCC_GPIOZ_CLKAM_ENABLE() SET_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZAMEN)
#define __HAL_RCC_LPDMA_CLKAM_ENABLE() SET_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMAAMEN)
#define __HAL_RCC_HSEM_CLKAM_ENABLE() SET_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMAMEN)
#define __HAL_RCC_IPCC2_CLKAM_ENABLE() SET_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2AMEN)
#define __HAL_RCC_RTC_CLKAM_ENABLE() SET_BIT(RCC->RTCCFGR, RCC_RTCCFGR_RTCAMEN)
#define __HAL_RCC_LPTIM3_CLKAM_ENABLE() SET_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3AMEN)
#define __HAL_RCC_LPTIM4_CLKAM_ENABLE() SET_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4AMEN)
#define __HAL_RCC_LPTIM5_CLKAM_ENABLE() SET_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5AMEN)
#define __HAL_RCC_SPI8_CLKAM_ENABLE() SET_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8AMEN)
#define __HAL_RCC_LPUART1_CLKAM_ENABLE() SET_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1AMEN)
#define __HAL_RCC_I2C8_CLKAM_ENABLE() SET_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8AMEN)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_CLKAM_ENABLE() SET_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1AMEN)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_CLKAM_ENABLE() SET_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2AMEN)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_IWDG5_CLKAM_ENABLE() SET_BIT(RCC->IWDG5CFGR, RCC_IWDG5CFGR_IWDG5AMEN)
#define __HAL_RCC_WWDG2_CLKAM_ENABLE() SET_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2AMEN)
#define __HAL_RCC_I3C4_CLKAM_ENABLE() SET_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4AMEN)

/* Clock autonomous mode disable macros */
#define __HAL_RCC_C3_CLKAM_DISABLE() CLEAR_BIT(RCC->C3CFGR, RCC_C3CFGR_C3AMEN)
#define __HAL_RCC_LPSRAM1_CLKAM_DISABLE() CLEAR_BIT(RCC->LPSRAM1CFGR, RCC_LPSRAM1CFGR_LPSRAM1AMEN)
#define __HAL_RCC_LPSRAM2_CLKAM_DISABLE() CLEAR_BIT(RCC->LPSRAM2CFGR, RCC_LPSRAM2CFGR_LPSRAM2AMEN)
#define __HAL_RCC_LPSRAM3_CLKAM_DISABLE() CLEAR_BIT(RCC->LPSRAM3CFGR, RCC_LPSRAM3CFGR_LPSRAM3AMEN)
#define __HAL_RCC_GPIOZ_CLKAM_DISABLE() CLEAR_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZAMEN)
#define __HAL_RCC_LPDMA_CLKAM_DISABLE() CLEAR_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMAAMEN)
#define __HAL_RCC_HSEM_CLKAM_DISABLE() CLEAR_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMAMEN)
#define __HAL_RCC_IPCC2_CLKAM_DISABLE() CLEAR_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2AMEN)
#define __HAL_RCC_RTC_CLKAM_DISABLE() CLEAR_BIT(RCC->RTCCFGR, RCC_RTCCFGR_RTCAMEN)
#define __HAL_RCC_LPTIM3_CLKAM_DISABLE() CLEAR_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3AMEN)
#define __HAL_RCC_LPTIM4_CLKAM_DISABLE() CLEAR_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4AMEN)
#define __HAL_RCC_LPTIM5_CLKAM_DISABLE() CLEAR_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5AMEN)
#define __HAL_RCC_SPI8_CLKAM_DISABLE() CLEAR_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8AMEN)
#define __HAL_RCC_LPUART1_CLKAM_DISABLE() CLEAR_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1AMEN)
#define __HAL_RCC_I2C8_CLKAM_DISABLE() CLEAR_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8AMEN)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_CLKAM_DISABLE() CLEAR_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1AMEN)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_CLKAM_DISABLE() CLEAR_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2AMEN)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_IWDG5_CLKAM_DISABLE() CLEAR_BIT(RCC->IWDG5CFGR, RCC_IWDG5CFGR_IWDG5AMEN)
#define __HAL_RCC_WWDG2_CLKAM_DISABLE() CLEAR_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2AMEN)
#define __HAL_RCC_I3C4_CLKAM_DISABLE() CLEAR_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4AMEN)

/* Allows/Prevents CStop & Enhanced CSleep modes for all A35_* cores */
#ifdef CORE_CA35
#define __HAL_RCC_ALLOW_CSTOP() SET_BIT(RCC->C1SREQSETR, RCC_C1SREQSETR_STPREQ_P1 | RCC_C1SREQSETR_STPREQ_P0)
#define __HAL_RCC_PREVENT_CSTOP() SET_BIT(RCC->C1SREQCLRR, RCC_C1SREQCLRR_STPREQ_P1 | RCC_C1SREQCLRR_STPREQ_P0)
#define __HAL_RCC_ALLOW_ENHANCED_CSLEEP() SET_BIT(RCC->C1SREQSETR, RCC_C1SREQSETR_ESLPREQ)
#define __HAL_RCC_PREVENT_ENHANCED_CSLEEP() SET_BIT(RCC->C1SREQCLRR, RCC_C1SREQCLRR_ESLPREQ)
#endif

/* RCC_Calibration limits */
#define IS_RCC_HSICALIBRATION_VALUE(VALUE) ((VALUE) <= 0x3F) /* @TODO:Be sure value is right */
#define IS_RCC_CSICALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1F) /* @TODO:Be sure value is right */

/* MCO1 clock output control */
#define __HAL_RCC_MCO1_ENABLE()   SET_BIT(RCC->MCO1CFGR, RCC_MCO1CFGR_MCO1ON)
#define __HAL_RCC_MCO1_DISABLE()  CLEAR_BIT(RCC->MCO1CFGR, RCC_MCO1CFGR_MCO1ON)

/**
  * @brief  Macro to select source clock for MCO1 output
  *
  * @param  __RCC_MCO1SOURCE__: specifies the MCO1 entry clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_CK_MCO1: Flexiclockgen output 61
  *            @arg RCC_CK_OBS0: Flexiclockgen observation clock 0
  *
  * @retval None
  */
#define __HAL_RCC_MCO1_SETSOURCE(__RCC_MCO1SOURCE__) \
  MODIFY_REG( RCC->MCO1CFGR, RCC_MCO1CFGR_MCO1SEL, (__RCC_MCO1SOURCE__ == RCC_CK_MCO1) ?  0 : RCC_MCO1CFGR_MCO1SEL )

/** @brief  Macro to get the clock source used as MCO1 clock.
  * @retval The clock source used as MCO1 clock. The returned value can be one
  *         of the following:
  *            @arg RCC_CK_MCO1: Flexiclockgen output 61
  *            @arg RCC_CK_OBS0: Flexiclockgen observation clock 0
  */
#define __HAL_RCC_MCO1_GETSOURCE() ((RCC->MCO1CFGR & RCC_MCO1CFGR_MCO1SEL) ? RCC_CK_OBS0 : RCC_CK_MCO1)

/* MCO2 clock output control */
#define __HAL_RCC_MCO2_ENABLE()   SET_BIT(RCC->MCO2CFGR, RCC_MCO2CFGR_MCO2ON)
#define __HAL_RCC_MCO2_DISABLE()  CLEAR_BIT(RCC->MCO2CFGR, RCC_MCO2CFGR_MCO2ON)

/**
  * @brief  Macro to select source clock for MCO2 output
  *
  * @param  __RCC_MCO2SOURCE__: specifies the MCO2 entry clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_CK_MCO2: Flexiclockgen output 62
  *            @arg RCC_CK_OBS1: Flexiclockgen observation clock 1
  *
  * @retval None
  */
#define __HAL_RCC_MCO2_SETSOURCE(__RCC_MCO2SOURCE__) \
  MODIFY_REG( RCC->MCO2CFGR, RCC_MCO2CFGR_MCO2SEL, (__RCC_MCO2SOURCE__ == RCC_CK_MCO2) ?  0 : RCC_MCO2CFGR_MCO2SEL );

/** @brief  Macro to get the clock source used as MCO2 clock.
  * @retval The clock source used as MCO2 clock. The returned value can be one
  *         of the following:
  *            @arg RCC_CK_MCO2: Flexiclockgen output 62
  *            @arg RCC_CK_OBS1: Flexiclockgen observation clock 1
  */
#define __HAL_RCC_MCO2_GETSOURCE() ((RCC->MCO2CFGR & RCC_MCO2CFGR_MCO2SEL) ? RCC_CK_OBS1 : RCC_CK_MCO2)

/** @brief  Check RCC flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *         @TODO: To compleate this description
  * @retval The state of __FLAG__ (TRUE or FALSE).
  */

/** @defgroup RCC_ADC12_Kernel_Clock_Source RCC_ADC12_Kernel_Clock_Source
  * @{
  */
#define RCC_ADC12KERCLKSOURCE_CK_KER_ADC12     (0x0 << RCC_ADC12CFGR_ADC12KERSEL_Pos)
#define RCC_ADC12KERCLKSOURCE_CK_ICN_LS_MCU    (0x1 << RCC_ADC12CFGR_ADC12KERSEL_Pos)

#define IS_RCC_ADC12KERCLKSOURCE(SOURCE) (((SOURCE) == RCC_ADC12KERCLKSOURCE_CK_KER_ADC12) || \
                                          ((SOURCE) == RCC_ADC12KERCLKSOURCE_CK_ICN_LS_MCU))

/**
  * @brief  Macro to select ADC12 kernel source clock
  *
  * @param  __RCC_ADC12KERSOURCE__: specifies the ADC12 entry kernel clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_ADC12KERCLKSOURCE_CK_KER_ADC12:  "ck_ker_adc12"  Flexiclockgen output 46
  *            @arg RCC_ADC12KERCLKSOURCE_CK_ICN_LS_MCU: "ck_icn_ls_mcu" Flexiclockgen output 0
  *
  * @retval None
  */
#define __HAL_RCC_ADC12KERCLK_SETSOURCE(__RCC_ADC12KERSOURCE__) \
  MODIFY_REG( RCC->ADC12CFGR, RCC_ADC12CFGR_ADC12KERSEL,(__RCC_ADC12KERSOURCE__)  )

/** @brief  Macro to get the clock source used as ADC12 kernel source clock.
  * @retval The clock source used as ADC12 kernel source clock. The returned value can be one
  *         of the following:
  *            @arg RCC_ADC12KERCLKSOURCE_CK_KER_ADC12:  "ck_ker_adc12"  Flexiclockgen output 46
  *            @arg RCC_ADC12KERCLKSOURCE_CK_ICN_LS_MCU: "ck_icn_ls_mcu" Flexiclockgen output 0
  */
#define __HAL_RCC_ADC12KERCLK_GETSOURCE() ( (RCC->ADC12CFGR & RCC_ADC12CFGR_ADC12KERSEL_Msk) )

/** @defgroup RCC_ADC3_Kernel_Clock_Source RCC_ADC3_Kernel_Clock_Source
  * @{
  */
#define RCC_ADC3KERCLKSOURCE_CK_KER_ADC3      (0x0 << RCC_ADC3CFGR_ADC3KERSEL_Pos)
#define RCC_ADC3KERCLKSOURCE_CK_ICN_LS_MCU    (0x1 << RCC_ADC3CFGR_ADC3KERSEL_Pos)
#define RCC_ADC3KERCLKSOURCE_CK_KER_ADC12     (0x2 << RCC_ADC3CFGR_ADC3KERSEL_Pos)

#define IS_RCC_ADC3KERCLKSOURCE(SOURCE) (((SOURCE) == RCC_ADC3KERCLKSOURCE_CK_KER_ADC3) || \
                                         ((SOURCE) == RCC_ADC3KERCLKSOURCE_CK_ICN_LS_MCU) || \
                                         ((SOURCE) == RCC_ADC3KERCLKSOURCE_CK_KER_ADC12))

/**
  * @brief  Macro to select ADC3 kernel source clock
  *
  * @param  __RCC_ADC3KERSOURCE__: specifies the ADC3 entry kernel clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_ADC3KERCLKSOURCE_CK_KER_ADC3:   "ck_ker_adc3"   Flexiclockgen output 47
  *            @arg RCC_ADC3KERCLKSOURCE_CK_ICN_LS_MCU: "ck_icn_ls_mcu" Flexiclockgen output 0
  *            @arg RCC_ADC3KERCLKSOURCE_CK_KER_ADC12:  "ck_ker_adc12"  Flexiclockgen output 46
  *
  * @retval None
  */
#define __HAL_RCC_ADC3KERCLK_SETSOURCE(__RCC_ADC3KERSOURCE__) \
  MODIFY_REG( RCC->ADC3CFGR, RCC_ADC3CFGR_ADC3KERSEL, (__RCC_ADC3KERSOURCE__)  )

/** @brief  Macro to get the clock source used as ADC3 kernel source clock.
  * @retval The clock source used as ADC3 kernel source clock. The returned value can be one
  *         of the following:
  *            @arg RCC_ADC3KERCLKSOURCE_CK_KER_ADC3:   "ck_ker_adc3"   Flexiclockgen output 47
  *            @arg RCC_ADC3KERCLKSOURCE_CK_ICN_LS_MCU: "ck_icn_ls_mcu" Flexiclockgen output 0
  *            @arg RCC_ADC3KERCLKSOURCE_CK_KER_ADC12:  "ck_ker_adc12"  Flexiclockgen output 46
  */
#define __HAL_RCC_ADC3KERCLK_GETSOURCE() ( (RCC->ADC3CFGR & RCC_ADC3CFGR_ADC3KERSEL_Msk) )

#define __HAL_RCC_GET_FLAG(__FLAG__) ( __FLAG__ ? 1:0 )

/******************************************************************************
  * RESETS management
  ******************************************************************************/

/* Reset force macros */
#define __HAL_RCC_SERC_FORCE_RESET() SET_BIT(RCC->SERCCFGR, RCC_SERCCFGR_SERCRST)
#define __HAL_RCC_OSPIIOM_FORCE_RESET() SET_BIT(RCC->OSPIIOMCFGR, RCC_OSPIIOMCFGR_OSPIIOMRST)
#define __HAL_RCC_ETHSW_FORCE_RESET() SET_BIT(RCC->ETHSWCFGR, RCC_ETHSWCFGR_ETHSWRST)
#define __HAL_RCC_VDEC_FORCE_RESET() SET_BIT(RCC->VDECCFGR, RCC_VDECCFGR_VDECRST)
#define __HAL_RCC_VENC_FORCE_RESET() SET_BIT(RCC->VENCCFGR, RCC_VENCCFGR_VENCRST)
#define __HAL_RCC_C3_FORCE_RESET() SET_BIT(RCC->C3CFGR, RCC_C3CFGR_C3RST)
#define __HAL_RCC_OSPI1_FORCE_RESET() SET_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OSPI1RST)
#define __HAL_RCC_OSPI2_FORCE_RESET() SET_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OSPI2RST)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_OTFDEC1_FORCE_RESET() SET_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OTFDEC1RST)
#define __HAL_RCC_OTFDEC2_FORCE_RESET() SET_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OTFDEC2RST)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_GPIOA_FORCE_RESET() SET_BIT(RCC->GPIOACFGR, RCC_GPIOACFGR_GPIOxRST)
#define __HAL_RCC_GPIOB_FORCE_RESET() SET_BIT(RCC->GPIOBCFGR, RCC_GPIOBCFGR_GPIOxRST)
#define __HAL_RCC_GPIOC_FORCE_RESET() SET_BIT(RCC->GPIOCCFGR, RCC_GPIOCCFGR_GPIOxRST)
#define __HAL_RCC_GPIOD_FORCE_RESET() SET_BIT(RCC->GPIODCFGR, RCC_GPIODCFGR_GPIOxRST)
#define __HAL_RCC_GPIOE_FORCE_RESET() SET_BIT(RCC->GPIOECFGR, RCC_GPIOECFGR_GPIOxRST)
#define __HAL_RCC_GPIOF_FORCE_RESET() SET_BIT(RCC->GPIOFCFGR, RCC_GPIOFCFGR_GPIOxRST)
#define __HAL_RCC_GPIOG_FORCE_RESET() SET_BIT(RCC->GPIOGCFGR, RCC_GPIOGCFGR_GPIOxRST)
#define __HAL_RCC_GPIOH_FORCE_RESET() SET_BIT(RCC->GPIOHCFGR, RCC_GPIOHCFGR_GPIOxRST)
#define __HAL_RCC_GPIOI_FORCE_RESET() SET_BIT(RCC->GPIOICFGR, RCC_GPIOICFGR_GPIOxRST)
#define __HAL_RCC_GPIOJ_FORCE_RESET() SET_BIT(RCC->GPIOJCFGR, RCC_GPIOJCFGR_GPIOxRST)
#define __HAL_RCC_GPIOK_FORCE_RESET() SET_BIT(RCC->GPIOKCFGR, RCC_GPIOKCFGR_GPIOxRST)
#define __HAL_RCC_GPIOZ_FORCE_RESET() SET_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZRST)
#define __HAL_RCC_HPDMA1_FORCE_RESET() SET_BIT(RCC->HPDMA1CFGR, RCC_HPDMA1CFGR_HPDMA1RST)
#define __HAL_RCC_HPDMA2_FORCE_RESET() SET_BIT(RCC->HPDMA2CFGR, RCC_HPDMA2CFGR_HPDMA2RST)
#define __HAL_RCC_HPDMA3_FORCE_RESET() SET_BIT(RCC->HPDMA3CFGR, RCC_HPDMA3CFGR_HPDMA3RST)
#define __HAL_RCC_LPDMA_FORCE_RESET() SET_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMARST)
#define __HAL_RCC_HSEM_FORCE_RESET() SET_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMRST)
#define __HAL_RCC_IPCC1_FORCE_RESET() SET_BIT(RCC->IPCC1CFGR, RCC_IPCC1CFGR_IPCC1RST)
#define __HAL_RCC_IPCC2_FORCE_RESET() SET_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2RST)
#define __HAL_RCC_IS2M_FORCE_RESET() SET_BIT(RCC->IS2MCFGR, RCC_IS2MCFGR_IS2MRST)
#define __HAL_RCC_TIM1_FORCE_RESET() SET_BIT(RCC->TIM1CFGR, RCC_TIM1CFGR_TIM1RST)
#define __HAL_RCC_TIM2_FORCE_RESET() SET_BIT(RCC->TIM2CFGR, RCC_TIM2CFGR_TIM2RST)
#define __HAL_RCC_TIM3_FORCE_RESET() SET_BIT(RCC->TIM3CFGR, RCC_TIM3CFGR_TIM3RST)
#define __HAL_RCC_TIM4_FORCE_RESET() SET_BIT(RCC->TIM4CFGR, RCC_TIM4CFGR_TIM4RST)
#define __HAL_RCC_TIM5_FORCE_RESET() SET_BIT(RCC->TIM5CFGR, RCC_TIM5CFGR_TIM5RST)
#define __HAL_RCC_TIM6_FORCE_RESET() SET_BIT(RCC->TIM6CFGR, RCC_TIM6CFGR_TIM6RST)
#define __HAL_RCC_TIM7_FORCE_RESET() SET_BIT(RCC->TIM7CFGR, RCC_TIM7CFGR_TIM7RST)
#define __HAL_RCC_TIM8_FORCE_RESET() SET_BIT(RCC->TIM8CFGR, RCC_TIM8CFGR_TIM8RST)
#define __HAL_RCC_TIM10_FORCE_RESET() SET_BIT(RCC->TIM10CFGR, RCC_TIM10CFGR_TIM10RST)
#define __HAL_RCC_TIM11_FORCE_RESET() SET_BIT(RCC->TIM11CFGR, RCC_TIM11CFGR_TIM11RST)
#define __HAL_RCC_TIM12_FORCE_RESET() SET_BIT(RCC->TIM12CFGR, RCC_TIM12CFGR_TIM12RST)
#define __HAL_RCC_TIM13_FORCE_RESET() SET_BIT(RCC->TIM13CFGR, RCC_TIM13CFGR_TIM13RST)
#define __HAL_RCC_TIM14_FORCE_RESET() SET_BIT(RCC->TIM14CFGR, RCC_TIM14CFGR_TIM14RST)
#define __HAL_RCC_TIM15_FORCE_RESET() SET_BIT(RCC->TIM15CFGR, RCC_TIM15CFGR_TIM15RST)
#define __HAL_RCC_TIM16_FORCE_RESET() SET_BIT(RCC->TIM16CFGR, RCC_TIM16CFGR_TIM16RST)
#define __HAL_RCC_TIM17_FORCE_RESET() SET_BIT(RCC->TIM17CFGR, RCC_TIM17CFGR_TIM17RST)
#define __HAL_RCC_TIM20_FORCE_RESET() SET_BIT(RCC->TIM20CFGR, RCC_TIM20CFGR_TIM20RST)
#define __HAL_RCC_LPTIM1_FORCE_RESET() SET_BIT(RCC->LPTIM1CFGR, RCC_LPTIM1CFGR_LPTIM1RST)
#define __HAL_RCC_LPTIM2_FORCE_RESET() SET_BIT(RCC->LPTIM2CFGR, RCC_LPTIM2CFGR_LPTIM2RST)
#define __HAL_RCC_LPTIM3_FORCE_RESET() SET_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3RST)
#define __HAL_RCC_LPTIM4_FORCE_RESET() SET_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4RST)
#define __HAL_RCC_LPTIM5_FORCE_RESET() SET_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5RST)
#define __HAL_RCC_SPI1_FORCE_RESET() SET_BIT(RCC->SPI1CFGR, RCC_SPI1CFGR_SPI1RST)
#define __HAL_RCC_SPI2_FORCE_RESET() SET_BIT(RCC->SPI2CFGR, RCC_SPI2CFGR_SPI2RST)
#define __HAL_RCC_SPI3_FORCE_RESET() SET_BIT(RCC->SPI3CFGR, RCC_SPI3CFGR_SPI3RST)
#define __HAL_RCC_SPI4_FORCE_RESET() SET_BIT(RCC->SPI4CFGR, RCC_SPI4CFGR_SPI4RST)
#define __HAL_RCC_SPI5_FORCE_RESET() SET_BIT(RCC->SPI5CFGR, RCC_SPI5CFGR_SPI5RST)
#define __HAL_RCC_SPI6_FORCE_RESET() SET_BIT(RCC->SPI6CFGR, RCC_SPI6CFGR_SPI6RST)
#define __HAL_RCC_SPI7_FORCE_RESET() SET_BIT(RCC->SPI7CFGR, RCC_SPI7CFGR_SPI7RST)
#define __HAL_RCC_SPI8_FORCE_RESET() SET_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8RST)
#define __HAL_RCC_SPDIFRX_FORCE_RESET() SET_BIT(RCC->SPDIFRXCFGR, RCC_SPDIFRXCFGR_SPDIFRXRST)
#define __HAL_RCC_USART1_FORCE_RESET() SET_BIT(RCC->USART1CFGR, RCC_USART1CFGR_USART1RST)
#define __HAL_RCC_USART2_FORCE_RESET() SET_BIT(RCC->USART2CFGR, RCC_USART2CFGR_USART2RST)
#define __HAL_RCC_USART3_FORCE_RESET() SET_BIT(RCC->USART3CFGR, RCC_USART3CFGR_USART3RST)
#define __HAL_RCC_UART4_FORCE_RESET() SET_BIT(RCC->UART4CFGR, RCC_UART4CFGR_UART4RST)
#define __HAL_RCC_UART5_FORCE_RESET() SET_BIT(RCC->UART5CFGR, RCC_UART5CFGR_UART5RST)
#define __HAL_RCC_USART6_FORCE_RESET() SET_BIT(RCC->USART6CFGR, RCC_USART6CFGR_USART6RST)
#define __HAL_RCC_UART7_FORCE_RESET() SET_BIT(RCC->UART7CFGR, RCC_UART7CFGR_UART7RST)
#define __HAL_RCC_UART8_FORCE_RESET() SET_BIT(RCC->UART8CFGR, RCC_UART8CFGR_UART8RST)
#define __HAL_RCC_UART9_FORCE_RESET() SET_BIT(RCC->UART9CFGR, RCC_UART9CFGR_UART9RST)
#define __HAL_RCC_LPUART1_FORCE_RESET() SET_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1RST)
#define __HAL_RCC_I2C1_FORCE_RESET() SET_BIT(RCC->I2C1CFGR, RCC_I2C1CFGR_I2C1RST)
#define __HAL_RCC_I2C2_FORCE_RESET() SET_BIT(RCC->I2C2CFGR, RCC_I2C2CFGR_I2C2RST)
#define __HAL_RCC_I2C3_FORCE_RESET() SET_BIT(RCC->I2C3CFGR, RCC_I2C3CFGR_I2C3RST)
#define __HAL_RCC_I2C4_FORCE_RESET() SET_BIT(RCC->I2C4CFGR, RCC_I2C4CFGR_I2C4RST)
#define __HAL_RCC_I2C5_FORCE_RESET() SET_BIT(RCC->I2C5CFGR, RCC_I2C5CFGR_I2C5RST)
#define __HAL_RCC_I2C6_FORCE_RESET() SET_BIT(RCC->I2C6CFGR, RCC_I2C6CFGR_I2C6RST)
#define __HAL_RCC_I2C7_FORCE_RESET() SET_BIT(RCC->I2C7CFGR, RCC_I2C7CFGR_I2C7RST)
#define __HAL_RCC_I2C8_FORCE_RESET() SET_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8RST)
#define __HAL_RCC_SAI1_FORCE_RESET() SET_BIT(RCC->SAI1CFGR, RCC_SAI1CFGR_SAI1RST)
#define __HAL_RCC_SAI2_FORCE_RESET() SET_BIT(RCC->SAI2CFGR, RCC_SAI2CFGR_SAI2RST)
#define __HAL_RCC_SAI3_FORCE_RESET() SET_BIT(RCC->SAI3CFGR, RCC_SAI3CFGR_SAI3RST)
#define __HAL_RCC_SAI4_FORCE_RESET() SET_BIT(RCC->SAI4CFGR, RCC_SAI4CFGR_SAI4RST)
#define __HAL_RCC_MDF1_FORCE_RESET() SET_BIT(RCC->MDF1CFGR, RCC_MDF1CFGR_MDF1RST)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_FORCE_RESET() SET_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1RST)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_FORCE_RESET() SET_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2RST)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_FDCAN_FORCE_RESET() SET_BIT(RCC->FDCANCFGR, RCC_FDCANCFGR_FDCANRST)
#define __HAL_RCC_HDP_FORCE_RESET() SET_BIT(RCC->HDPCFGR, RCC_HDPCFGR_HDPRST)
#define __HAL_RCC_ADC12_FORCE_RESET() SET_BIT(RCC->ADC12CFGR, RCC_ADC12CFGR_ADC12RST)
#define __HAL_RCC_ADC3_FORCE_RESET() SET_BIT(RCC->ADC3CFGR, RCC_ADC3CFGR_ADC3RST)
#define __HAL_RCC_ETH1_FORCE_RESET() SET_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1RST)
#define __HAL_RCC_ETH2_FORCE_RESET() SET_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2RST)
#define __HAL_RCC_USB2_FORCE_RESET() SET_BIT(RCC->USB2CFGR, RCC_USB2CFGR_USB2RST)
#define __HAL_RCC_USB2PHY1_FORCE_RESET() SET_BIT(RCC->USB2PHY1CFGR, RCC_USB2PHY1CFGR_USB2PHY1RST)
#define __HAL_RCC_USB2PHY2_FORCE_RESET() SET_BIT(RCC->USB2PHY2CFGR, RCC_USB2PHY2CFGR_USB2PHY2RST)
#define __HAL_RCC_USB3DRD_FORCE_RESET() SET_BIT(RCC->USB3DRDCFGR, RCC_USB3DRDCFGR_USB3DRDRST)
#define __HAL_RCC_USB3PCIEPHY_FORCE_RESET() SET_BIT(RCC->USB3PCIEPHYCFGR, RCC_USB3PCIEPHYCFGR_USB3PCIEPHYRST)
#define __HAL_RCC_PCIE_FORCE_RESET() SET_BIT(RCC->PCIECFGR, RCC_PCIECFGR_PCIERST)
#define __HAL_RCC_UCPD1_FORCE_RESET() SET_BIT(RCC->UCPD1CFGR, RCC_UCPD1CFGR_UCPD1RST)
#define __HAL_RCC_FMC_FORCE_RESET() SET_BIT(RCC->FMCCFGR, RCC_FMCCFGR_FMCRST)
#define __HAL_RCC_SDMMC1_FORCE_RESET() SET_BIT(RCC->SDMMC1CFGR, RCC_SDMMC1CFGR_SDMMC1RST)
#define __HAL_RCC_SDMMC2_FORCE_RESET() SET_BIT(RCC->SDMMC2CFGR, RCC_SDMMC2CFGR_SDMMC2RST)
#define __HAL_RCC_SDMMC3_FORCE_RESET() SET_BIT(RCC->SDMMC3CFGR, RCC_SDMMC3CFGR_SDMMC3RST)
#define __HAL_RCC_GPU_FORCE_RESET() SET_BIT(RCC->GPUCFGR, RCC_GPUCFGR_GPURST)
#define __HAL_RCC_LTDC_FORCE_RESET() SET_BIT(RCC->LTDCCFGR, RCC_LTDCCFGR_LTDCRST)
#define __HAL_RCC_DSI_FORCE_RESET() SET_BIT(RCC->DSICFGR, RCC_DSICFGR_DSIRST)
#define __HAL_RCC_LVDS_FORCE_RESET() SET_BIT(RCC->LVDSCFGR, RCC_LVDSCFGR_LVDSRST)
#define __HAL_RCC_CSI2_FORCE_RESET() SET_BIT(RCC->CSI2CFGR, RCC_CSI2CFGR_CSI2RST)
#define __HAL_RCC_DCMIPP_FORCE_RESET() SET_BIT(RCC->DCMIPPCFGR, RCC_DCMIPPCFGR_DCMIPPRST)
#define __HAL_RCC_CCI_FORCE_RESET() SET_BIT(RCC->CCICFGR, RCC_CCICFGR_CCIRST)
#define __HAL_RCC_RNG_FORCE_RESET() SET_BIT(RCC->RNGCFGR, RCC_RNGCFGR_RNGRST)
#define __HAL_RCC_PKA_FORCE_RESET() SET_BIT(RCC->PKACFGR, RCC_PKACFGR_PKARST)
#define __HAL_RCC_SAES_FORCE_RESET() SET_BIT(RCC->SAESCFGR, RCC_SAESCFGR_SAESRST)
#define __HAL_RCC_HASH_FORCE_RESET() SET_BIT(RCC->HASHCFGR, RCC_HASHCFGR_HASHRST)
#define __HAL_RCC_CRYP1_FORCE_RESET() SET_BIT(RCC->CRYP1CFGR, RCC_CRYP1CFGR_CRYP1RST)
#define __HAL_RCC_CRYP2_FORCE_RESET() SET_BIT(RCC->CRYP2CFGR, RCC_CRYP2CFGR_CRYP2RST)
#define __HAL_RCC_WWDG1_FORCE_RESET() SET_BIT(RCC->WWDG1CFGR, RCC_WWDG1CFGR_WWDG1RST)
#define __HAL_RCC_WWDG2_FORCE_RESET() SET_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2RST)
#define __HAL_RCC_BUSPERFM_FORCE_RESET() SET_BIT(RCC->BUSPERFMCFGR, RCC_BUSPERFMCFGR_BUSPERFMRST)
#define __HAL_RCC_VREF_FORCE_RESET() SET_BIT(RCC->VREFCFGR, RCC_VREFCFGR_VREFRST)
#define __HAL_RCC_TMPSENS_FORCE_RESET() SET_BIT(RCC->TMPSENSCFGR, RCC_TMPSENSCFGR_TMPSENSRST)
#define __HAL_RCC_CRC_FORCE_RESET() SET_BIT(RCC->CRCCFGR, RCC_CRCCFGR_CRCRST)
#define __HAL_RCC_I3C1_FORCE_RESET() SET_BIT(RCC->I3C1CFGR, RCC_I3C1CFGR_I3C1RST)
#define __HAL_RCC_I3C2_FORCE_RESET() SET_BIT(RCC->I3C2CFGR, RCC_I3C2CFGR_I3C2RST)
#define __HAL_RCC_I3C3_FORCE_RESET() SET_BIT(RCC->I3C3CFGR, RCC_I3C3CFGR_I3C3RST)
#define __HAL_RCC_I3C4_FORCE_RESET() SET_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4RST)

/* Reset release macros */
#define __HAL_RCC_SERC_RELEASE_RESET() CLEAR_BIT(RCC->SERCCFGR, RCC_SERCCFGR_SERCRST)
#define __HAL_RCC_OSPIIOM_RELEASE_RESET() CLEAR_BIT(RCC->OSPIIOMCFGR, RCC_OSPIIOMCFGR_OSPIIOMRST)
#define __HAL_RCC_ETHSW_RELEASE_RESET() CLEAR_BIT(RCC->ETHSWCFGR, RCC_ETHSWCFGR_ETHSWRST)
#define __HAL_RCC_VDEC_RELEASE_RESET() CLEAR_BIT(RCC->VDECCFGR, RCC_VDECCFGR_VDECRST)
#define __HAL_RCC_VENC_RELEASE_RESET() CLEAR_BIT(RCC->VENCCFGR, RCC_VENCCFGR_VENCRST)
#define __HAL_RCC_C3_RELEASE_RESET() CLEAR_BIT(RCC->C3CFGR, RCC_C3CFGR_C3RST)
#define __HAL_RCC_OSPI1_RELEASE_RESET() CLEAR_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OSPI1RST)
#define __HAL_RCC_OSPI2_RELEASE_RESET() CLEAR_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OSPI2RST)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_OTFDEC1_RELEASE_RESET() CLEAR_BIT(RCC->OSPI1CFGR, RCC_OSPI1CFGR_OTFDEC1RST)
#define __HAL_RCC_OTFDEC2_RELEASE_RESET() CLEAR_BIT(RCC->OSPI2CFGR, RCC_OSPI2CFGR_OTFDEC2RST)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_GPIOA_RELEASE_RESET() CLEAR_BIT(RCC->GPIOACFGR, RCC_GPIOACFGR_GPIOxRST)
#define __HAL_RCC_GPIOB_RELEASE_RESET() CLEAR_BIT(RCC->GPIOBCFGR, RCC_GPIOBCFGR_GPIOxRST)
#define __HAL_RCC_GPIOC_RELEASE_RESET() CLEAR_BIT(RCC->GPIOCCFGR, RCC_GPIOCCFGR_GPIOxRST)
#define __HAL_RCC_GPIOD_RELEASE_RESET() CLEAR_BIT(RCC->GPIODCFGR, RCC_GPIODCFGR_GPIOxRST)
#define __HAL_RCC_GPIOE_RELEASE_RESET() CLEAR_BIT(RCC->GPIOECFGR, RCC_GPIOECFGR_GPIOxRST)
#define __HAL_RCC_GPIOF_RELEASE_RESET() CLEAR_BIT(RCC->GPIOFCFGR, RCC_GPIOFCFGR_GPIOxRST)
#define __HAL_RCC_GPIOG_RELEASE_RESET() CLEAR_BIT(RCC->GPIOGCFGR, RCC_GPIOGCFGR_GPIOxRST)
#define __HAL_RCC_GPIOH_RELEASE_RESET() CLEAR_BIT(RCC->GPIOHCFGR, RCC_GPIOHCFGR_GPIOxRST)
#define __HAL_RCC_GPIOI_RELEASE_RESET() CLEAR_BIT(RCC->GPIOICFGR, RCC_GPIOICFGR_GPIOxRST)
#define __HAL_RCC_GPIOJ_RELEASE_RESET() CLEAR_BIT(RCC->GPIOJCFGR, RCC_GPIOJCFGR_GPIOxRST)
#define __HAL_RCC_GPIOK_RELEASE_RESET() CLEAR_BIT(RCC->GPIOKCFGR, RCC_GPIOKCFGR_GPIOxRST)
#define __HAL_RCC_GPIOZ_RELEASE_RESET() CLEAR_BIT(RCC->GPIOZCFGR, RCC_GPIOZCFGR_GPIOZRST)
#define __HAL_RCC_HPDMA1_RELEASE_RESET() CLEAR_BIT(RCC->HPDMA1CFGR, RCC_HPDMA1CFGR_HPDMA1RST)
#define __HAL_RCC_HPDMA2_RELEASE_RESET() CLEAR_BIT(RCC->HPDMA2CFGR, RCC_HPDMA2CFGR_HPDMA2RST)
#define __HAL_RCC_HPDMA3_RELEASE_RESET() CLEAR_BIT(RCC->HPDMA3CFGR, RCC_HPDMA3CFGR_HPDMA3RST)
#define __HAL_RCC_LPDMA_RELEASE_RESET() CLEAR_BIT(RCC->LPDMACFGR, RCC_LPDMACFGR_LPDMARST)
#define __HAL_RCC_HSEM_RELEASE_RESET() CLEAR_BIT(RCC->HSEMCFGR, RCC_HSEMCFGR_HSEMRST)
#define __HAL_RCC_IPCC1_RELEASE_RESET() CLEAR_BIT(RCC->IPCC1CFGR, RCC_IPCC1CFGR_IPCC1RST)
#define __HAL_RCC_IPCC2_RELEASE_RESET() CLEAR_BIT(RCC->IPCC2CFGR, RCC_IPCC2CFGR_IPCC2RST)
#define __HAL_RCC_IS2M_RELEASE_RESET() CLEAR_BIT(RCC->IS2MCFGR, RCC_IS2MCFGR_IS2MRST)
#define __HAL_RCC_TIM1_RELEASE_RESET() CLEAR_BIT(RCC->TIM1CFGR, RCC_TIM1CFGR_TIM1RST)
#define __HAL_RCC_TIM2_RELEASE_RESET() CLEAR_BIT(RCC->TIM2CFGR, RCC_TIM2CFGR_TIM2RST)
#define __HAL_RCC_TIM3_RELEASE_RESET() CLEAR_BIT(RCC->TIM3CFGR, RCC_TIM3CFGR_TIM3RST)
#define __HAL_RCC_TIM4_RELEASE_RESET() CLEAR_BIT(RCC->TIM4CFGR, RCC_TIM4CFGR_TIM4RST)
#define __HAL_RCC_TIM5_RELEASE_RESET() CLEAR_BIT(RCC->TIM5CFGR, RCC_TIM5CFGR_TIM5RST)
#define __HAL_RCC_TIM6_RELEASE_RESET() CLEAR_BIT(RCC->TIM6CFGR, RCC_TIM6CFGR_TIM6RST)
#define __HAL_RCC_TIM7_RELEASE_RESET() CLEAR_BIT(RCC->TIM7CFGR, RCC_TIM7CFGR_TIM7RST)
#define __HAL_RCC_TIM8_RELEASE_RESET() CLEAR_BIT(RCC->TIM8CFGR, RCC_TIM8CFGR_TIM8RST)
#define __HAL_RCC_TIM10_RELEASE_RESET() CLEAR_BIT(RCC->TIM10CFGR, RCC_TIM10CFGR_TIM10RST)
#define __HAL_RCC_TIM11_RELEASE_RESET() CLEAR_BIT(RCC->TIM11CFGR, RCC_TIM11CFGR_TIM11RST)
#define __HAL_RCC_TIM12_RELEASE_RESET() CLEAR_BIT(RCC->TIM12CFGR, RCC_TIM12CFGR_TIM12RST)
#define __HAL_RCC_TIM13_RELEASE_RESET() CLEAR_BIT(RCC->TIM13CFGR, RCC_TIM13CFGR_TIM13RST)
#define __HAL_RCC_TIM14_RELEASE_RESET() CLEAR_BIT(RCC->TIM14CFGR, RCC_TIM14CFGR_TIM14RST)
#define __HAL_RCC_TIM15_RELEASE_RESET() CLEAR_BIT(RCC->TIM15CFGR, RCC_TIM15CFGR_TIM15RST)
#define __HAL_RCC_TIM16_RELEASE_RESET() CLEAR_BIT(RCC->TIM16CFGR, RCC_TIM16CFGR_TIM16RST)
#define __HAL_RCC_TIM17_RELEASE_RESET() CLEAR_BIT(RCC->TIM17CFGR, RCC_TIM17CFGR_TIM17RST)
#define __HAL_RCC_TIM20_RELEASE_RESET() CLEAR_BIT(RCC->TIM20CFGR, RCC_TIM20CFGR_TIM20RST)
#define __HAL_RCC_LPTIM1_RELEASE_RESET() CLEAR_BIT(RCC->LPTIM1CFGR, RCC_LPTIM1CFGR_LPTIM1RST)
#define __HAL_RCC_LPTIM2_RELEASE_RESET() CLEAR_BIT(RCC->LPTIM2CFGR, RCC_LPTIM2CFGR_LPTIM2RST)
#define __HAL_RCC_LPTIM3_RELEASE_RESET() CLEAR_BIT(RCC->LPTIM3CFGR, RCC_LPTIM3CFGR_LPTIM3RST)
#define __HAL_RCC_LPTIM4_RELEASE_RESET() CLEAR_BIT(RCC->LPTIM4CFGR, RCC_LPTIM4CFGR_LPTIM4RST)
#define __HAL_RCC_LPTIM5_RELEASE_RESET() CLEAR_BIT(RCC->LPTIM5CFGR, RCC_LPTIM5CFGR_LPTIM5RST)
#define __HAL_RCC_SPI1_RELEASE_RESET() CLEAR_BIT(RCC->SPI1CFGR, RCC_SPI1CFGR_SPI1RST)
#define __HAL_RCC_SPI2_RELEASE_RESET() CLEAR_BIT(RCC->SPI2CFGR, RCC_SPI2CFGR_SPI2RST)
#define __HAL_RCC_SPI3_RELEASE_RESET() CLEAR_BIT(RCC->SPI3CFGR, RCC_SPI3CFGR_SPI3RST)
#define __HAL_RCC_SPI4_RELEASE_RESET() CLEAR_BIT(RCC->SPI4CFGR, RCC_SPI4CFGR_SPI4RST)
#define __HAL_RCC_SPI5_RELEASE_RESET() CLEAR_BIT(RCC->SPI5CFGR, RCC_SPI5CFGR_SPI5RST)
#define __HAL_RCC_SPI6_RELEASE_RESET() CLEAR_BIT(RCC->SPI6CFGR, RCC_SPI6CFGR_SPI6RST)
#define __HAL_RCC_SPI7_RELEASE_RESET() CLEAR_BIT(RCC->SPI7CFGR, RCC_SPI7CFGR_SPI7RST)
#define __HAL_RCC_SPI8_RELEASE_RESET() CLEAR_BIT(RCC->SPI8CFGR, RCC_SPI8CFGR_SPI8RST)
#define __HAL_RCC_SPDIFRX_RELEASE_RESET() CLEAR_BIT(RCC->SPDIFRXCFGR, RCC_SPDIFRXCFGR_SPDIFRXRST)
#define __HAL_RCC_USART1_RELEASE_RESET() CLEAR_BIT(RCC->USART1CFGR, RCC_USART1CFGR_USART1RST)
#define __HAL_RCC_USART2_RELEASE_RESET() CLEAR_BIT(RCC->USART2CFGR, RCC_USART2CFGR_USART2RST)
#define __HAL_RCC_USART3_RELEASE_RESET() CLEAR_BIT(RCC->USART3CFGR, RCC_USART3CFGR_USART3RST)
#define __HAL_RCC_UART4_RELEASE_RESET() CLEAR_BIT(RCC->UART4CFGR, RCC_UART4CFGR_UART4RST)
#define __HAL_RCC_UART5_RELEASE_RESET() CLEAR_BIT(RCC->UART5CFGR, RCC_UART5CFGR_UART5RST)
#define __HAL_RCC_USART6_RELEASE_RESET() CLEAR_BIT(RCC->USART6CFGR, RCC_USART6CFGR_USART6RST)
#define __HAL_RCC_UART7_RELEASE_RESET() CLEAR_BIT(RCC->UART7CFGR, RCC_UART7CFGR_UART7RST)
#define __HAL_RCC_UART8_RELEASE_RESET() CLEAR_BIT(RCC->UART8CFGR, RCC_UART8CFGR_UART8RST)
#define __HAL_RCC_UART9_RELEASE_RESET() CLEAR_BIT(RCC->UART9CFGR, RCC_UART9CFGR_UART9RST)
#define __HAL_RCC_LPUART1_RELEASE_RESET() CLEAR_BIT(RCC->LPUART1CFGR, RCC_LPUART1CFGR_LPUART1RST)
#define __HAL_RCC_I2C1_RELEASE_RESET() CLEAR_BIT(RCC->I2C1CFGR, RCC_I2C1CFGR_I2C1RST)
#define __HAL_RCC_I2C2_RELEASE_RESET() CLEAR_BIT(RCC->I2C2CFGR, RCC_I2C2CFGR_I2C2RST)
#define __HAL_RCC_I2C3_RELEASE_RESET() CLEAR_BIT(RCC->I2C3CFGR, RCC_I2C3CFGR_I2C3RST)
#define __HAL_RCC_I2C4_RELEASE_RESET() CLEAR_BIT(RCC->I2C4CFGR, RCC_I2C4CFGR_I2C4RST)
#define __HAL_RCC_I2C5_RELEASE_RESET() CLEAR_BIT(RCC->I2C5CFGR, RCC_I2C5CFGR_I2C5RST)
#define __HAL_RCC_I2C6_RELEASE_RESET() CLEAR_BIT(RCC->I2C6CFGR, RCC_I2C6CFGR_I2C6RST)
#define __HAL_RCC_I2C7_RELEASE_RESET() CLEAR_BIT(RCC->I2C7CFGR, RCC_I2C7CFGR_I2C7RST)
#define __HAL_RCC_I2C8_RELEASE_RESET() CLEAR_BIT(RCC->I2C8CFGR, RCC_I2C8CFGR_I2C8RST)
#define __HAL_RCC_SAI1_RELEASE_RESET() CLEAR_BIT(RCC->SAI1CFGR, RCC_SAI1CFGR_SAI1RST)
#define __HAL_RCC_SAI2_RELEASE_RESET() CLEAR_BIT(RCC->SAI2CFGR, RCC_SAI2CFGR_SAI2RST)
#define __HAL_RCC_SAI3_RELEASE_RESET() CLEAR_BIT(RCC->SAI3CFGR, RCC_SAI3CFGR_SAI3RST)
#define __HAL_RCC_SAI4_RELEASE_RESET() CLEAR_BIT(RCC->SAI4CFGR, RCC_SAI4CFGR_SAI4RST)
#define __HAL_RCC_MDF1_RELEASE_RESET() CLEAR_BIT(RCC->MDF1CFGR, RCC_MDF1CFGR_MDF1RST)
#if ! defined(STM32MP2XX_ASSY3_1_0)
#define __HAL_RCC_ADF1_RELEASE_RESET() CLEAR_BIT(RCC->ADF1CFGR, RCC_ADF1CFGR_ADF1RST)
#else /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_MDF2_RELEASE_RESET() CLEAR_BIT(RCC->MDF2CFGR, RCC_MDF2CFGR_MDF2RST)
#endif /* ! defined(STM32MP2XX_ASSY3_1_0) */
#define __HAL_RCC_FDCAN_RELEASE_RESET() CLEAR_BIT(RCC->FDCANCFGR, RCC_FDCANCFGR_FDCANRST)
#define __HAL_RCC_HDP_RELEASE_RESET() CLEAR_BIT(RCC->HDPCFGR, RCC_HDPCFGR_HDPRST)
#define __HAL_RCC_ADC12_RELEASE_RESET() CLEAR_BIT(RCC->ADC12CFGR, RCC_ADC12CFGR_ADC12RST)
#define __HAL_RCC_ADC3_RELEASE_RESET() CLEAR_BIT(RCC->ADC3CFGR, RCC_ADC3CFGR_ADC3RST)
#define __HAL_RCC_ETH1_RELEASE_RESET() CLEAR_BIT(RCC->ETH1CFGR, RCC_ETH1CFGR_ETH1RST)
#define __HAL_RCC_ETH2_RELEASE_RESET() CLEAR_BIT(RCC->ETH2CFGR, RCC_ETH2CFGR_ETH2RST)
#define __HAL_RCC_USB2_RELEASE_RESET() CLEAR_BIT(RCC->USB2CFGR, RCC_USB2CFGR_USB2RST)
#define __HAL_RCC_USB2PHY1_RELEASE_RESET() CLEAR_BIT(RCC->USB2PHY1CFGR, RCC_USB2PHY1CFGR_USB2PHY1RST)
#define __HAL_RCC_USB2PHY2_RELEASE_RESET() CLEAR_BIT(RCC->USB2PHY2CFGR, RCC_USB2PHY2CFGR_USB2PHY2RST)
#define __HAL_RCC_USB3DRD_RELEASE_RESET() CLEAR_BIT(RCC->USB3DRDCFGR, RCC_USB3DRDCFGR_USB3DRDRST)
#define __HAL_RCC_USB3PCIEPHY_RELEASE_RESET() CLEAR_BIT(RCC->USB3PCIEPHYCFGR, RCC_USB3PCIEPHYCFGR_USB3PCIEPHYRST)
#define __HAL_RCC_PCIE_RELEASE_RESET() CLEAR_BIT(RCC->PCIECFGR, RCC_PCIECFGR_PCIERST)
#define __HAL_RCC_UCPD1_RELEASE_RESET() CLEAR_BIT(RCC->UCPD1CFGR, RCC_UCPD1CFGR_UCPD1RST)
#define __HAL_RCC_FMC_RELEASE_RESET() CLEAR_BIT(RCC->FMCCFGR, RCC_FMCCFGR_FMCRST)
#define __HAL_RCC_SDMMC1_RELEASE_RESET() CLEAR_BIT(RCC->SDMMC1CFGR, RCC_SDMMC1CFGR_SDMMC1RST)
#define __HAL_RCC_SDMMC2_RELEASE_RESET() CLEAR_BIT(RCC->SDMMC2CFGR, RCC_SDMMC2CFGR_SDMMC2RST)
#define __HAL_RCC_SDMMC3_RELEASE_RESET() CLEAR_BIT(RCC->SDMMC3CFGR, RCC_SDMMC3CFGR_SDMMC3RST)
#define __HAL_RCC_GPU_RELEASE_RESET() CLEAR_BIT(RCC->GPUCFGR, RCC_GPUCFGR_GPURST)
#define __HAL_RCC_LTDC_RELEASE_RESET() CLEAR_BIT(RCC->LTDCCFGR, RCC_LTDCCFGR_LTDCRST)
#define __HAL_RCC_DSI_RELEASE_RESET() CLEAR_BIT(RCC->DSICFGR, RCC_DSICFGR_DSIRST)
#define __HAL_RCC_LVDS_RELEASE_RESET() CLEAR_BIT(RCC->LVDSCFGR, RCC_LVDSCFGR_LVDSRST)
#define __HAL_RCC_CSI2_RELEASE_RESET() CLEAR_BIT(RCC->CSI2CFGR, RCC_CSI2CFGR_CSI2RST)
#define __HAL_RCC_DCMIPP_RELEASE_RESET() CLEAR_BIT(RCC->DCMIPPCFGR, RCC_DCMIPPCFGR_DCMIPPRST)
#define __HAL_RCC_CCI_RELEASE_RESET() CLEAR_BIT(RCC->CCICFGR, RCC_CCICFGR_CCIRST)
#define __HAL_RCC_RNG_RELEASE_RESET() CLEAR_BIT(RCC->RNGCFGR, RCC_RNGCFGR_RNGRST)
#define __HAL_RCC_PKA_RELEASE_RESET() CLEAR_BIT(RCC->PKACFGR, RCC_PKACFGR_PKARST)
#define __HAL_RCC_SAES_RELEASE_RESET() CLEAR_BIT(RCC->SAESCFGR, RCC_SAESCFGR_SAESRST)
#define __HAL_RCC_HASH_RELEASE_RESET() CLEAR_BIT(RCC->HASHCFGR, RCC_HASHCFGR_HASHRST)
#define __HAL_RCC_CRYP1_RELEASE_RESET() CLEAR_BIT(RCC->CRYP1CFGR, RCC_CRYP1CFGR_CRYP1RST)
#define __HAL_RCC_CRYP2_RELEASE_RESET() CLEAR_BIT(RCC->CRYP2CFGR, RCC_CRYP2CFGR_CRYP2RST)
#define __HAL_RCC_WWDG1_RELEASE_RESET() CLEAR_BIT(RCC->WWDG1CFGR, RCC_WWDG1CFGR_WWDG1RST)
#define __HAL_RCC_WWDG2_RELEASE_RESET() CLEAR_BIT(RCC->WWDG2CFGR, RCC_WWDG2CFGR_WWDG2RST)
#define __HAL_RCC_BUSPERFM_RELEASE_RESET() CLEAR_BIT(RCC->BUSPERFMCFGR, RCC_BUSPERFMCFGR_BUSPERFMRST)
#define __HAL_RCC_VREF_RELEASE_RESET() CLEAR_BIT(RCC->VREFCFGR, RCC_VREFCFGR_VREFRST)
#define __HAL_RCC_TMPSENS_RELEASE_RESET() CLEAR_BIT(RCC->TMPSENSCFGR, RCC_TMPSENSCFGR_TMPSENSRST)
#define __HAL_RCC_CRC_RELEASE_RESET() CLEAR_BIT(RCC->CRCCFGR, RCC_CRCCFGR_CRCRST)
#define __HAL_RCC_I3C1_RELEASE_RESET() CLEAR_BIT(RCC->I3C1CFGR, RCC_I3C1CFGR_I3C1RST)
#define __HAL_RCC_I3C2_RELEASE_RESET() CLEAR_BIT(RCC->I3C2CFGR, RCC_I3C2CFGR_I3C2RST)
#define __HAL_RCC_I3C3_RELEASE_RESET() CLEAR_BIT(RCC->I3C3CFGR, RCC_I3C3CFGR_I3C3RST)
#define __HAL_RCC_I3C4_RELEASE_RESET() CLEAR_BIT(RCC->I3C4CFGR, RCC_I3C4CFGR_I3C4RST)

/* Reset flags (RCC_HWRSTSCLRR register) */
#define RCC_MASK_ALL_RESET_FLAGS                ((uint32_t)((0xf << 16) | 0x7fff))
#define RCC_FLAG_PORRST                         (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_PORRSTF)
#define RCC_FLAG_BORRST                         (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_BORRSTF)
#define RCC_FLAG_PADRST                         (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_PADRSTF)
#define RCC_FLAG_HCSSRST                        (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_HCSSRSTF)
#define RCC_FLAG_VCORERST                       (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_VCORERSTF)
#define RCC_FLAG_VCPURSTF                       (RCC->C1HWRSTSCLRR & RCC_C1HWRSTSCLRR_VCPURSTF)
#define RCC_FLAG_SYSC1RSTF                      (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_SYSC1RSTF)
#define RCC_FLAG_SYSC2RSTF                      (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_SYSC2RSTF)
#define RCC_FLAG_IWDG1RSTF                      (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_IWDG1SYSRSTF)
#define RCC_FLAG_IWDG2RSTF                      (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_IWDG2SYSRSTF)
#define RCC_FLAG_IWDG3RSTF                      (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_IWDG3SYSRSTF)
#define RCC_FLAG_IWDG4RSTF                      (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_IWDG4SYSRSTF)
#define RCC_FLAG_IWDG5RSTF                      (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_IWDG5SYSRSTF)
#define RCC_FLAG_C1RSTF                         (RCC->C1HWRSTSCLRR & RCC_C1HWRSTSCLRR_C1RSTF)
#define RCC_FLAG_C2RSTF                         (RCC->C2HWRSTSCLRR & RCC_C2HWRSTSCLRR_C2RSTF)
#define RCC_FLAG_C1P1RSTF                       (RCC->C1HWRSTSCLRR & RCC_C1HWRSTSCLRR_C1P1RSTF)
#define RCC_FLAG_RETCRCERRRSTF                  (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_RETCRCERRRSTF)
#define RCC_FLAG_RETECCFAILCRCRSTF              (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_RETECCFAILCRCRSTF)
#define RCC_FLAG_RETECCFAILRESTRSTF             (RCC->HWRSTSCLRR & RCC_HWRSTSCLRR_RETECCFAILRESTRSTF)

/** @brief  Clear all the reset flags
  * @note   Writing a '1' clears the corresponding bit to '0'
  */
#ifdef CORE_CA35
#define __HAL_RCC_CLEAR_RESET_FLAGS() \
  { WRITE_REG(RCC->HWRSTSCLRR, RCC_MASK_ALL_RESET_FLAGS); \
    WRITE_REG(RCC->C1HWRSTSCLRR, (RCC_C1HWRSTSCLRR_C1RSTF|RCC_C1HWRSTSCLRR_C1P1RSTF|RCC_C1HWRSTSCLRR_VCPURSTF)); }
#endif /* CORE_CA35 */
#ifdef CORE_CM33
#define __HAL_RCC_CLEAR_RESET_FLAGS() \
  { WRITE_REG(RCC->HWRSTSCLRR, RCC_MASK_ALL_RESET_FLAGS); \
    WRITE_REG(RCC->C2HWRSTSCLRR, RCC_C2HWRSTSCLRR_C2RSTF); }
#endif /* CORE_CM33 */

/** @brief  Macro to configure the Minimum Reset Duration
  * @note   Set and reset by software. They define the minimum guaranteed
  *         duration of the NRST low pulse. The LSI oscillator is automatically
  *         enabled when needed by the RPCTL.
  * @note   00000: NRST low pulse duration is guaranteed by the pulse stretcher
  *                of the PAD. The RPCTL is bypassed (default after reset)
  *         00010: The guaranteed NRST low pulse duration is about 2 ms (2 x 32 ck_lsi cycles),
  *         ...
  *         11111: The guaranteed NRST low pulse duration is about 31 ms (31 x 32 ck_lsi cycles).
  */
#define __HAL_RCC_MRD_CONFIG(__DURATION__) \
  do { MODIFY_REG(RCC->RDCR, RCC_RDCR_MRD, \
                    (uint32_t)(__DURATION__) << RCC_RDCR_MRD_Pos); \
    HAL_Delay(1); \
  }  while(0)

/** @brief  Macros to force or release the VSW/backup domain reset.
  * @note   WARNING: BDCR write protection is assumed to be removed first.
  * @note   All RTC registers and the RCC_BDCR register are set to their reset values.
  * @note   The BKPSRAM and CPU3 Instruction RAM are not affected.
  */
#define __HAL_RCC_BACKUPRESET_FORCE()   SET_BIT(RCC->BDCR, RCC_BDCR_VSWRST)
#define __HAL_RCC_BACKUPRESET_RELEASE() CLEAR_BIT(RCC->BDCR, RCC_BDCR_VSWRST)

/** @brief  Macro to manage resets initiated by CPU1/CPU2
  *         (through registers RCC_GRSTCSETR/RCC_C1RSTCSETR/RCC_C1P1RSTCSETR/RCC_C2RSTCSETR/RCC_CPUBOOTCR)
  *         and corresponding reset flags (To be confirmed according to their usefulness and to HAL naming rules)
  */
#define __HAL_RCC_GLOBAL_RESET_FORCE()      SET_BIT(RCC->GRSTCSETR, RCC_GRSTCSETR_SYSRST)
#define RCC_FLAG_SYSRST                     (RCC->GRSTCSETR & RCC_GRSTCSETR_SYSRST)
#define __HAL_RCC_CPU1_RESET_FORCE()        SET_BIT(RCC->C1RSTCSETR, RCC_C1RSTCSETR_C1RST)
//#define RCC_FLAG_C1RST                      (RCC->C1RSTCSETR & RCC_C1RSTCSETR_C1RST)
#define __HAL_RCC_CPU1PROC1_RESET_FORCE()   SET_BIT(RCC->C1P1RSTCSETR, RCC_C1P1RSTCSETR_C1P1RST)
//#define RCC_FLAG_C1P1RST                    (RCC->C1P1RSTCSETR & RCC_C1P1RSTCSETR_C1P1RST)
#define __HAL_RCC_CPU1PROC1_PORESET_FORCE() SET_BIT(RCC->C1P1RSTCSETR, RCC_C1P1RSTCSETR_C1P1PORRST)
//#define RCC_FLAG_C1P1PORRST                 (RCC->C1P1RSTCSETR & RCC_C1P1RSTCSETR_C1P1PORRST)
#define __HAL_RCC_CPU2_RESET_FORCE()        SET_BIT(RCC->C2RSTCSETR, RCC_C2RSTCSETR_C2RST)
//#define RCC_FLAG_C2RST                      (RCC->C2RSTCSETR & RCC_C2RSTCSETR_C2RST)
#define __HAL_RCC_CPU1_BOOT_ENABLE()        SET_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU1)
#define __HAL_RCC_CPU1_BOOT_DISABLE()       CLEAR_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU1)
#define __HAL_RCC_CPU2_BOOT_ENABLE()        SET_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU2)
#define __HAL_RCC_CPU2_BOOT_DISABLE()       CLEAR_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU2)

/** @brief  Macros to manage IWDG1 (through registers RCC_IWDGC1CFGSETR/RCC_IWDGC1CFGCLRR/RCC_IWDGC1FZSETR/RCC_IWDGC1FZCLRR)
  */
#define __HAL_RCC_IWDG1_SYSRST_ENABLE()   SET_BIT(RCC->IWDGC1CFGSETR, RCC_IWDGC1CFGSETR_IWDG1_SYSRSTEN)
#define __HAL_RCC_IWDG1_SYSRST_DISABLE()  SET_BIT(RCC->IWDGC1CFGCLRR, RCC_IWDGC1CFGCLRR_IWDG1_SYSRSTEN)
#define __HAL_RCC_IWDG1_CLK_FREEZE()      SET_BIT(RCC->IWDGC1FZSETR, RCC_IWDGC1FZSETR_FZ_IWDG1)
#define __HAL_RCC_IWDG1_CLK_UNFREEZE()    SET_BIT(RCC->IWDGC1FZCLRR, RCC_IWDGC1FZCLRR_FZ_IWDG1)

/** @brief  Macros to manage IWDG2 (through registers RCC_IWDGC1CFGSETR/RCC_IWDGC1CFGCLRR/RCC_IWDGC1FZSETR/RCC_IWDGC1FZCLRR)
  */
#define __HAL_RCC_IWDG2_SYSRST_ENABLE()   SET_BIT(RCC->IWDGC1CFGSETR, RCC_IWDGC1CFGSETR_IWDG2_SYSRSTEN)
#define __HAL_RCC_IWDG2_SYSRST_DISABLE()  SET_BIT(RCC->IWDGC1CFGCLRR, RCC_IWDGC1CFGCLRR_IWDG2_SYSRSTEN)
#define __HAL_RCC_IWDG2_KERRST_ENABLE()   SET_BIT(RCC->IWDGC1CFGSETR, RCC_IWDGC1CFGSETR_IWDG2_KERRST)
#define __HAL_RCC_IWDG2_KERRST_DISABLE()  SET_BIT(RCC->IWDGC1CFGCLRR, RCC_IWDGC1CFGCLRR_IWDG2_KERRST)
#define __HAL_RCC_IWDG2_CLK_FREEZE()      SET_BIT(RCC->IWDGC1FZSETR, RCC_IWDGC1FZSETR_FZ_IWDG2)
#define __HAL_RCC_IWDG2_CLK_UNFREEZE()    SET_BIT(RCC->IWDGC1FZCLRR, RCC_IWDGC1FZCLRR_FZ_IWDG2)

/** @brief  Macros to manage IWDG3 (through registers RCC_IWDGC2CFGSETR/RCC_IWDGC2CFGCLRR/RCC_IWDGC2FZSETR/RCC_IWDGC2FZCLRR)
  */
#define __HAL_RCC_IWDG3_SYSRST_ENABLE()   SET_BIT(RCC->IWDGC2CFGSETR, RCC_IWDGC2CFGSETR_IWDG3_SYSRSTEN)
#define __HAL_RCC_IWDG3_SYSRST_DISABLE()  SET_BIT(RCC->IWDGC2CFGCLRR, RCC_IWDGC2CFGCLRR_IWDG3_SYSRSTEN)
#define __HAL_RCC_IWDG3_CLK_FREEZE()      SET_BIT(RCC->IWDGC2FZSETR, RCC_IWDGC2FZSETR_FZ_IWDG3)
#define __HAL_RCC_IWDG3_CLK_UNFREEZE()    SET_BIT(RCC->IWDGC2FZCLRR, RCC_IWDGC2FZCLRR_FZ_IWDG3)

/** @brief  Macros to manage IWDG4 (through registers RCC_IWDGC2CFGSETR/RCC_IWDGC2CFGCLRR/RCC_IWDGC2FZSETR/RCC_IWDGC2FZCLRR)
  */
#define __HAL_RCC_IWDG4_SYSRST_ENABLE()   SET_BIT(RCC->IWDGC2CFGSETR, RCC_IWDGC2CFGSETR_IWDG4_SYSRSTEN)
#define __HAL_RCC_IWDG4_SYSRST_DISABLE()  SET_BIT(RCC->IWDGC2CFGCLRR, RCC_IWDGC2CFGCLRR_IWDG4_SYSRSTEN)
#define __HAL_RCC_IWDG4_KERRST_ENABLE()   SET_BIT(RCC->IWDGC2CFGSETR, RCC_IWDGC2CFGSETR_IWDG4_KERRST)
#define __HAL_RCC_IWDG4_KERRST_DISABLE()  SET_BIT(RCC->IWDGC2CFGCLRR, RCC_IWDGC2CFGCLRR_IWDG4_KERRST)
#define __HAL_RCC_IWDG4_CLK_FREEZE()      SET_BIT(RCC->IWDGC2FZSETR, RCC_IWDGC2FZSETR_FZ_IWDG4)
#define __HAL_RCC_IWDG4_CLK_UNFREEZE()    SET_BIT(RCC->IWDGC2FZCLRR, RCC_IWDGC2FZCLRR_FZ_IWDG4)

/** @brief  Macros to manage IWDG5 (through registers RCC_IWDGC3CFGSETR/RCC_IWDGC3CFGCLRR)
  */
#define __HAL_RCC_IWDG5_SYSRST_ENABLE()   SET_BIT(RCC->IWDGC3CFGSETR, RCC_IWDGC3CFGSETR_IWDG5_SYSRSTEN)
#define __HAL_RCC_IWDG5_SYSRST_DISABLE()  SET_BIT(RCC->IWDGC3CFGCLRR, RCC_IWDGC3CFGCLRR_IWDG5_SYSRSTEN)

/******************************************************************************
  * INTERRUPTS management
  ******************************************************************************/

/* Interrupts definition. Same for both CPU1/A35 & CPU2/M33 */
#define RCC_IT_LSIRDY       RCC_C1CIFCLRR_LSIRDYF
#define RCC_IT_LSERDY       RCC_C1CIFCLRR_LSERDYF
#define RCC_IT_HSIRDY       RCC_C1CIFCLRR_HSIRDYF
#define RCC_IT_HSERDY       RCC_C1CIFCLRR_HSERDYF
#define RCC_IT_CSIRDY       RCC_C1CIFCLRR_CSIRDYF
#define RCC_IT_PLL1RDY      RCC_C1CIFCLRR_PLL1RDYF
#define RCC_IT_PLL2RDY      RCC_C1CIFCLRR_PLL2RDYF
#define RCC_IT_PLL3RDY      RCC_C1CIFCLRR_PLL3RDYF
#define RCC_IT_PLL4RDY      RCC_C1CIFCLRR_PLL4RDYF
#define RCC_IT_PLL5RDY      RCC_C1CIFCLRR_PLL5RDYF
#define RCC_IT_PLL6RDY      RCC_C1CIFCLRR_PLL6RDYF
#define RCC_IT_PLL7RDY      RCC_C1CIFCLRR_PLL7RDYF
#define RCC_IT_PLL8RDY      RCC_C1CIFCLRR_PLL8RDYF
#define RCC_IT_LSECSS       RCC_C1CIFCLRR_LSECSSF
#define RCC_IT_WKUP         RCC_C1CIFCLRR_WKUPF

/** @brief  Enable RCC interrupt thru RCC_CxCIESETR register.
  * @param  __INTERRUPT__: specifies the RCC interrupt sources to be enabled.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_IT_LSIRDY:  LSI ready interrupt
  *            @arg RCC_IT_LSERDY:  LSE ready interrupt
  *            @arg RCC_IT_HSIRDY:  HSI ready interrupt
  *            @arg RCC_IT_HSERDY:  HSE ready interrupt
  *            @arg RCC_IT_CSIRDY:  CSI ready interrupt
  *            @arg RCC_IT_PLL1RDY: PLL1RDY ready interrupt
  *            @arg RCC_IT_PLL2RDY: PLL2RDY ready interrupt
  *            @arg RCC_IT_PLL3RDY: PLL3RDY ready interrupt
  *            @arg RCC_IT_PLL4RDY: PLL4RDY ready interrupt
  *            @arg RCC_IT_PLL5RDY: PLL5RDY ready interrupt
  *            @arg RCC_IT_PLL6RDY: PLL6RDY ready interrupt
  *            @arg RCC_IT_PLL7RDY: PLL7RDY ready interrupt
  *            @arg RCC_IT_PLL8RDY: PLL8RDY ready interrupt
  *            @arg RCC_IT_LSECSS:  LSE Clock security system interrupt
  *            @arg RCC_IT_WKUP:    Wake-up from CSTOP Interrupt
  */
#ifdef CORE_CA35
#define __HAL_RCC_ENABLE_IT(__INTERRUPT__) (RCC->C1CIESETR = (__INTERRUPT__))
#endif /* CORE_CA35 */
#ifdef CORE_CM33
#define __HAL_RCC_ENABLE_IT(__INTERRUPT__) (RCC->C2CIESETR = (__INTERRUPT__))
#endif /* CORE_CM33 */

/** @brief Disable RCC interrupt thru RCC_CxCIECLRR register.
  * @param  __INTERRUPT__: specifies the RCC interrupt sources to be disabled.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_IT_LSIRDY:  LSI ready interrupt
  *            @arg RCC_IT_LSERDY:  LSE ready interrupt
  *            @arg RCC_IT_HSIRDY:  HSI ready interrupt
  *            @arg RCC_IT_HSERDY:  HSE ready interrupt
  *            @arg RCC_IT_CSIRDY:  CSI ready interrupt
  *            @arg RCC_IT_PLL1RDY: PLL1RDY ready interrupt
  *            @arg RCC_IT_PLL2RDY: PLL2RDY ready interrupt
  *            @arg RCC_IT_PLL3RDY: PLL3RDY ready interrupt
  *            @arg RCC_IT_PLL4RDY: PLL4RDY ready interrupt
  *            @arg RCC_IT_PLL5RDY: PLL5RDY ready interrupt
  *            @arg RCC_IT_PLL6RDY: PLL6RDY ready interrupt
  *            @arg RCC_IT_PLL7RDY: PLL7RDY ready interrupt
  *            @arg RCC_IT_PLL8RDY: PLL8RDY ready interrupt
  *            @arg RCC_IT_LSECSS:  LSE Clock security system interrupt
  *            @arg RCC_IT_WKUP:    Wake-up from CSTOP Interrupt
  */

#ifdef CORE_CA35
#define __HAL_RCC_DISABLE_IT(__INTERRUPT__) CLEAR_BIT(RCC->C1CIESETR, (__INTERRUPT__))
#endif /* CORE_CA35 */
#ifdef CORE_CM33
#define __HAL_RCC_DISABLE_IT(__INTERRUPT__) CLEAR_BIT(RCC->C2CIESETR, (__INTERRUPT__))
#endif /* CORE_CM33 */

/** @brief  Clear some RCC's pending interrupts.
  * @param  __INTERRUPT__: specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_IT_LSIRDY:  LSI ready interrupt
  *            @arg RCC_IT_LSERDY:  LSE ready interrupt
  *            @arg RCC_IT_HSIRDY:  HSI ready interrupt
  *            @arg RCC_IT_HSERDY:  HSE ready interrupt
  *            @arg RCC_IT_CSIRDY:  CSI ready interrupt
  *            @arg RCC_IT_PLL1RDY: PLL1RDY ready interrupt
  *            @arg RCC_IT_PLL2RDY: PLL2RDY ready interrupt
  *            @arg RCC_IT_PLL3RDY: PLL3RDY ready interrupt
  *            @arg RCC_IT_PLL4RDY: PLL4RDY ready interrupt
  *            @arg RCC_IT_PLL5RDY: PLL5RDY ready interrupt
  *            @arg RCC_IT_PLL6RDY: PLL6RDY ready interrupt
  *            @arg RCC_IT_PLL7RDY: PLL7RDY ready interrupt
  *            @arg RCC_IT_PLL8RDY: PLL8RDY ready interrupt
  *            @arg RCC_IT_LSECSS:  LSE Clock security system interrupt
  *            @arg RCC_IT_WKUP:    Wake-up from CSTOP Interrupt
  */
#ifdef CORE_CA35
#define __HAL_RCC_CLEAR_IT(__INTERRUPT__) (RCC->C1CIFCLRR = (__INTERRUPT__))
#endif /* CORE_CA35 */
#ifdef CORE_CM33
#define __HAL_RCC_CLEAR_IT(__INTERRUPT__) (RCC->C2CIFCLRR = (__INTERRUPT__))
#endif /* CORE_CM33 */

/** @brief  Check the RCC's interrupt has occurred or not.
  * @param  __INTERRUPT__: specifies the RCC interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg RCC_IT_LSIRDY:  LSI ready interrupt
  *            @arg RCC_IT_LSERDY:  LSE ready interrupt
  *            @arg RCC_IT_HSIRDY:  HSI ready interrupt
  *            @arg RCC_IT_HSERDY:  HSE ready interrupt
  *            @arg RCC_IT_CSIRDY:  CSI ready interrupt
  *            @arg RCC_IT_PLL1RDY: CA35/PLL1 PLL ready interrupt
  *            @arg RCC_IT_PLL2RDY: PLL2RDY ready interrupt
  *            @arg RCC_IT_PLL3RDY: PLL3RDY ready interrupt
  *            @arg RCC_IT_PLL4RDY: PLL4RDY ready interrupt
  *            @arg RCC_IT_PLL5RDY: PLL5RDY ready interrupt
  *            @arg RCC_IT_PLL6RDY: PLL6RDY ready interrupt
  *            @arg RCC_IT_PLL7RDY: PLL7RDY ready interrupt
  *            @arg RCC_IT_PLL8RDY: PLL8RDY ready interrupt
  *            @arg RCC_IT_LSECSS:  LSE Clock security system interrupt
  *            @arg RCC_IT_WKUP:    Wake-up from CSTOP Interrupt
  * @retval The new state of __INTERRUPT__ (TRUE or FALSE).
  */
#ifdef CORE_CA35
#define __HAL_RCC_GET_IT(__INTERRUPT__) (RCC->C1CIFCLRR & (__INTERRUPT__))
#endif /* CORE_CA35 */
#ifdef CORE_CM33
#define __HAL_RCC_GET_IT(__INTERRUPT__) (RCC->C2CIFCLRR & (__INTERRUPT__))
#endif /* CORE_CM33 */

/** @brief  Check the RCC's interrupt is enabled or not.
  * @retval The new state of __INTERRUPT__ (TRUE or FALSE).
  */
#ifdef CORE_CA35
#define __HAL_RCC_IT_ENABLED(__INTERRUPT__) (RCC->C1CIESETR & (__INTERRUPT__))
#endif /* CORE_CA35 */
#ifdef CORE_CM33
#define __HAL_RCC_IT_ENABLED(__INTERRUPT__) (RCC->C2CIESETR & (__INTERRUPT__))
#endif /* CORE_CM33 */

void     HAL_RCC_WAKEUP_IRQHandler(void);
void     HAL_RCC_WAKEUP_Callback(void);
void     HAL_RCC_IRQHandler(void);
void     HAL_RCC_Callback(uint32_t Flags);

/******************************************************************************
  * MISC
  ******************************************************************************/

HAL_StatusTypeDef HAL_RCC_DeInit(void);

/* Include RCC HAL Extension module */
#include "stm32mp2xx_hal_rcc_ex_valid.h"

/***** MP2 RIF SPECIFIC : TO BE MERGED TO DIFFERENT "defgroup" HERE ABOVE *****/
/** @defgroup RCC_RIF_HAL RCC_RIF
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup RCC_HAL_RIF_Exported_Constants RCC HAL RIF Exported Constants
  * @{
  */

/** @defgroup RCC_HAL_EC_RIF_LOCALRES_ACCESS RCC HAL Filtering of Local Resource Access
  * @{
  */
/****************************************************************************/
/* WARNING about LEGACY constants !                                         */
/* FOR AVOIDING CONFUSION IN SECURITY & PRIVILEGE ACCESS FILTERING          */
/* - "NSEC" stands for "No Security Filtering for Local Resource Access",   */
/*   which means that not only "secure" but also "non-secure" accesses are  */
/*   allowed.                                                               */
/* - "NPRIV" stands for "No Privilege Filtering for Local Resource Access", */
/*   which means that not only "privileged" but also "not-privileged"       */
/*   accesses are allowed.                                                  */
/****************************************************************************/

#define RCC_LOCALRES_PRIV    0x00000E00   /*!< Privilege attribute for Local Resource Access Filtering */
#define RCC_LOCALRES_NPRIV   0x00000300   /*!< No Privilege attribute for Local Resource Access Filtering */
#define RCC_LOCALRES_NOPRIV  0x00000300   /*!< No Privilege attribute for Local Resource Access Filtering */

#define RCC_LOCALRES_SEC     0x00005000   /*!< Secure attribute for Local Resource Access Filtering */
#define RCC_LOCALRES_NSEC    0x00002000   /*!< Unsecure/No Security attribute for Local Resource Access Filtering */
#define RCC_LOCALRES_NOSEC   0x00002000   /*!< Unsecure/No Security attribute for Local Resource Access Filtering */

#define RCC_LOCALRES_LOCKED  0xDD000000   /*!< Lock attribute for configuration of Local Resource Access Filtering */
/**
  * @}
  */

/** @defgroup RCC_HAL_EC_RIF_LOCALRES_ATTRIBUTES RCC HAL Attributes for Local Resource Access Filtering
  * @{
  */
#define RCC_LOCALRES_ATTR_NONE     0x00000000   /*!< No Attribute value for Local Resource Access Filtering */
#define RCC_LOCALRES_ATTR_INVALID  0x2200008C   /*!< Invalid Attribute value for Local Resource Access Filtering */

#define RCC_LOCALRES_ATTR_CID_TYPE_Pos      (0U)
#define RCC_LOCALRES_ATTR_CID_TYPE_Msk      (0x3UL << RCC_LOCALRES_ATTR_CID_TYPE_Pos)
#define RCC_LOCALRES_ATTR_STATIC_CID_Pos    (4U)
#define RCC_LOCALRES_ATTR_STATIC_CID_Msk    (0x7UL << RCC_LOCALRES_ATTR_STATIC_CID_Pos)
#define RCC_LOCALRES_ATTR_PRIV_Pos          (8U)
#define RCC_LOCALRES_ATTR_PRIV_Msk          (0xFUL << RCC_LOCALRES_ATTR_PRIV_Pos)
#define RCC_LOCALRES_ATTR_SEC_Pos           (12U)
#define RCC_LOCALRES_ATTR_SEC_Msk           (0xFUL << RCC_LOCALRES_ATTR_SEC_Pos)
#define RCC_LOCALRES_ATTR_DYNAMIC_CID_Pos   (16U)
#define RCC_LOCALRES_ATTR_DYNAMIC_CID_Msk   (0xFFUL << RCC_LOCALRES_ATTR_DYNAMIC_CID_Pos)
#define RCC_LOCALRES_ATTR_LOCK_Pos          (24U)
#define RCC_LOCALRES_ATTR_LOCK_Msk          (0xFFUL << RCC_LOCALRES_ATTR_LOCK_Pos)
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup RCC_HAL_RIF_Exported_Macros RCC HAL RIF Exported Macros
  * @{
  */
#define IS_LOCALRES_INDEX(INDEX) (((INDEX) >= RCC_LOCALRES_0) && \
                                  ((INDEX) <= RCC_LOCALRES_113))

/* On secure Cortex core only : */
#if defined(CORTEX_IN_SECURE_STATE)
#define IS_PRIV_ATTRIBUTE(ATTRIBUTE) (((ATTRIBUTE) == RCC_LOCALRES_PRIV)   || \
                                      ((ATTRIBUTE) == RCC_LOCALRES_NOPRIV) || \
                                      ((ATTRIBUTE) == RCC_LOCALRES_ATTR_NONE))

#define IS_LOCK_ATTRIBUTE(ATTRIBUTE) (((ATTRIBUTE) == RCC_LOCALRES_LOCKED) || \
                                      ((ATTRIBUTE) == RCC_LOCALRES_ATTR_NONE))

#define IS_SEC_ATTRIBUTE(ATTRIBUTE) (((ATTRIBUTE) == RCC_LOCALRES_SEC)   || \
                                     ((ATTRIBUTE) == RCC_LOCALRES_NOSEC) || \
                                     ((ATTRIBUTE) == RCC_LOCALRES_ATTR_NONE))

#define IS_CID_ATTRIBUTE(ATTRIBUTE) (((ATTRIBUTE) == RCC_LOCALRES_CID_TYPE_NONE) || \
                                     ((((ATTRIBUTE) & RCC_LOCALRES_ATTR_CID_TYPE_Msk) == RCC_LOCALRES_CID_TYPE_STATIC) \
                                      && ((~(ATTRIBUTE) & ~RCC_LOCALRES_CID_STATIC_Msk) == ~RCC_LOCALRES_CID_STATIC_Msk)) || \
                                     ((((ATTRIBUTE) & RCC_LOCALRES_ATTR_CID_TYPE_Msk) == RCC_LOCALRES_CID_TYPE_DYNAMIC) \
                                      && ((~(ATTRIBUTE) & ~RCC_LOCALRES_CID_DYNAMIC_Msk) == ~RCC_LOCALRES_CID_DYNAMIC_Msk)) )
#endif /* defined(CORTEX_IN_SECURE_STATE) */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup RCC_HAL_RIF_Exported_Functions RCC HAL RIF Exported Functions
  * @{
  */

/** @defgroup RCC_HAL_EF_RIF RCC HAL RIF
  * @brief RCC HAL RIF module driver
  * @{
  */
/* On secure Cortex core only : */
#if defined(CORTEX_IN_SECURE_STATE)
HAL_StatusTypeDef HAL_RCC_ConfigAttributes(uint32_t LocalResIndex, uint32_t LocalResAttributes);
#endif /* defined(CORTEX_IN_SECURE_STATE) */

HAL_StatusTypeDef HAL_RCC_GetConfigAttributes(uint32_t LocalResIndex, uint32_t *pLocalResAttributes);
HAL_StatusTypeDef HAL_RCC_TakeLocalResSemaphore(uint32_t LocalResIndex);
HAL_StatusTypeDef HAL_RCC_ReleaseLocalResSemaphore(uint32_t LocalResIndex);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/******************************************************************************/

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32MP2xx_HAL_RCC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
