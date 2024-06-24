/**
  ******************************************************************************
  * @file    stm32mp2xx_hal_rcc.c
  * @author  MCD Application Team
  * @TODO:   Re-write this brief
  * @brief   RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Reset and Clock Control (RCC) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions
  *
  @verbatim
  ==============================================================================
                      ##### RCC specific features #####
  ==============================================================================
    [..]
      After reset the device is running from Internal High Speed oscillator
      (HSI 64MHz) with Flash 0 wait state,and all peripherals are off except
      internal SRAM, Flash, JTAG and PWR
      (+) There is no pre-scaler on High speed (AHB) and Low speed (APB) buses;
          all peripherals mapped on these buses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in analogue mode , except the JTAG pins which
          are assigned to be used for debug purpose.

    [..]
      Once the device started from reset, the user application has to:
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings
      (+) Configure the AHB and APB buses pre-scalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock kernel source(s) for peripherals which clocks are not
          derived from the System clock through :RCC_D1CCIPR,RCC_D2CCIP1R,RCC_D2CCIP2R
          and RCC_D3CCIPR registers

                      ##### RCC Limitations #####
  ==============================================================================
    [..]
      A delay between an RCC peripheral clock enable and the effective peripheral
      enabling should be taken into account in order to manage the peripheral read/write
      from/to registers.
      (+) This delay depends on the peripheral mapping.
      (+) If peripheral is mapped on AHB: the delay is 2 AHB clock cycle
          after the clock enable bit is set on the hardware register
      (+) If peripheral is mapped on APB: the delay is 2 APB clock cycle
          after the clock enable bit is set on the hardware register

    [..]
      Implemented Workaround:
      (+) For AHB & APB peripherals, a dummy read to the peripheral register has been
          inserted in each __HAL_RCC_PPP_CLK_ENABLE() macro.

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
#if 0 // 191021
/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal.h"
#include "stm32mp2xx_hal_rcc_valid.h"
#include <stdio.h>

/** @addtogroup STM32MP2xx_HAL_Driver
  * @{
  */

/** @defgroup RCC RCC
  * @brief RCC HAL module driver
  * @{
  */

#ifdef HAL_RCC_VALID_MODULE_ENABLED

/* Additional macros for register access by address */
#ifndef SET_BITA
#define SET_BITA(ADDR, BIT)     (*(uint32_t*)(ADDR) |= (BIT))
#endif
#ifndef CLEAR_BITA
#define CLEAR_BITA(ADDR, BIT)   (*(uint32_t*)(ADDR) &= ~(BIT))
#endif
#ifndef READ_BITA
#define READ_BITA(ADDR, BIT)    (*(uint32_t*)(ADDR) & (BIT))
#endif
#ifndef CLEAR_REGA
#define CLEAR_REGA(ADDR)        (*(uint32_t*)(ADDR) = (0x0))
#endif
#ifndef WRITE_REGA
#define WRITE_REGA(ADDR,VAL)    (*(uint32_t*)(ADDR) = (VAL))
#endif
#ifndef READ_REGA
#define READ_REGA(ADDR)         (*(uint32_t*)(ADDR))
#endif
#ifndef MODIFY_REGA
#define MODIFY_REGA(ADDR, CLEARMASK, SETMASK)  WRITE_REGA((ADDR), (((READ_REGA(ADDR)) & (~(CLEARMASK))) | (SETMASK)))
#endif

/** @defgroup RCC_Private_Function_Prototypes RCC Private Functions Prototypes
  * @{
  */

/**
  * @}
  */

/* The following functions are useable from validation code but non-official */

/* Wait until '*addr & mask' == 'val'.
   Return: HAL_OK or HAL_TIMEOUT */
int RCC_Wait(volatile void *addr, uint32_t mask, uint32_t val, uint32_t timeout)
{
  uint32_t tickstart;
  tickstart = HAL_GetTick();
  while ((READ_REGA(addr) & mask) != val)
  {
    if ((uint32_t)(HAL_GetTick() - tickstart) > timeout)
    {
      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

/* Change BDCR write protection
   enable: 1=BDCR write protected, 0=can be modified */
void RCC_XableProtection(int enable)
{
  /* DBD3P bit in PWR->BDCR1 reg */
  if (enable)
  {
    CLEAR_BIT(PWR->BDCR1, 1 << 0);
  }
  else
    SET_BIT(PWR->BDCR1, 1 << 0);
}

/******************************************************************************
  * Flexiclockgen management
  * This part is designed to be a generic flexgen library.
  * Informations just below allow to make the link with real SOC.
  ******************************************************************************/

typedef struct
{
  uint16_t *ref_table;    // refclk = muxsel inputs
  uint16_t ref_size;      // Nb of muxsel inputs
  uint16_t *src_table;    // srcclk
  uint16_t src_size;      // Nb of srcclk inputs
  uint16_t *msout_table;  // Muxsel outputs
  uint16_t msout_size;    // Nb of muxsel outputs
  uint16_t *gout_table;   // GFG outputs
  uint16_t gout_size;     // Nb of GFG outputs
  uint16_t fdiv_first;    // Final div first clock ID (channel 0)
  uint16_t fdiv_last;     // Final div last clock ID
  uint16_t *cbar_table;   // Crossbar inputs
  uint16_t cbar_size;     // Nb of crossbar inputs
} RCC_FlexgenTypeDef;

/*
  * Start of SOC adaptation part
  */

/* Refclk (muxsel inputs) clocks mapping (index order) */
uint16_t RCC_FG_MuxselInTable[] =
{
  RCC_CK_HSI, // Index 0
  RCC_CK_HSE,
  RCC_CK_CSI,
  RCC_CK_LSI,
  RCC_CK_LSE,
};

/* Src clocks mapping (index order) */
uint16_t RCC_FG_SrcInTable[] =
{
  RCC_CK_HSI_KER,
  RCC_CK_HSE_KER,
  RCC_CK_CSI_KER,
  RCC_CK_SPDIF,
  RCC_CK_I2S,
  RCC_CK_LSI,
  RCC_CK_LSE,
};

/* Muxsel outputs clocks mapping */
uint16_t RCC_FG_MuxselOutTable[] =
{
  RCC_CK_PLL4REF, // Index 0 => GFG0/PLL4
  RCC_CK_PLL5REF,
  RCC_CK_PLL6REF,
  RCC_CK_PLL7REF,
  RCC_CK_PLL8REF, // Index 4 = > GFG4/PLL8
  RCC_CK_PLL1REF, // Index 5 => to flexgen external
  RCC_CK_PLL2REF,
  RCC_CK_PLL3REF,
};

/* GFG output clocks */
uint16_t RCC_FG_GFGOutTable[] =
{
  RCC_CK_PLL4, // Index 0 => GFG0/PLL4
  RCC_CK_PLL5,
  RCC_CK_PLL6,
  RCC_CK_PLL7,
  RCC_CK_PLL8, // Index 4 = > GFG4/PLL8
};

/* Cross bar inputs clocks mapping (index order) */
uint16_t RCC_FG_CrossbarInTable[] =
{
  RCC_CK_PLL4, // Index 0
  RCC_CK_PLL5,
  RCC_CK_PLL6,
  RCC_CK_PLL7,
  RCC_CK_PLL8,
  RCC_CK_HSI,
  RCC_CK_HSE,
  RCC_CK_CSI,
  RCC_CK_HSI_KER,
  RCC_CK_HSE_KER,
  RCC_CK_CSI_KER,
  RCC_CK_SPDIF,
  RCC_CK_I2S,
  RCC_CK_LSI,
  RCC_CK_LSE,
};

RCC_FlexgenTypeDef RCC_Flexgen =
{
  .ref_table = RCC_FG_MuxselInTable,
  .ref_size = sizeof(RCC_FG_MuxselInTable) / sizeof(uint16_t),
  .src_table = RCC_FG_SrcInTable,
  .src_size = sizeof(RCC_FG_SrcInTable) / sizeof(uint16_t),
  .msout_table = RCC_FG_MuxselOutTable,
  .msout_size = sizeof(RCC_FG_MuxselOutTable) / sizeof(uint16_t),
  .gout_table = RCC_FG_GFGOutTable,
  .gout_size = sizeof(RCC_FG_GFGOutTable) / sizeof(uint16_t),
  .cbar_table = RCC_FG_CrossbarInTable,
  .cbar_size = sizeof(RCC_FG_CrossbarInTable) / sizeof(uint16_t),
  .fdiv_first = RCC_CK_ICN_MCU,
  .fdiv_last = RCC_CK_CPU1EXT2F,
};

/*
 * End of SOC adapation part.
 * Rest of the code hereafter is generic.
 */

/* Check if 'clk_id' is handled by flexgen
   Returns: Clock index if found, or -1 if not found */
int RCC_FG_GetClockIdx(uint16_t clk_id, uint16_t *table, uint16_t size)
{
  int table_size = size;
  int idx;
  for (idx = 0; idx < table_size; idx++)
    if (table[idx] == clk_id) { break; }
  if (idx == table_size)
  {
    return -1;  // Invalid source
  }
  return idx;
}

/* Switch crossbar for given channel (xbar_idx).
   In case of timeout, previous switch value is restored to not let
   crossbar in a bad state.
   WARNING: Previous source must be ON to allow switch completion.
     With current RCC reset limitations only power ON may restore
     a clean state.
   Returns: 0=OK, -1=ERROR/timeout */
int RCC_FG_SetCrossbar(int xbar_idx, int xbar_in)
{
  if ((READ_REG(RCC->XBARxCFGR[xbar_idx]) & 0xf) == xbar_in)
  {
    return 0;  // Already set
  }

  uint32_t timeout = 1; // Few cycles of slowest clock (32K). 1ms is far enough
  uint32_t tickstart;

  /* Checking crossbar status first */
  for (tickstart = HAL_GetTick();
       (READ_REG(RCC->XBARxCFGR[xbar_idx]) & (1 << 7)) && ((int32_t)(HAL_GetTick() - tickstart) < timeout););
  if (READ_REG(RCC->XBARxCFGR[xbar_idx]) & (1 << 7))
  {
    return -1; // Timeout
  }

  /* Configuring + enable */
  uint32_t saved = READ_REG(RCC->XBARxCFGR[xbar_idx]);
  WRITE_REG(RCC->XBARxCFGR[xbar_idx], (1 << 6) | xbar_in);

  /* Waiting switch completion */
  for (tickstart = HAL_GetTick();
       (READ_REG(RCC->XBARxCFGR[xbar_idx]) & (1 << 7)) && ((int32_t)(HAL_GetTick() - tickstart) < timeout););
  if (READ_REG(RCC->XBARxCFGR[xbar_idx]) & (1 << 7))
  {
    /* Extending timeout */
    for (tickstart = HAL_GetTick();
         (READ_REG(RCC->XBARxCFGR[xbar_idx]) & (1 << 7)) && ((int32_t)(HAL_GetTick() - tickstart) < (timeout * 10)););
    if ((READ_REG(RCC->XBARxCFGR[xbar_idx]) & (1 << 7)) == 0)
    {
      return 0; // Finally OK => probably wrong tick programmation
    }
    /* END Extending timeout */
    WRITE_REG(RCC->XBARxCFGR[xbar_idx], saved); // Restoring to not let unstable status
    return -1; // Timeout
  }

  return 0;
}

/* Description: Read XBAR or MUXEL config to identify source clock
     clk_id = output clock
     src_id = input clock
   Returns: 0=OK, -1=ERROR */
int RCC_FG_GetSource(uint32_t clk_id, uint32_t *src_id)
{
  if (!clk_id || !src_id)
  {
    printf("<E> RCC_FG_GetSource : invalid args clk_id/src_id = %u/0x%08lX\n\r", clk_id, (uint32_t)src_id);
    return -1; // Invalid args
  }

  /* 'clk_id' = PLL ref clock ? */
  /* TODO: This part should be removed, except for PLL1..3 */
  int clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.msout_table, RCC_Flexgen.msout_size);
  if (clk_idx != -1)
  {
    int src_idx;
    src_idx = (READ_REG(RCC->MUXSELCFGR) >> (4 * clk_idx)) & 0x3;
    if (src_idx >= RCC_Flexgen.ref_size)
    {
      printf("<E> RCC_FG_GetSource : invalid src_idx %u (vs %u)\n\r", src_idx, RCC_Flexgen.ref_size);
      return -1;
    }
    *src_id = RCC_Flexgen.ref_table[src_idx];
    return 0;
  }

  /* 'clk_id' = PLL output clock ? */
  clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.gout_table, RCC_Flexgen.gout_size);
  if (clk_idx != -1)
  {
    int src_idx;
    src_idx = (READ_REG(RCC->MUXSELCFGR) >> (4 * clk_idx)) & 0x3;
    if (src_idx >= RCC_Flexgen.ref_size)
    {
      printf("<E> RCC_FG_GetSource : invalid src_idx %u (vs %u)\n\r", src_idx, RCC_Flexgen.ref_size);
      return -1;
    }
    *src_id = RCC_Flexgen.ref_table[src_idx];
    return 0;
  }

  /* Final div clock ? */
  if ((clk_id >= RCC_Flexgen.fdiv_first) && (clk_id <= RCC_Flexgen.fdiv_last))
  {
    /* Computing fdiv channel number */
    int clk_idx = clk_id - RCC_Flexgen.fdiv_first;

    /* Reading xbar config */
    int src_idx = READ_REG(RCC->XBARxCFGR[clk_idx]) & 0xf;

    /* Identifying connected clock */
    if (src_idx >= RCC_Flexgen.cbar_size)
    {
      printf("<E> RCC_FG_GetSource : invalid src_idx %u (vs %u)\n\r", src_idx, RCC_Flexgen.cbar_size);
      return -1;
    }
    *src_id = RCC_Flexgen.cbar_table[src_idx];
    return 0;
  }
  printf("<E> RCC_FG_GetSource : Neither a MUXSEL output nor a FDIV output\n\r");
  return -1; // Neither a MUXSEL output nor a FDIV output
}

/* Description: Configure MUXSEL or XBAR
     clk_id = output clock ID
     src_id  = input clock ID
   Note: Not static since directly used by valid SW. Maybe useful to create an official
         "HAL_RCC_SetSource(clk_id, src_id)"
   Returns:     0=OK, -1=ERROR */
int RCC_FG_SetSource(uint32_t clk_id, uint32_t src_id)
{
  if (!clk_id || (clk_id >= RCC_CK_LAST))
  {
    printf("<E> RCC_FG_SetSource : invalid clock %u (vs %u)\n\r", clk_id, RCC_CK_LAST);
    return -1; // Invalid clock
  }
  if (!src_id || (src_id >= RCC_CK_LAST))
  {
    printf("<E> RCC_FG_SetSource : invalid source %u (vs %u)\n\r", src_id, RCC_CK_LAST);
    return -1; // Invalid source clock
  }

  /* 'clk_id' = final div output clock ? */
  if ((clk_id >= RCC_Flexgen.fdiv_first) && (clk_id <= RCC_Flexgen.fdiv_last))
  {

    /* Computing xbar input */
    int in_idx;
    in_idx = RCC_FG_GetClockIdx(src_id, RCC_Flexgen.cbar_table, RCC_Flexgen.cbar_size);
    if (in_idx == -1)
    {
      printf("<E> RCC_FG_SetSource : invalid source of final div output clock\n\r");
      return -1; // Invalid source
    }

    /* Configuring crossbar */
    int out_idx = clk_id - RCC_Flexgen.fdiv_first;
    if (RCC_FG_SetCrossbar(out_idx, in_idx) != 0)
    {
      printf("<E> RCC_FG_SetSource : invalid crossbar config\n\r");
      return -1;
    }
    return 0;
  }

  /* 'clk_id' = GFG ref clock ? */
  int out_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.msout_table, RCC_Flexgen.msout_size);
  if (out_idx != -1)
  {
    /* Computing muxsel input */
    int in_idx;
    in_idx = RCC_FG_GetClockIdx(src_id, RCC_Flexgen.ref_table, RCC_Flexgen.ref_size);
    if (in_idx == -1)
    {
      printf("<E> RCC_FG_SetSource : invalid source of GFG ref clock\n\r");
      return -1; // Invalid source
    }

    uint32_t val;
    val = READ_REG(RCC->MUXSELCFGR) & ~(0x3 << (4 * out_idx));
    WRITE_REG(RCC->MUXSELCFGR, val | (in_idx << (4 * out_idx)));
    return 0;
  }

  /* 'clk_id' = observation clock ? */
  if ((clk_id == RCC_CK_OBS0) || (clk_id == RCC_CK_OBS1))
  {
    /* TODO: To be completed */
  }

  printf("<E> RCC_FG_SetSource : Unexpected clock\n\r");
  return -1; // Unexpected clock
}

/* Description: Get programmed divider (prediv + final div)
   Returns:     0=OK, -1=ERROR */
int RCC_FG_GetDiv(uint32_t clk_id, uint32_t *div)
{
  uint32_t pdiv, fdiv;
  int channel;

  if (!clk_id || (clk_id >= RCC_CK_LAST))
  {
    return -1;  // Invalid clock
  }
  if ((clk_id < RCC_Flexgen.fdiv_first) || (clk_id > RCC_Flexgen.fdiv_last))
  {
    return -1;  // Not a final div output
  }

  channel = clk_id - RCC_Flexgen.fdiv_first;

  /* Get pre-divider value */
  switch (READ_REG(RCC->PREDIVxCFGR[channel]) & RCC_PREDIVxCFGR_PREDIVx_Msk)
  {
    case 0:
      pdiv = 1;
      break;
    case 1:
      pdiv = 2;
      break;
    case 3:
      pdiv = 4;
      break;
    default:
      pdiv = 1024;
      break;
  }

  /* Get final divider */
  fdiv = (READ_REG(RCC->FINDIVxCFGR[channel]) & 0x3f) + 1;

  /* Compute divider */
  *div = pdiv * fdiv;

  return 0;
}

/* Description: Set pre and/or final divider ratios
   Note: In case of div config failed, previous div restored.
   Returns:     0=OK, -1=ERROR/timeout */
int RCC_FG_SetDiv(uint32_t clk_id, uint32_t div)
{
  uint32_t pdiv, fdiv;
  int channel;

  if (!clk_id || (clk_id >= RCC_CK_LAST))
  {
    return -1;  // Invalid clock
  }
  if ((clk_id < RCC_Flexgen.fdiv_first) || (clk_id > RCC_Flexgen.fdiv_last))
  {
    return -1;  // Not a final div output
  }

  channel = clk_id - RCC_Flexgen.fdiv_first;

  /* Computing predivider & final divider ratios.
     Reminder: pre div can only take values 1, 2, 4 or 1024 (coded 0, 1, 3, or 0x3FF)
               final div can take values 1 to 64 (coded 0 to 0x3F) */
  /* TBC : Exclude div invalid zero value */
  /* TBC : Exclude div odd values impossible to set */
  /* TBC : Exclude div value greater than 64*1024 = 65536 */

  if (div <= 64)
  {
    pdiv = 0x0;      /* "div1" */
    fdiv = div - 1;  /* "div"  */
  }
  else if (div <= 128)
  {
    pdiv = 0x1;      /* "div2"  */
    fdiv = (div / 2) - 1;
  }
  else if (div <= 256)
  {
    pdiv = 0x3;      /* "div4"  */
    fdiv = (div / 4) - 1;
  }
  else
  {
    pdiv = 0x3FF;    /* "div1024" */
    fdiv = (div / 1024) - 1;
  }

  volatile uint32_t *st_addr; // Status reg address
  int st_bit; // Status bit for PREDIVxCFGR & FINDIVxCFGR
  if (channel < 32)
  {
    st_bit = channel;
  }
  else
    st_bit = channel - 32;
  uint32_t saved;
  uint32_t tickstart;
#if defined (USE_STM32MP257CXX_EMU)
  uint32_t timeout = 2; // TO BE CONFIRMED (means ]1;2] ticks, latency included)
#elif defined (USE_STM32MP257CXX_FPGA)
  uint32_t timeout = 8; // TO BE CONFIRMED (means ]7;8] ticks, latency included)
#else
  uint32_t timeout = 2; // TO BE CONFIRMED (means ]1;2] ticks, latency included)
#endif /* defined (USE_STM32MP257CXX_EMU) | defined (USE_STM32MP257CXX_FPGA) */
  /* Pre-divider config if required */
  if ((READ_REG(RCC->PREDIVxCFGR[channel]) & 0x3ff) != pdiv)
  {
    if (channel < 32)
    {
      st_addr = &(RCC->PREDIVSR1);
    }
    else
      st_addr = &(RCC->PREDIVSR2);

    /* Check for completion of prediv current value */
    for (tickstart = HAL_GetTick(); (*st_addr & (1 << st_bit)) && (HAL_GetTick() - tickstart < timeout););
    if (*st_addr & (1 << st_bit))
    {
      printf("RCC_FG_SetDiv ERROR : PREDIV not READY (@0x%08lX = 0x%08lX) for channel %d\n",
             (uint32_t)st_addr, *st_addr, channel);
      return -1; // Timeout
    }
    saved = READ_REG(RCC->PREDIVxCFGR[channel]);
    WRITE_REG(RCC->PREDIVxCFGR[channel], pdiv);

    /* Wait for completion of prediv change */
    for (tickstart = HAL_GetTick(); (*st_addr & (1 << st_bit)) && (HAL_GetTick() - tickstart < timeout););
    if (*st_addr & (1 << st_bit))
    {
      printf("RCC_FG_SetDiv ERROR : PREDIV not CHANGED (@0x%08lX = 0x%08lX) for channel %d\n",
             (uint32_t)st_addr, *st_addr, channel);
      WRITE_REG(RCC->PREDIVxCFGR[channel], saved); // Restoring previous

      /* Wait for completion of prediv restoration */
      for (tickstart = HAL_GetTick(); (*st_addr & (1 << st_bit)) && (HAL_GetTick() - tickstart < timeout););
      if (*st_addr & (1 << st_bit))
      {
        printf("RCC_FG_SetDiv ERROR : PREDIV not RESTORED (@0x%08lX = 0x%08lX) for channel %d\n",
               (uint32_t)st_addr, *st_addr, channel);
      }
      return -1; // Timeout
    }
  }

  /* Final divider config if required */
  if ((READ_REG(RCC->FINDIVxCFGR[channel]) & 0x3f) != fdiv)
  {
    if (channel < 32)
    {
      st_addr = &(RCC->FINDIVSR1);
    }
    else
      st_addr = &(RCC->FINDIVSR2);

    /* Check for completion of findiv current value */
    for (tickstart = HAL_GetTick(); (*st_addr & (1 << st_bit)) && (HAL_GetTick() - tickstart < timeout););
    if (*st_addr & (1 << st_bit))
    {
      printf("RCC_FG_SetDiv ERROR : FINDIV not READY (@0x%08lX = 0x%08lX) for channel %d\n",
             (uint32_t)st_addr, *st_addr, channel);
      return -1; // Timeout
    }

    saved = READ_REG(RCC->FINDIVxCFGR[channel]);
    uint32_t findiven;
    findiven = saved & (1 << 6); // Get enable status
    WRITE_REG(RCC->FINDIVxCFGR[channel], findiven | fdiv); // Xable + final div

    /* Wait for completion of findiv change */
    for (tickstart = HAL_GetTick(); (*st_addr & (1 << st_bit)) && (HAL_GetTick() - tickstart < timeout););
    if (*st_addr & (1 << st_bit))
    {
      printf("RCC_FG_SetDiv ERROR : FINDIV not CHANGED (@0x%08lX = 0x%08lX) for channel %d\n",
             (uint32_t)st_addr, *st_addr, channel);
      WRITE_REG(RCC->FINDIVxCFGR[channel], saved); // Restoring previous

      /* Wait for completion of findiv restoration */
      for (tickstart = HAL_GetTick(); (*st_addr & (1 << st_bit)) && (HAL_GetTick() - tickstart < timeout););
      if (*st_addr & (1 << st_bit))
      {
        printf("RCC_FG_SetDiv ERROR : FINDIV not RESTORED (@0x%08lX = 0x%08lX) for channel %d\r\n",
               (uint32_t)st_addr, *st_addr, channel);
      }
      return -1; // Timeout
    }
  }
  return 0;
}

/* Read GFG0 to 4 (PLL4 to 8) config
   Returns: 0=OK, -1=ERROR */
static int RCC_FG_GetGFG(RCC_PLLCfgTypeDef *pll)
{
  assert_param(pll != NULL);
  assert_param(pll->id != RCC_CK_NONE);
  assert_param(pll->id < RCC_CK_LAST);

  /* Source identification.
     REMINDER: PLL is statically connected to xxREF. PLL source identification
               must therefore be done on xxREF clock */
  uint16_t ref_id; // PLL ref clock (RCC_CK_PLLxREF)
  ref_id = RCC_CK_PLL4REF + (pll->id - RCC_CK_PLL4);
  if (RCC_FG_GetSource(ref_id, &pll->source) != 0)
  {
    return -1;
  }

  /* Computing proper config registers addresses */
  uint32_t x;
  void *cfgr1, *cfgr2, *cfgr3, *cfgr4, *cfgr5, *cfgr6, *cfgr7;
  x = pll->id - RCC_Flexgen.gout_table[0];
  cfgr1 = (void *)&RCC->PLL4CFGR1 + (0x28 * x);
  cfgr2 = (void *)&RCC->PLL4CFGR2 + (0x28 * x);
  cfgr3 = (void *)&RCC->PLL4CFGR3 + (0x28 * x);
  cfgr4 = (void *)&RCC->PLL4CFGR4 + (0x28 * x);
  cfgr5 = (void *)&RCC->PLL4CFGR5 + (0x28 * x);
  cfgr6 = (void *)&RCC->PLL4CFGR6 + (0x28 * x);
  cfgr7 = (void *)&RCC->PLL4CFGR7 + (0x28 * x);

  uint32_t val;

  /* State ?
     Note: ON means enabled (PLLEN) and ready (PLLRDY)
     Note: Bypass state does not reflect PLL state (may be ON or OFF but unused) */
  pll->state = 0;
  val = READ_REGA(cfgr4);
  if (val & (1 << 10)) // Bypassed ?
  {
    pll->state |= RCC_PLLSTATE_BYPASS;
  }
  val = READ_REGA(cfgr1);
  if ((val & (1 << 8)) && (val & (1 << 24))) // PLLEN=1 + PLLRDY=1
  {
    pll->state |= RCC_PLLSTATE_ON;
  }
  else
    pll->state |= RCC_PLLSTATE_OFF;

  pll->flags = 0; // Integer, DAC disabled, SSCG disabled, SSCG center

  val = READ_REGA(cfgr2);
  pll->frefdiv = (val >> 0) & 0x3f;
  pll->fbdiv = (val >> 16) & 0xfff;

  /* Frac or integer ? */
  val = READ_REGA(cfgr4);
  if (val & (1 << 8)) // DSMEN
  {
    pll->flags |= RCC_PLLFLAG_FRACTIONAL;
  }
  val = READ_REGA(cfgr3);
  pll->frac_in = val & 0xffffff;
  if (val & (1 << 25))
  {
    pll->flags |= RCC_PLLFLAG_DAC_ON;
  }

  /* SSCG status */
  val = READ_REGA(cfgr5);
  pll->sscg_moddepth = (val >> 16) & 0x1f; // Modulation depth
  pll->sscg_modfreq = (val >> 0) & 0xf; // Modulation freq
  val = READ_REGA(cfgr3);
  if (val & (1 << 24)) // DOWNSPREAD
  {
    pll->flags |= RCC_PLLFLAG_SSCG_DOWN;
  }
  if ((val & (1 << 26)) == 0) // SSCGDIS
  {
    pll->flags |= RCC_PLLFLAG_SSCG_ON;
  }

  /* Output status */
  val = READ_REGA(cfgr4);
  if (val & (1 << 9))   // FOUTPOSTDIVEN
  {
    pll->postdiv1 = READ_REGA(cfgr6) & 0x7;
    pll->postdiv2 = READ_REGA(cfgr7) & 0x7;
  }
  else
  {
    /* Fout postdiv disabled => resulting div = 1 */
    pll->postdiv1 = 1;
    pll->postdiv2 = 1;
  }

  return 0;
}

/* Description: Configure GFG (PLL or FS)
                WARNING: This function uses bypass if available but if not
                it assumes PLL/FS is free to be reprogrammed (not in use).
   Returns:     0=OK, -1=ERROR */
static int RCC_FG_SetGFG(RCC_PLLCfgTypeDef *pll)
{
  assert_param(pll != NULL);
  assert_param(pll->id != RCC_CK_NONE);
  assert_param(pll->id < RCC_CK_LAST);
  assert_param(pll->source != RCC_CK_NONE);
  assert_param(pll->source < RCC_CK_LAST);

  /* Checking PLL clock */
  int clk_idx;
  clk_idx = RCC_FG_GetClockIdx(pll->id, RCC_Flexgen.gout_table, RCC_Flexgen.gout_size);
  if (clk_idx == -1)
  {
    return -1;  // Invalid PLL
  }

  /* Checking PLL source clock.
     It can only be one of the MUXSEL inputs (refclk) */
  int src_idx;
  src_idx = RCC_FG_GetClockIdx(pll->source, RCC_Flexgen.ref_table, RCC_Flexgen.ref_size);
  if (src_idx == -1)
  {
    return -1;  // Invalid source
  }

  void *cfgr1, *cfgr2, *cfgr3, *cfgr4, *cfgr5, *cfgr6, *cfgr7;
  cfgr1 = (void *)&RCC->PLL4CFGR1 + (0x28 * clk_idx);
  cfgr2 = (void *)&RCC->PLL4CFGR2 + (0x28 * clk_idx);
  cfgr3 = (void *)&RCC->PLL4CFGR3 + (0x28 * clk_idx);
  cfgr4 = (void *)&RCC->PLL4CFGR4 + (0x28 * clk_idx);
  cfgr5 = (void *)&RCC->PLL4CFGR5 + (0x28 * clk_idx);
  cfgr6 = (void *)&RCC->PLL4CFGR6 + (0x28 * clk_idx);
  cfgr7 = (void *)&RCC->PLL4CFGR7 + (0x28 * clk_idx);
  uint32_t src_id; // PLL source clock ID
  src_id = RCC_CK_PLL4REF + (pll->id - RCC_CK_PLL4); // TODO: Make it generic

  /* PLLTS16FFCLAFRAC2 only so far */

  /* PLLTS16FFCLAFRAC2 setup order:
     - Set bypass (supported by PLL1 ?)
     - Disable postdiv (supported by PLL1 ?) then VCO
     - Select source
     - Configure PLL
     - Enable PLL if RCC_PLLSTATE_ON
     - Remove bypass if NOT RCC_PLLSTATE_BYPASS (supported by PLL1 ?)
   */

  /* Check FREF
     Must be running and in good range */
  /* TODO */

  /* [TEMPORARY ?] */
  /* When PLL OFF command is selected, just do it */
  /* This avoids "<E> PLL<y>RDY not cleared" issue on FPGA platform */
  /* when running application on A35 */
  if (pll->state & RCC_PLLSTATE_OFF)
  {
    /* Disable PLL */
    CLEAR_BITA(cfgr1, 1 << 8);

    /* Wait for unlock */
    if (RCC_Wait(cfgr1, 1 << 24, 0, PLL_TIMEOUT_VALUE) != HAL_OK)
    {
      return -1;
    }
    else
    {
      /* Remove bypass */
      CLEAR_BITA(cfgr4, (1 << 10));
//            return 0;
    }
  }

  /* Bypass, in case PLL is used */
  /* NO MORE USE IT ON FPGA, BECAUSE IT LEADS TO MALFUNCTION */
  //SET_BITA(cfgr4, (1 << 10));
  /* Disable post div then PLL */
  /* COMMENTED BECAUSE UNUSEFUL */
  //CLEAR_BITA(cfgr4, 1 << 9); // FOUTPOSTDIVEN=0
  //CLEAR_BITA(cfgr1, 1 << 8); // PLLEN=0

  /* Select PLL ref source */
  if (RCC_FG_SetSource(src_id, pll->source) != 0)
  {
    return -1;
  }

  /* Configure PLL */
  uint32_t sscg_dis, dacen, downspread;
  sscg_dis = (pll->flags & RCC_PLLFLAG_SSCG_ON) ? 0 : 1;
  dacen = (pll->flags & RCC_PLLFLAG_DAC_ON) ? 1 : 0;
  downspread = (pll->flags & RCC_PLLFLAG_SSCG_DOWN) ? 1 : 0;
  uint32_t dsmen, postdiven;
  dsmen = (pll->flags & RCC_PLLFLAG_FRACTIONAL) ? 1 : 0;
//    if ((!pll->postdiv1 && !pll->postdiv2) ||
//        ((pll->postdiv1 == 1) && (pll->postdiv2 == 1)))
  if ((!pll->postdiv1 && !pll->postdiv2))
  {
    postdiven = 0;  // FOUTPOSTDIVEN=0
  }
  else
    postdiven = 1; // FOUTPOSTDIVEN=1
  WRITE_REGA(cfgr2, (pll->fbdiv << 16) | pll->frefdiv);
  WRITE_REGA(cfgr3, (sscg_dis << 26) | (dacen << 25) | (downspread << 24) | pll->frac_in);
//    WRITE_REGA(cfgr4, (1 << 10) | (postdiven << 9) | (dsmen << 8)); // Keep bypass + foutpostdiven : NO BYPASS !!!
  WRITE_REGA(cfgr4, (postdiven << 9) | (dsmen << 8)); // Keep foutpostdiven
  WRITE_REGA(cfgr5, (pll->sscg_moddepth << 16) | pll->sscg_modfreq);
  WRITE_REGA(cfgr6, pll->postdiv1); // Postdiv1
  WRITE_REGA(cfgr7, pll->postdiv2); // Postdiv2

  if (pll->state & RCC_PLLSTATE_ON)   // PLL ON
  {
    /* TODO: Could be replaced by call to HAL_RCC_Xable() */
    /* Enable PLL */
    SET_BITA(cfgr1, 1 << 8); // PLLEN
    /* Wait for lock */
    if (RCC_Wait(cfgr1, 1 << 24, 1 << 24, PLL_TIMEOUT_VALUE) != HAL_OK)
    {
      return -1;
    }
  }

  // NO BYPASS
  //if ((pll->state & RCC_PLLSTATE_BYPASS) == 0)
  //    CLEAR_BITA(cfgr4, (1 << 10)); // Remove bypass

  return 0;
}

/* Description: Enable/disable clocks (PLL or final divider stage)
   Returns:     0=OK, -1=ERROR */
static int RCC_FG_Xable(uint16_t clk_id, int enable)
{
  /* Final divider clock ? */
  if ((clk_id >= RCC_Flexgen.fdiv_first) && (clk_id <= RCC_Flexgen.fdiv_last))
  {
    int channel = clk_id - RCC_Flexgen.fdiv_first;
    uint32_t data;
    if (enable)
    {
      /* Just in case because not supposed to be OFF,
         enabling clock at crossbar stage. */
      data = READ_REG(RCC->XBARxCFGR[channel]);
      WRITE_REG(RCC->XBARxCFGR[channel], data | (1 << 6));

      /* Enabling clock at final divider stage */
      data = READ_REG(RCC->FINDIVxCFGR[channel]);
      WRITE_REG(RCC->FINDIVxCFGR[channel], data | (1 << 6));
    }
    else
    {
      /* Disabling clock at final divider stage */
      data = READ_REG(RCC->FINDIVxCFGR[channel]);
      WRITE_REG(RCC->FINDIVxCFGR[channel], data & ~(1 << 6));
    }
    return 0;
  }

  /* GFG output clock ? */
  int clk_idx;
  clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.gout_table, RCC_Flexgen.gout_size);
  if (clk_idx != -1)
  {
    void *cfgr1;
    cfgr1 = (void *)&RCC->PLL4CFGR1 + (0x28 * clk_idx);
    if (enable)
    {
      SET_BITA(cfgr1, (1 << 8));
    }
    else
      CLEAR_BITA(cfgr1, (1 << 8)); // Set OFF
    return 0;
  }

  return -1; // Not supported
}

/* Get 'enabled' status of GFG (PLLs) or final div outputs.
   Returns: 0=OK, -1=ERROR */
static int RCC_FG_GetStatus(uint16_t clk_id, int *enabled)
{
  /* 'clk_id' == final divider output ? */
  if ((clk_id >= RCC_Flexgen.fdiv_first) && (clk_id <= RCC_Flexgen.fdiv_last))
  {
    int channel = clk_id - RCC_Flexgen.fdiv_first;

    *enabled = 1; // Assuming enabled at both Xbar & Fdiv

    /* Check cross-bar status */
    uint32_t data;
    data = READ_REG(RCC->XBARxCFGR[channel]);
    if ((data & (1 << 6)) == 0)
    {
      *enabled = 0; // Disabled at cross-bar stage
    }
    else
    {
      /* Enabled at cross-bar level.
         Check final divider status */
      data = READ_REG(RCC->FINDIVxCFGR[channel]);
      if ((data & (1 << 6)) == 0)
      {
        *enabled = 0; // Disabled at final divider stage
      }
    }
    return 0;
  }

  /* 'clk_id' == GFG output clock ? */
  int clk_idx;
  clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.gout_table, RCC_Flexgen.gout_size);
  if (clk_idx != -1)
  {
    void *addr = (void *) & (RCC->PLL4CFGR1) + (0x28 * clk_idx); // PLLxCFGR1 addr
    if (READ_REGA(addr) & (1 << 24)) // Check PLL ready flag
    {
      *enabled = 1;
    }
    else
    {
      *enabled = 0;
    }
    return 0;
  }

  return -1; // Invalid clock
}

/* Clock ID to jitter estimator/observation 'ckintsel' field.
   Returns: 0=OK, -1=ERROR */
static int RCC_FG_ID2Ckintsel(uint16_t clk_id, uint32_t *ckintsel)
{
  /* 'clk_id' == Final div output ? */
  if ((clk_id >= RCC_Flexgen.fdiv_first) && (clk_id <= RCC_Flexgen.fdiv_last))
  {
    *ckintsel = (0x3 << 6) | (clk_id - RCC_Flexgen.fdiv_first);
    return 0;
  }

  /* 'clk_id' == ref input clock ? */
  int clk_idx;
  clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.ref_table, RCC_Flexgen.ref_size);
  if (clk_idx != -1)
  {
    *ckintsel = (0x0 << 6) | clk_idx;
    return 0;
  }

  /* 'clk_id' == src input clock ? */
  clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.src_table, RCC_Flexgen.src_size);
  if (clk_idx != -1)
  {
    *ckintsel = (0x2 << 6) | clk_idx;
    return 0;
  }

  /* 'clk_id' == MUXSEL output clock ? */
  clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.msout_table, RCC_Flexgen.msout_size);
  if (clk_idx != -1)
  {
#ifdef APPLY_BZ121873
    *ckintsel = (0x0 << 6) | (clk_idx + RCC_Flexgen.ref_size);
#else
    *ckintsel = (0x0 << 6) | (clk_idx + RCC_Flexgen.ref_size + 2);
#endif /* APPLY_BZ121873 */
    return 0;
  }

  /* 'clk_id' == GFG output clock ? */
  clk_idx = RCC_FG_GetClockIdx(clk_id, RCC_Flexgen.gout_table, RCC_Flexgen.gout_size);
  if (clk_idx != -1)
  {
    *ckintsel = (0x1 << 6) | clk_idx;
    return 0;
  }

  return -1; // Unsupported clock
}

/* Jitter estimator/observation 'ckintsel' field to clock ID.
   Returns: 0=OK, -1=ERROR */
static int RCC_FG_Ckintsel2ID(uint32_t ckintsel, uint16_t *clk_id)
{
  uint32_t shb; // Selection high bits [7:6]
  uint32_t slb; // Selection low bits [5:0]
  shb = ((ckintsel >> 6) & 0x3);
  slb = (ckintsel & 0x3f);

  /* 'clk_id' == Final div output ? */
  if (shb == 0x3)
  {
    *clk_id = RCC_Flexgen.fdiv_first + slb;
    return 0;
  }

  /* 'clk_id' == src input clock ? */
  if (shb == 0x2)
  {
    if (slb >= RCC_Flexgen.src_size)
    {
      return -1;
    }
    *clk_id = RCC_Flexgen.src_table[slb];
    return 0;
  }

  /* 'clk_id' == GFG output clock ? */
  if (shb == 0x1)
  {
    if (slb >= RCC_Flexgen.gout_size)
    {
      return -1;
    }
    *clk_id = RCC_Flexgen.gout_table[slb];
    return 0;
  }

  /* ckintsel[7:6] = 0 => either osc (ref in) or pll ref */
  if (slb < RCC_Flexgen.ref_size)
  {
    *clk_id = RCC_Flexgen.ref_table[slb];
    return 0;
  }
  slb -= (RCC_Flexgen.ref_size + 2);
  if (slb < RCC_Flexgen.msout_size)
  {
    *clk_id = RCC_Flexgen.msout_table[slb];
    return 0;
  }

  return -1; // Unsupported clock
}

/* Perform a HW frequency measurement
   by using embedded "clock frequency calculator"
   according to RCC user spec "Clock frequency calculation" subsection
   'clk_id' = Clock to measure.
   'ref_id' = Clock used as reference.
   '*freq' = Measured frequency
   Note: not static to be callable from validation code
   Returns: 0=OK, -1=ERROR */
int RCC_FG_GetMeasure(uint32_t clk_id, uint32_t ref_id, uint32_t *freq)
{
  int fstatus;
  uint32_t freq_meas_complete_flag, fcalctwc_val, min_fcalctwc;
  uint32_t regval, fcalcsts_bit;
  uint32_t timeout_flag, timeout_in_ticks, tickstart;
  uint64_t ckin_freq_val;

  /* Initial checks */
  assert_param(clk_id != RCC_CK_NONE);
  assert_param(clk_id < RCC_CK_LAST);
  assert_param(ref_id != RCC_CK_NONE);
  assert_param(ref_id < RCC_CK_LAST);
  assert_param(freq != NULL);

  *freq = 0;

  /* Checking ref clock: can only be GFG input/ref clock */
  int ref_idx;
  ref_idx = RCC_FG_GetClockIdx(ref_id, RCC_Flexgen.msout_table, RCC_Flexgen.msout_size);
  if (ref_idx == -1)
  {
    return -1;
  }
  uint32_t ref_freq = HAL_RCC_GetFreq(ref_id);
  if (!ref_freq)
  {
    return -1;
  }

  /* If clock to measure is disabled, don't attempt to measure */
  if (!HAL_RCC_IsRunning(clk_id))
  {
    return 0;
  }

  /* ALGORITHM STARTS HERE. IT IS THE FOLLOWING :
   * While there is an overflow in measurement,
   * decrease FCALCTWC value from 15 (65535 cycles) to 1 (3 cycles).
   */
  freq_meas_complete_flag = 0;
  fcalctwc_val = 0xF;
  min_fcalctwc = 0x1;
  while ((fcalctwc_val >= min_fcalctwc)
         && (freq_meas_complete_flag == 0))
  {
    /* set the FCALCRSTN bit in the RCC Clock Frequency Calculator and Observation 1
     * clock Configuration Register (RCC_FCALCOBS1CFGR) to enable the clock frequency
     * calculator.
     */
    SET_BIT(RCC->FCALCOBS1CFGR, (1 << 27));

    /* set the CKINTSEL (or CKEXTSEL if FCALCCKEXTSEL bit is set) field in the RCC
     * Clock Frequency Calculator and Observation 0 Clock Configuration Register
     * (RCC_FCALCOBS0CFGR) to select the clock frequency calculator input clock
     * (i.e. ckin).
     */
    uint32_t ckintsel; // Selector of clock to measure
    if (RCC_FG_ID2Ckintsel(clk_id, &ckintsel) != 0)
    {
      return -1;
    }
    regval = RCC->FCALCOBS0CFGR;
    regval &= ~(0xFF);
    regval |= ckintsel;
    WRITE_REG(RCC->FCALCOBS0CFGR, regval);

    /* set the FCALCCKEN field in the RCC Clock Frequency Calculator and Observation 0
     * Clock Configuration Register (RCC_FCALCOBS0CFGR) to enable the selected input
     * clock at the input of clock frequency calculator.
     */
    regval = RCC->FCALCOBS0CFGR;
    regval |= (1 << 25);
    WRITE_REG(RCC->FCALCOBS0CFGR, regval);

    /* set the FCALCREFCKSEL field in the RCC Clock Frequency Calculator Reference
     * clock Configuration Register (RCC_FCALCREFCFGR) to select the reference clock
     * (i.e. ckref).
     */
    WRITE_REG(RCC->FCALCREFCFGR, ref_idx);

    /* set the clock frequency time window value via the FCALCTWC field in the RCC Clock
     * Frequency Calculator Control Register 2 (RCC_FCALCCR2)
     */
    regval = RCC->FCALCCR2;
    regval &= ~(0xF << 11);
    regval |= (fcalctwc_val << 11);
    WRITE_REG(RCC->FCALCCR2, regval);
    /* set the FCALCTYP field to 0x0C in the RCC Clock Frequency Calculator Control
     * Register 2 (RCC_FCALCCR2) to select the frequency value type.
     */
    regval = RCC->FCALCCR2;
    regval |= (0xC << 17);
    WRITE_REG(RCC->FCALCCR2, regval);
    /* select the application mode via the FCALCMD field in the RCC Clock Frequency
     * Calculator Control Register 2 (RCC_FCALCCR2).
     */
    regval = RCC->FCALCCR2;
    regval |= (0x3 << 3);
    WRITE_REG(RCC->FCALCCR2, regval);

    /* set the FCALCRUN bit in the RCC Clock Frequency Calculator Control Register 1
     * (RCC_FCALCCR1).
     */
    WRITE_REG(RCC->FCALCCR1, 1);

    /* poll the FCALCSTS bit in the RCC Clock Frequency Calculator Status Register
     * (RCC_FCALCSR) until the calculation is done.
     */
    fcalcsts_bit = 0;
    timeout_flag = 0;
    /* CI : timeout value (in ms) to be tuned according */
    /*      to lowest frequency to be measured */
    timeout_in_ticks = 100;

    tickstart = HAL_GetTick();
    while ((fcalcsts_bit == 0)
           && (timeout_flag == 0))
    {
      regval = RCC->FCALCSR;
      if ((regval & 0xFFFF) != 0)
      {
        fcalcsts_bit = (regval & (1 << 19)) >> 19;
      }
      /* detect timeout (time expired whereas counter still zero) */
      else if (HAL_GetTick() - tickstart > timeout_in_ticks)
      {
        timeout_flag = 1;
      }
    }
    /* read the measured value via the FVAL field in the RCC Clock Frequency Calculator
     * Status Register (RCC_FCALCSR). If FVAL[16] is 1 then the measured value is false.
     */
    /* detect overflow */
    if ((regval & (1 << 16)) == (1 << 16))
    {
      /* set status for next measurement iteration */
      fstatus = 0;
    }
    else if (1 == timeout_flag)
    {
      printf("<E> FCALCTWC = %ld : TIMEOUT - FVAL = %ld\n", fcalctwc_val, (regval & 0xFFFF));
      /* set status for leaving measurement algorithm */
      fstatus = -1;
    }
    else
    {
      if (0 == (regval & 0xFFFF))
      {
        printf("<W> FCALCTWC = %ld : FVAL = 0\n", fcalctwc_val);
        /* set status for leaving measurement algorithm */
        fstatus = -1;
      }
      else
      {
        /* compute measured frequency */
        ckin_freq_val = (uint64_t)((regval & 0xFFFF) - 16) * (uint64_t)ref_freq;
        uint32_t denom_val = (1 << (fcalctwc_val + 1)) + 5;
        ckin_freq_val /= (uint64_t)denom_val;
        *freq = (uint32_t)ckin_freq_val;
        freq_meas_complete_flag = 1;
        fstatus = 0;
      }
    }
    /* Re-initialisation : */
    /* clear the FCALCRUN bit in the RCC Clock Frequency Calculator
     * Control Register 1 (RCC_FCALCCR1)
     */
    WRITE_REG(RCC->FCALCCR1, 0);
    /* clear the FCALCRSTN bit in the RCC Clock Frequency Calculator and Observation 1
     * clock Configuration Register (RCC_FCALCOBS1CFGR) to disable the clock frequency
     * calculator.
     */
    CLEAR_BIT(RCC->FCALCOBS1CFGR, (1 << 27));

    /* Waiting for status register to be cleared : NOT SPECIFIED */
    timeout_flag = 0;
    timeout_in_ticks = 100; /* TO BE TUNED */
    tickstart = HAL_GetTick();
    regval = READ_REG(RCC->FCALCSR);
    while (((regval & 0xffff) != 0)
           && (timeout_flag == 0))
    {
      regval = READ_REG(RCC->FCALCSR);
      /* detect timeout */
      if (HAL_GetTick() - tickstart > timeout_in_ticks)
      {
        timeout_flag = 1;
      }
    }
    if (1 == timeout_flag)
    {
      printf("<E> FCALCSR not reset after TIMEOUT = %08lX\n", regval);
      printf("<E> Tick start / end = %08lX / %08lX\n", tickstart, HAL_GetTick());
      fstatus = -1;
    }
    /* decrease FCALCTWC value */
    fcalctwc_val -= 1;
    /* check status */
    if (-1 == fstatus)
    {
      break;
    }
  }
  return fstatus;
}

/* Configure observation path.
   'clk_id' = RCC_CK_OBS0 or RCC_CK_OBS1.
   'src_id' = Clock to observe
   'div' = Division factor to apply prior to observation
   Returns: 0=OK, -1=ERROR */
static int RCC_FG_SetObs(uint16_t clk_id, uint16_t src_id, uint32_t *div)
{
  assert_param(clk_id != RCC_CK_NONE);
  assert_param(clk_id < RCC_CK_LAST);
  assert_param(src_id != RCC_CK_NONE);
  assert_param(src_id < RCC_CK_LAST);
  assert_param(div != NULL);

  /* TODO: Need to add support of EXT clcoks */
  /* TODO: Must be generic code */
// extern const char *RCC_ClockNames[];
// extern int printf(const char *format, ...);
// printf("Routing %s to %s for observation\n", RCC_ClockNames[src_id], RCC_ClockNames[clk_id]);
  uint32_t obsext = 0;
  uint32_t ckintsel, ckextsel = 0;
  if (RCC_FG_ID2Ckintsel(src_id, &ckintsel) != 0)
  {
    return -1;
  }

  /* Computing closest but greater div.
     Only div by 1, 2 4, 8, 16, 32, 64 and 128 are supported */
  uint32_t obsdiv = 0;
  for (obsdiv = 0; obsdiv < 7; obsdiv++)
    if ((1 << obsdiv) >= *div) { break; }
      *div = 1 << obsdiv;

  volatile uint32_t *obscfg;
  if (clk_id == RCC_CK_OBS0)
  {
    obscfg = &(RCC->FCALCOBS0CFGR);
  }
  else
    obscfg = &(RCC->FCALCOBS1CFGR);

  if (obsext == 0)
  {
    WRITE_REGA(obscfg, (1 << 26) | (obsdiv << 22) | (ckintsel << 0));
  }
  else
    WRITE_REGA(obscfg, (1 << 26) | (obsdiv << 22) | (1 << 16) | (ckextsel << 8));

  return 0;
}

/* Get observation path config
   'clk_id' = RCC_CK_OBS0 or RCC_CK_OBS1.
   'src_id' = Clock to observe (pointer)
   'div' = Division factor to apply prior to observation (pointer)
   Returns: 0=OK, -1=ERROR */
static int RCC_FG_GetObs(uint16_t clk_id, uint16_t *src_id, uint32_t *div)
{
  assert_param(clk_id != RCC_CK_NONE);
  assert_param(clk_id < RCC_CK_LAST);
  assert_param(src_id != NULL);
  assert_param(div != NULL);

  /* TODO: Need to add support of EXT clcoks */
  /* TODO: Must be generic code */
// extern const char *RCC_ClockNames[];
// extern int printf(const char *format, ...);
// printf("Routing %s to %s for observation\n", RCC_ClockNames[src_id], RCC_ClockNames[clk_id]);
  // uint32_t obsext = 0;
  // uint32_t ckintsel, ckextsel = 0;
  // if (RCC_FG_ID2Ckintsel(src_id, &ckintsel) != 0)
  // return -1;

  uint32_t obscfg;
  if (clk_id == RCC_CK_OBS0)
  {
    obscfg = READ_REG(RCC->FCALCOBS0CFGR);
  }
  else
    obscfg = READ_REG(RCC->FCALCOBS1CFGR);

  /* Source ? */
  RCC_FG_Ckintsel2ID(obscfg & 0xff, src_id);

  /* Divider value ? */
  *div = 1 << ((obscfg >> 22) & 0x7);

  /* State */
  // if (obscfg & (1 << 26))
  // state = RCC_CLKSTATE_ON;
  // else
  // state = RCC_CLKSTATE_OFF;

  return 0;
}

/******************************************************************************
  * END of flexiclockgen management functions
  ******************************************************************************/

/** @defgroup RCC_Private_Functions RCC Private Functions
  * @{
  */

/** @defgroup RCC_Group1 Initialization and de-initialization functions
  *  @TODO:    Re-write this brief
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
           ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]
      This section provides functions allowing to configure the internal/external oscillators
      (HSE, HSI, LSE,CSI, LSI, PLL, CSS and MCO) and the System busses clocks (SYSCLK, AHB, APB1
       and APB2).

    [..] Internal/external clock and PLL configuration
         (#) HSI (high-speed internal), 64 MHz factory-trimmed RC used directly or through
             the PLL as System clock source.
         (#) CSI is a low-power RC oscillator which can be used directly as system clock, peripheral
             clock, or PLL input.But even with frequency calibration, is less accurate than an
             external crystal oscillator or ceramic resonator.
         (#) LSI (low-speed internal), 32 KHz low consumption RC used as IWDG and/or RTC
             clock source.

         (#) HSE (high-speed external), 4 to 48 MHz crystal oscillator used directly or
             through the PLL as System clock source. Can be used also as RTC clock source.

         (#) LSE (low-speed external), 32 KHz oscillator used as RTC clock source.

         (#) PLL , The RCC features three independent PLLs (clocked by HSI , HSE or CSI),
             featuring three different output clocks and able  to work either in integer or Fractional mode.
           (++) A main PLL, PLL1, which is generally used to provide clocks to the CPUs
                and to some peripherals.
           (++) Two dedicated PLLs, PLL2 and PLL3, which are used to generate the kernel clock for peripherals.

         (#) CSS (Clock security system), once enable and if a HSE clock failure occurs
            (HSE used directly or through PLL as System clock source), the System clock
             is automatically switched to HSI and an interrupt is generated if enabled.
             The interrupt is linked to the Cortex-M NMI (Non-Mask-able Interrupt)
             exception vector.

         (#) MCO1 (micro controller clock output), used to output HSI, LSE, HSE, PLL1(PLL1_Q)
             or HSI48 clock (through a configurable pre-scaler) on PA8 pin.

         (#) MCO2 (micro controller clock output), used to output HSE, PLL2(PLL2_P), SYSCLK,
             LSI, CSI, or PLL1(PLL1_P) clock (through a configurable pre-scaler) on PA14 pin.

    [..] System, AHB and APB buses clocks configuration
         (#) Several clock sources can be used to drive the System clock (SYSCLK): CSI,HSI,
             HSE and PLL.
             The AHB clock (HCLK) is derived from System core clock through configurable
             pre-scaler and used to clock the CPU, memory and peripherals mapped
             on AHB and APB bus of the 3 Domains (D1, D2, D3) through configurable pre-scalers
             and used to clock the peripherals mapped on these buses. You can use
             "HAL_RCC_GetSysClockFreq()" function to retrieve system clock frequency.

         -@- All the peripheral clocks are derived from the System clock (SYSCLK) except those
             with dual clock domain where kernel source clock could be selected through
             RCC_D1CCIPR,RCC_D2CCIP1R,RCC_D2CCIP2R and RCC_D3CCIPR registers

@endverbatim
  * @{
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *          - HSI ON and used as system clock source
  *          - HSE/CSI/PLL4..8 OFF
  *          - AHB, APB Bus pre-scaler set to 1.
  *          - MCO1 and MCO2 OFF
  *          - All interrupts disabled (except Wake-up from CSTOP Interrupt Enable)
  * @note   This function doesn't modify the configuration of the
  *          - Peripheral clocks
  *          - LSI, LSE and RTC clock
  *          - HSECSS and LSECSS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_DeInit(void)
{
  /* Enable HSI */
  if (HAL_RCC_Xable(RCC_CK_HSI, 1) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reroute all flexgen outputs to use HSI with div1 */
  RCC_ClkCfgTypeDef clk;
  uint16_t id;
  for (id = RCC_Flexgen.fdiv_first; id <= RCC_Flexgen.fdiv_last; id++)
  {
    clk.id = id;
    clk.source = RCC_CK_HSI;
    clk.divvalue = 1;
    clk.state = RCC_CLKSTATE_ON;
    if (HAL_RCC_SetClockCfg(&clk) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }

  /* Switch OFF all flexgen PLLs */
  RCC_PLLCfgTypeDef pll;
  for (id = RCC_CK_PLL4; id <= RCC_CK_PLL8; id++)
  {
    pll.id = id;
    HAL_RCC_GetPllCfg(&pll);
    pll.source = RCC_CK_HSI;
    pll.state = RCC_PLLSTATE_OFF;
    if (HAL_RCC_SetPllCfg(&pll) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }
  /* TODO: May need more cleanup on PLL registers */
  /* TODO: Specifically reset POSTDIVEN */

  /* Reroute all GFG ref to HSI */
  WRITE_REG(RCC->MUXSELCFGR, 0); // Is that not covered by HAL_RCC_SetPllCfg()

  /* Switch OFF HSE & CSI */
  if (HAL_RCC_Xable(RCC_CK_HSE, 0) != HAL_OK)
  {
    return HAL_ERROR;
  }
  if (HAL_RCC_Xable(RCC_CK_CSI, 0) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reset MCOx configuration registers */
  CLEAR_REG(RCC->MCO1CFGR);
  CLEAR_REG(RCC->MCO2CFGR);

  /* Reset APB dividers */
  CLEAR_REG(RCC->APB1DIVR);
  CLEAR_REG(RCC->APB2DIVR);
  CLEAR_REG(RCC->APB3DIVR);
  CLEAR_REG(RCC->APB4DIVR);

  /* TODO: PLL1 ? */
  /* TODO: PLL2 ? */
  /* TODO: PLL3 ? */

  /* Adapt Systick interrupt period for 1ms */
  if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
#if O_CODE
  uint32_t tickstart;

  /* Disable PLL1 outputs */
  CLEAR_BIT(RCC->PLL1CR, RCC_PLL1CR_DIVPEN | RCC_PLL1CR_DIVQEN |
            RCC_PLL1CR_DIVREN);

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Disable PLL1 */
  CLEAR_BIT(RCC->PLL1CR, RCC_PLL1CR_PLLON);

  /* Wait till PLL is disabled */
  while ((RCC->PLL1CR & RCC_PLL1CR_PLL1RDY) != 0U)
  {
    if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Clear remaining SSCG_CTRL bit */
  CLEAR_BIT(RCC->PLL1CR, RCC_PLL1CR_SSCG_CTRL);

  /* Disable PLL2 outputs */
  CLEAR_BIT(RCC->PLL2CR, RCC_PLL2CR_DIVPEN | RCC_PLL2CR_DIVQEN |
            RCC_PLL2CR_DIVREN);

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Disable PLL2 */
  CLEAR_BIT(RCC->PLL2CR, RCC_PLL2CR_PLLON);

  /* Wait till PLL is disabled */
  while ((RCC->PLL2CR & RCC_PLL2CR_PLL2RDY) != 0U)
  {
    if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Clear remaining SSCG_CTRL bit */
  CLEAR_BIT(RCC->PLL2CR, RCC_PLL2CR_SSCG_CTRL);

  /* Disable PLL3 outputs */
  CLEAR_BIT(RCC->PLL3CR, RCC_PLL3CR_DIVPEN | RCC_PLL3CR_DIVQEN |
            RCC_PLL3CR_DIVREN);

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Disable PLL3 */
  CLEAR_BIT(RCC->PLL3CR, RCC_PLL3CR_PLLON);

  /* Wait till PLL is disabled */
  while ((RCC->PLL3CR & RCC_PLL3CR_PLL3RDY) != 0U)
  {
    if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Clear remaining SSCG_CTRL bit */
  CLEAR_BIT(RCC->PLL3CR, RCC_PLL3CR_SSCG_CTRL);

  /* Disable PLL4 outputs */
  CLEAR_BIT(RCC->PLL4CR, RCC_PLL4CR_DIVPEN | RCC_PLL4CR_DIVQEN |
            RCC_PLL4CR_DIVREN);

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Disable PLL4 */
  CLEAR_BIT(RCC->PLL4CR, RCC_PLL4CR_PLLON);

  /* Wait till PLL is disabled */
  while ((RCC->PLL4CR & RCC_PLL4CR_PLL4RDY) != 0U)
  {
    if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Clear remaining SSCG_CTRL bit */
  CLEAR_BIT(RCC->PLL4CR, RCC_PLL4CR_SSCG_CTRL);

  /* Reset PLL 1 and 2 Ref. Clock Selection Register */
  MODIFY_REG(RCC->RCK12SELR, (RCC_RCK12SELR_PLL12SRC), RCC_RCK12SELR_PLL12SRC_0);

  /* Reset RCC PLL 3 Ref. Clock Selection Register */
  MODIFY_REG(RCC->RCK3SELR, (RCC_RCK3SELR_PLL3SRC), RCC_RCK3SELR_PLL3SRC_0);

  /* Reset PLL4 Ref. Clock Selection Register */
  MODIFY_REG(RCC->RCK4SELR, (RCC_RCK4SELR_PLL4SRC), RCC_RCK4SELR_PLL4SRC_0);

  /* Reset RCC PLL1 Configuration Register 1 */
  WRITE_REG(RCC->PLL1CFGR1, 0x00010031U);

  /* Reset RCC PLL1 Configuration Register 2 */
  WRITE_REG(RCC->PLL1CFGR2, 0x00010100U);

  /* Reset RCC PLL1 Fractional Register */
  CLEAR_REG(RCC->PLL1FRACR);

  /* Reset RCC PLL1 Clock Spreading Generator Register */
  CLEAR_REG(RCC->PLL1CSGR);

  /* Reset RCC PLL2 Configuration Register 1 */
  WRITE_REG(RCC->PLL2CFGR1, 0x00010063U);

  /* Reset RCC PLL2 Configuration Register 2 */
  WRITE_REG(RCC->PLL2CFGR2, 0x00010101U);

  /* Reset RCC PLL2 Fractional Register */
  CLEAR_REG(RCC->PLL2FRACR);

  /* Reset RCC PLL2 Clock Spreading Generator Register */
  CLEAR_REG(RCC->PLL2CSGR);

  /* Reset RCC PLL3 Configuration Register 1 */
  WRITE_REG(RCC->PLL3CFGR1, 0x00010031U);

  /* Reset RCC PLL3 Configuration Register 2 */
  WRITE_REG(RCC->PLL3CFGR2, 0x00010101U);

  /* Reset RCC PLL3 Fractional Register */
  CLEAR_REG(RCC->PLL3FRACR);

  /* Reset RCC PLL3 Clock Spreading Generator Register */
  CLEAR_REG(RCC->PLL3CSGR);

  /* Reset RCC PLL4 Configuration Register 1 */
  WRITE_REG(RCC->PLL4CFGR1, 0x00010031U);

  /* Reset RCC PLL4 Configuration Register 2 */
  WRITE_REG(RCC->PLL4CFGR2, 0x00000000U);

  /* Reset RCC PLL4 Fractional Register */
  CLEAR_REG(RCC->PLL4FRACR);

  /* Reset RCC PLL4 Clock Spreading Generator Register */
  CLEAR_REG(RCC->PLL4CSGR);

  /* Reset HSIDIV once PLLs are off */
  CLEAR_BIT(RCC->HSICFGR, RCC_HSICFGR_HSIDIV);

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till HSIDIV is ready */
  while ((RCC->OCRDYR & RCC_OCRDYR_HSIDIVRDY) == 0U)
  {
    if ((int32_t)(HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Reset HSITRIM value */
  CLEAR_BIT(RCC->HSICFGR, RCC_HSICFGR_HSITRIM);

  /* Reset the Oscillator Enable Control registers */
  WRITE_REG(RCC->OCENCLRR, RCC_OCENCLRR_HSIKERON | RCC_OCENCLRR_CSION |
            RCC_OCENCLRR_CSIKERON | RCC_OCENCLRR_HSEDIGBYP | RCC_OCENCLRR_HSEON |
            RCC_OCENCLRR_HSEKERON | RCC_OCENCLRR_HSEBYP);

  /* Clear LSION bit */
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSION);

  /* Reset CSITRIM value */
  CLEAR_BIT(RCC->CSICFGR, RCC_CSICFGR_CSITRIM);

#ifdef CORE_CA35
  /* Clear RCC CPU1 Clock Source Interrupt Enable Set Register */
  CLEAR_BIT(RCC->C1CIESETR, (RCC_C1CIESETR_LSIRDYIE | RCC_C1CIESETR_LSERDYIE |
                             RCC_C1CIESETR_HSIRDYIE | RCC_C1CIESETR_HSERDYIE |
                             RCC_C1CIESETR_CSIRDYIE | RCC_C1CIESETR_PLL1DYIE | RCC_C1CIESETR_PLL2DYIE |
                             RCC_C1CIESETR_PLL3DYIE | RCC_C1CIESETR_PLL4DYIE | RCC_C1CIESETR_PLL5DYIE |
                             RCC_C1CIESETR_PLL6DYIE | RCC_C1CIESETR_PLL7DYIE | RCC_C1CIESETR_PLL8DYIE |
                             RCC_C1CIESETR_LSECSSIE | RCC_C1CIESETR_WKUPIE));

  /* Clear all MPU interrupt flags */
  /* in RCC CPU1 Clock Source Interrupt Flag Clear Register */
  SET_BIT(RCC->C1CIFCLRR, (RCC_C1CIFCLRR_LSIRDYF | RCC_C1CIFCLRR_LSERDYF |
                           RCC_C1CIFCLRR_HSIRDYF | RCC_C1CIFCLRR_HSERDYF |
                           RCC_C1CIFCLRR_CSIRDYF | RCC_C1CIFCLRR_PLL1DYF | RCC_C1CIFCLRR_PLL2DYF |
                           RCC_C1CIFCLRR_PLL3DYF | RCC_C1CIFCLRR_PLL4DYF | RCC_C1CIFCLRR_PLL5DYF |
                           RCC_C1CIFCLRR_PLL6DYF | RCC_C1CIFCLRR_PLL7DYF | RCC_C1CIFCLRR_PLL8DYF |
                           RCC_C1CIFCLRR_LSECSSF | RCC_C1CIFCLRR_WKUPF));

  /* Clear all RCC MPU Reset flags */
  /* in RCC CPU1 BOOT Reset Status Clear Register */
  SET_BIT(RCC->C1BOOTRSTSCLRR, (RCC_C1BOOTRSTSCLRR_PORRSTF |
                                RCC_C1BOOTRSTSCLRR_BORRSTF | RCC_C1BOOTRSTSCLRR_PADRSTF |
                                RCC_C1BOOTRSTSCLRR_HCSSRSTF | RCC_C1BOOTRSTSCLRR_VCORERSTF |
                                RCC_C1BOOTRSTSCLRR_VCPURSTF | RCC_C1BOOTRSTSCLRR_SYSC1RSTF |
                                RCC_C1BOOTRSTSCLRR_SYSC2RSTF | RCC_C1BOOTRSTSCLRR_IWDG1SYSRSTF |
                                RCC_C1BOOTRSTSCLRR_IWDG2SYSRSTF | RCC_C1BOOTRSTSCLRR_IWDG3SYSRSTF |
                                RCC_C1BOOTRSTSCLRR_IWDG4SYSRSTF | RCC_C1BOOTRSTSCLRR_IWDG5SYSRSTF |
                                RCC_C1BOOTRSTSCLRR_C1RSTF | RCC_C1BOOTRSTSCLRR_C1P1RSTF |
                                RCC_C1BOOTRSTSCLRR_RETCRCERRRSTF | RCC_C1BOOTRSTSCLRR_RETECCFAILCRCRSTF |
                                RCC_C1BOOTRSTSCLRR_RETECCFAILRESTRSTF | RCC_C1BOOTRSTSCLRR_STBYC1RSTF |
                                RCC_C1BOOTRSTSCLRR_D1STBYRSTF | RCC_C1BOOTRSTSCLRR_D2STBYRSTF |
                                RCC_C1BOOTRSTSCLRR_IWDG1LOCRSTF | RCC_C1BOOTRSTSCLRR_IWDG2LOCRSTF));
#endif

#ifdef CORE_CM33
  /* Clear RCC CPU2 Clock Source Interrupt Enable Register */
  CLEAR_BIT(RCC->C2CIESETR, (RCC_C2CIESETR_LSIRDYIE | RCC_C2CIESETR_LSERDYIE |
                             RCC_C2CIESETR_HSIRDYIE | RCC_C2CIESETR_HSERDYIE |
                             RCC_C2CIESETR_CSIRDYIE | RCC_C2CIESETR_PLL1DYIE | RCC_C2CIESETR_PLL2DYIE |
                             RCC_C2CIESETR_PLL3DYIE | RCC_C2CIESETR_PLL4DYIE | RCC_C2CIESETR_PLL5DYIE |
                             RCC_C2CIESETR_PLL6DYIE | RCC_C2CIESETR_PLL7DYIE | RCC_C2CIESETR_PLL8DYIE |
                             RCC_C2CIESETR_LSECSSIE | RCC_C2CIESETR_WKUPIE));

  /* Clear all MCU interrupt flags */
  /* in RCC CPU2 Clock Source Interrupt Flag Clear Register */
  SET_BIT(RCC->C2CIFCLRR, (RCC_C2CIFCLRR_LSIRDYF | RCC_C2CIFCLRR_LSERDYF |
                           RCC_C2CIFCLRR_HSIRDYF | RCC_C2CIFCLRR_HSERDYF |
                           RCC_C2CIFCLRR_CSIRDYF | RCC_C2CIFCLRR_PLL1DYF | RCC_C2CIFCLRR_PLL2DYF |
                           RCC_C2CIFCLRR_PLL3DYF | RCC_C2CIFCLRR_PLL4DYF | RCC_C2CIFCLRR_PLL5DYF |
                           RCC_C2CIFCLRR_PLL6DYF | RCC_C2CIFCLRR_PLL7DYF | RCC_C2CIFCLRR_PLL8DYF |
                           RCC_C2CIFCLRR_LSECSSF | RCC_C2CIFCLRR_WKUPF));

  /* Clear all RCC MCU Reset Flags */
  /* in RCC CPU2 BOOT Reset Status Clear Register */
  SET_BIT(RCC->C2BOOTRSTSCLRR, (RCC_C2BOOTRSTSCLRR_PORRSTF |
                                RCC_C2BOOTRSTSCLRR_BORRSTF | RCC_C2BOOTRSTSCLRR_PADRSTF |
                                RCC_C2BOOTRSTSCLRR_HCSSRSTF | RCC_C2BOOTRSTSCLRR_VCORERSTF |
                                RCC_C2BOOTRSTSCLRR_SYSC1RSTF |
                                RCC_C2BOOTRSTSCLRR_SYSC2RSTF | RCC_C2BOOTRSTSCLRR_IWDG1SYSRSTF |
                                RCC_C2BOOTRSTSCLRR_IWDG2SYSRSTF | RCC_C2BOOTRSTSCLRR_IWDG3SYSRSTF |
                                RCC_C2BOOTRSTSCLRR_IWDG4SYSRSTF | RCC_C2BOOTRSTSCLRR_IWDG5SYSRSTF |
                                RCC_C2BOOTRSTSCLRR_C2RSTF |
                                RCC_C2BOOTRSTSCLRR_RETCRCERRRSTF | RCC_C2BOOTRSTSCLRR_RETECCFAILCRCRSTF |
                                RCC_C2BOOTRSTSCLRR_RETECCFAILRESTRSTF | RCC_C2BOOTRSTSCLRR_STBYC2RSTF |
                                RCC_C2BOOTRSTSCLRR_D2STBYRSTF |
                                RCC_C2BOOTRSTSCLRR_IWDG3LOCRSTF | RCC_C2BOOTRSTSCLRR_IWDG4LOCRSTF));
#endif

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HSI_VALUE;

  /* Adapt Systick interrupt period */
  if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
  {
    return HAL_ERROR;
  }
#endif
}

/* Returns: 1 if clock ready, 0 else */
int HAL_RCC_IsReady(uint32_t clk_id)
{
  int ready;
  switch (clk_id)
  {
    case RCC_CK_HSE:
      ready = (READ_REG(RCC->OCRDYR) & RCC_OCRDYR_HSERDY);
      break;
    case RCC_CK_HSI:
      ready = (READ_REG(RCC->OCRDYR) & RCC_OCRDYR_HSIRDY);
      break;
    case RCC_CK_LSI:
      ready = (READ_REG(RCC->BDCR) & RCC_BDCR_LSIRDY);
      break;
    case RCC_CK_LSE:
      ready = (READ_REG(RCC->BDCR) & RCC_BDCR_LSERDY);
      break;
    case RCC_CK_CSI:
      ready = (READ_REG(RCC->D3DCR) & RCC_D3DCR_CSIRDY);
      break;
    default:
      return 0;
  }

  if (ready)
  {
    return 1;
  }
  return 0;
}

/* How to decide it's IN USE ?
   Returns: 1 if clock is in use, 0 else */
int HAL_RCC_OscIsInUse(RCC_OscCfgTypeDef *osc)
{
  int ready = 0;
  switch (osc->id)
  {
    case RCC_CK_HSE:
      ready = (RCC->OCRDYR & RCC_OCRDYR_HSERDY);
      break;
    case RCC_CK_HSI:
      ready = (RCC->OCRDYR & RCC_OCRDYR_HSIRDY);
      break;
    case RCC_CK_LSI:
      ready = (RCC->BDCR & RCC_BDCR_LSIRDY);
      break;
    case RCC_CK_LSE:
      ready = (RCC->BDCR & RCC_BDCR_LSERDY);
      break;
    case RCC_CK_CSI:
      ready = (RCC->D3DCR & RCC_D3DCR_CSIRDY);
      break;
    default:
      return HAL_ERROR;
  }

  return ready;
}

/* Enable or disable clock and wait for completion.
 * @param  clk_id: Clock ID
 * @param  enable: 1 to enable, 0 to disable
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RCC_Xable(uint32_t clk_id, int enable)
{
  switch (clk_id)
  {
    case RCC_CK_HSE:
    case RCC_CK_HSE_KER:
      if (enable)
      {
        SET_BIT(RCC->OCENSETR, RCC_OCENSETR_HSEON);
        if (RCC_Wait((void *)&RCC->OCRDYR, RCC_OCRDYR_HSERDY, RCC_OCRDYR_HSERDY, HSE_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      else
      {
        WRITE_REG(RCC->OCENCLRR, RCC_OCENCLRR_HSEON);
        if (RCC_Wait((void *)&RCC->OCRDYR, RCC_OCRDYR_HSERDY, 0, HSE_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      break;
    case RCC_CK_HSI:
    case RCC_CK_HSI_KER:
      if (enable)
      {
        SET_BIT(RCC->OCENSETR, RCC_OCENSETR_HSION);
        if (RCC_Wait((void *)&RCC->OCRDYR, RCC_OCRDYR_HSIRDY, RCC_OCRDYR_HSIRDY, HSI_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      else
      {
        WRITE_REG(RCC->OCENCLRR, RCC_OCENCLRR_HSION);
        if (RCC_Wait((void *)&RCC->OCRDYR, RCC_OCRDYR_HSIRDY, 0, HSI_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      break;
    case RCC_CK_CSI:
    case RCC_CK_CSI_KER:
      if (enable)
      {
        SET_BIT(RCC->D3DCR, RCC_D3DCR_CSION);
        if (RCC_Wait((void *)&RCC->D3DCR, RCC_D3DCR_CSIRDY, RCC_D3DCR_CSIRDY, CSI_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      else
      {
        CLEAR_BIT(RCC->D3DCR, RCC_D3DCR_CSION);
        if (RCC_Wait((void *)&RCC->D3DCR, RCC_D3DCR_CSIRDY, 0, CSI_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      break;
    case RCC_CK_LSI:
      RCC_XableProtection(0); /* Disable BDCR protection */
      if (enable)
      {
        SET_BIT(RCC->BDCR, RCC_BDCR_LSION);
        if (RCC_Wait((void *)&RCC->BDCR, RCC_BDCR_LSIRDY, RCC_BDCR_LSIRDY, LSI_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      else
      {
        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSION);
        if (RCC_Wait((void *)&RCC->BDCR, RCC_BDCR_LSIRDY, 0, LSI_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      RCC_XableProtection(1);
      break;
    case RCC_CK_LSE:
      RCC_XableProtection(0); /* Disable BDCR protection */
      if (enable)
      {
        SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
        if (RCC_Wait((void *)&RCC->BDCR, RCC_BDCR_LSERDY, RCC_BDCR_LSERDY, RCC_LSE_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      else
      {
        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);
        if (RCC_Wait((void *)&RCC->BDCR, RCC_BDCR_LSERDY, 0, RCC_LSE_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      RCC_XableProtection(1);
      break;
    /* Flexiclockgen clocks */
    case RCC_CK_PLL4 ... RCC_CK_PLL8:
    case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
      if (RCC_FG_Xable(clk_id, enable) != 0)
      {
        return HAL_ERROR;
      }
      break;
    case RCC_CK_RTC:
      RCC_XableProtection(0); // Disable write protection
      if (enable)
      {
        SET_BIT(RCC->BDCR, RCC_BDCR_RTCCKEN);
      }
      else
        CLEAR_BIT(RCC->BDCR, RCC_BDCR_RTCCKEN);
      RCC_XableProtection(1); // Reenable write protection
      break;
#ifdef CORE_CA35
    case RCC_CK_PLL1:
    {
      __HAL_RCC_SYSCPU1_CLK_ENABLE(); // Required for SSC access
#define A3x_SS_PLL_ENABLE 0xa0
      /* TODO: To be updated when A53SSC_BASE exists */
      volatile void *a53_pll_en = (void *)A35SSC_BASE + A3x_SS_PLL_ENABLE;
      if (enable)
      {
        SET_BITA(a53_pll_en, (1 << 0)); // Set PLL_EN
        if (RCC_Wait(a53_pll_en, (1 << 1), (1 << 1), PLL_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
      else
      {
        CLEAR_BITA(a53_pll_en, (1 << 0)); // Clear PLL_EN
        if (RCC_Wait(a53_pll_en, (1 << 1), 0, PLL_TIMEOUT_VALUE) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    break;
#endif
    default:
      return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Initializes the RCC Oscillators according to the specified parameters in the
  *         RCC_OscCfgTypeDef.
  * @param  osc: pointer to a 'RCC_OscCfgTypeDef' structure that
  *         containing the oscilator configuration information.
  * @retval HAL status
  */
__weak HAL_StatusTypeDef HAL_RCC_SetOscCfg(RCC_OscCfgTypeDef *osc)
{
  /* Some checks */
  assert_param(osc != NULL);
  assert_param(osc->id != RCC_CK_NONE);
  assert_param(osc->id < RCC_CK_LAST);
  assert_param(RCC_VALID_OSCSTATE(osc->state));
  /* Note: don't check state since real HW status may not be known
  if (HAL_RCC_OscIsReady(osc) && (osc->state != RCC_OSCSTATE_ON))
      return HAL_ERROR; // Unexpected state
  if (!HAL_RCC_OscIsReady(osc) && (osc->state == RCC_OSCSTATE_ON))
      return HAL_ERROR; // Unexpected state
  */

  HAL_StatusTypeDef status;
  switch (osc->id)
  {
    case RCC_CK_HSE:
      /* HSE Configuration:
         If in use, do nothing. */
      if (!HAL_RCC_OscIsInUse(osc))
      {
        /* Configure HSE oscillator */
        /* Disable HSEON before configuring the HSE --------------*/
        status = HAL_RCC_Xable(RCC_CK_HSE, 0);
        if (status != HAL_OK)
        {
          return status;
        }

        /* Clear remaining bits */

        WRITE_REG(RCC->OCENCLRR, (RCC_OCENCLRR_HSEDIGBYP | RCC_OCENCLRR_HSEBYP));

        /* Enable HSE if needed ---------------------------------------*/
        if (osc->state != RCC_OSCSTATE_OFF)
        {
          if (osc->state == RCC_OSCSTATE_BYPASS)
          {
            SET_BIT(RCC->OCENSETR, RCC_OCENSETR_HSEBYP);
          }
          else if (osc->state == RCC_OSCSTATE_DIGBYPASS)
          {
            SET_BIT(RCC->OCENSETR, RCC_OCENSETR_HSEDIGBYP | RCC_OCENSETR_HSEBYP);
          }

          /* Enable oscillator */
          status = HAL_RCC_Xable(RCC_CK_HSE, 1);
          if (status != HAL_OK)
          {
            return status;
          }
        }
      }
      break;
    case RCC_CK_HSI:
      /* HSI Configuration
         If OFF requested and in use, do nothing */
      if (osc->state != RCC_OSCSTATE_OFF)
      {
        /* Enable the Internal High Speed oscillator (HSI). */
        status = HAL_RCC_Xable(RCC_CK_HSI, 1);
        if (status != HAL_OK)
        {
          return status;
        }

        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(osc->calibrationvalue);
      }
      else if (!HAL_RCC_OscIsInUse(osc))
      {
        /* Disable the Internal High Speed oscillator (HSI). */
        status = HAL_RCC_Xable(RCC_CK_HSI, 0);
        if (status != HAL_OK)
        {
          return status;
        }
      }
      break;
    case RCC_CK_CSI:
      /* CSI Configuration
         Disabled only if unused */
      if (osc->state != RCC_OSCSTATE_OFF)
      {
        /* Enable the Internal High Speed oscillator (CSI). */
        status = HAL_RCC_Xable(RCC_CK_CSI, 1);
        if (status != HAL_OK)
        {
          return status;
        }

        /* Adjusts the Internal High Speed oscillator (CSI) calibration value.*/
        __HAL_RCC_CSI_CALIBRATIONVALUE_ADJUST(osc->calibrationvalue);
      }
      else if (!HAL_RCC_OscIsInUse(osc))
      {
        /* Disable the Internal High Speed oscillator (CSI). */
        status = HAL_RCC_Xable(RCC_CK_CSI, 0);
        if (status != HAL_OK)
        {
          return status;
        }
      }
      else
      {
        /* Adjusts the Internal High Speed oscillator (CSI) calibration value.*/
        __HAL_RCC_CSI_CALIBRATIONVALUE_ADJUST(osc->calibrationvalue);
      }
      break;
    case RCC_CK_LSI:
      /* LSI Configuration */
      if (osc->state != RCC_OSCSTATE_OFF)
      {
        /* Enable the Internal Low Speed oscillator (LSI). */
        status = HAL_RCC_Xable(RCC_CK_LSI, 1);
        if (status != HAL_OK)
        {
          return status;
        }
      }
      else
      {
        /* Disable the Internal Low Speed oscillator (LSI). */
        status = HAL_RCC_Xable(RCC_CK_LSI, 0);
        if (status != HAL_OK)
        {
          return status;
        }
      }
      break;
    case RCC_CK_LSE:
      /* LSE Configuration */
      if (osc->drivevalue > RCC_OSCDRIVE_HIGH)
      {
        return HAL_ERROR;  // Invalid param
      }

      /* Turning LSE off is needed before configuring */
      status = HAL_RCC_Xable(RCC_CK_LSE, 0);
      if (status != HAL_OK)
      {
        return status;
      }

      RCC_XableProtection(0); // Disable BDCR protection

      /* Clear remaining bits */
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEDIGBYP | RCC_BDCR_LSEBYP);

      /* Enable LSE if needed */
      if (osc->state != RCC_OSCSTATE_OFF)
      {
        if (osc->state == RCC_OSCSTATE_BYPASS)
        {
          SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);
        }
        else if (osc->state == RCC_OSCSTATE_DIGBYPASS)
        {
          SET_BIT(RCC->BDCR, RCC_BDCR_LSEDIGBYP | RCC_BDCR_LSEBYP);
        }

        MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEDRV, osc->drivevalue << RCC_BDCR_LSEDRV_Pos);

        /* Enable oscillator */
        status = HAL_RCC_Xable(RCC_CK_LSE, 1);
        if (status != HAL_OK)
        {
          return status;
        }
      }

      RCC_XableProtection(1); // Restore BDCR protection
      break;
    default:
      return HAL_ERROR; // Unexpected clock
  }

  return HAL_OK;
}

/* Get PLL2 or PLL3 config (PLLTS16FFCLAFRAC2) */
static HAL_StatusTypeDef RCC_GetPLL23(RCC_PLLCfgTypeDef *pll)
{
  assert_param(pll != NULL);
  assert_param(pll->id != RCC_CK_NONE);
  assert_param(pll->id < RCC_CK_LAST);

  void *cfgr1, *cfgr2, *cfgr3, *cfgr4, *cfgr5, *cfgr6, *cfgr7;
  switch (pll->id)
  {
    case RCC_CK_PLL2:
      cfgr1 = (void *)&RCC->PLL2CFGR1;
      cfgr2 = (void *)&RCC->PLL2CFGR2;
      cfgr3 = (void *)&RCC->PLL2CFGR3;
      cfgr4 = (void *)&RCC->PLL2CFGR4;
      cfgr5 = (void *)&RCC->PLL2CFGR5;
      cfgr6 = (void *)&RCC->PLL2CFGR6;
      cfgr7 = (void *)&RCC->PLL2CFGR7;
      break;
    case RCC_CK_PLL3:
      cfgr1 = (void *)&RCC->PLL3CFGR1;
      cfgr2 = (void *)&RCC->PLL3CFGR2;
      cfgr3 = (void *)&RCC->PLL3CFGR3;
      cfgr4 = (void *)&RCC->PLL3CFGR4;
      cfgr5 = (void *)&RCC->PLL3CFGR5;
      cfgr6 = (void *)&RCC->PLL3CFGR6;
      cfgr7 = (void *)&RCC->PLL3CFGR7;
      break;
    default:
      return -1; // Invalid PLL
  }

  /* Source identification.
     REMINDER: PLL is statically connected to xxREF. PLL source identification
               must therefore be done on xxREF clock */
  uint16_t ref_id; // PLL ref clock (RCC_CK_PLLxREF)
  ref_id = RCC_CK_PLL2REF + (pll->id - RCC_CK_PLL2);
  if (RCC_FG_GetSource(ref_id, &pll->source) != 0)
  {
    return -1;
  }

  uint32_t val;

  /* State ?
     Note: ON means enabled (PLLEN) and ready (PLLRDY)
     Note: Bypass state does not reflect PLL state (may be ON or OFF but unused) */
  pll->state = 0;
  if (READ_REGA(cfgr4) & (1 << 10)) // Bypassed ?
  {
    pll->state |= RCC_PLLSTATE_BYPASS;
  }
  val = READ_REGA(cfgr1);
  if ((val & (1 << 8)) && (val & (1 << 24))) // PLLEN=1 + PLLRDY=1
  {
    pll->state |= RCC_PLLSTATE_ON;
  }
  else
    pll->state |= RCC_PLLSTATE_OFF;

  pll->flags = 0; // Integer, DAC disabled, SSCG disabled, SSCG center

  val = READ_REGA(cfgr2);
  pll->frefdiv = (val >> 0) & 0x3f;
  pll->fbdiv = (val >> 16) & 0xfff;

  /* Frac or integer ? */
  val = READ_REGA(cfgr4);
  if (val & (1 << 8)) // DSMEN
  {
    pll->flags |= RCC_PLLFLAG_FRACTIONAL;
  }
  val = READ_REGA(cfgr3);
  pll->frac_in = val & 0xffffff;
  if (val & (1 << 25))
  {
    pll->flags |= RCC_PLLFLAG_DAC_ON;
  }

  /* SSCG status */
  val = READ_REGA(cfgr5);
  pll->sscg_moddepth = (val >> 16) & 0x1f; // Modulation depth
  pll->sscg_modfreq = (val >> 0) & 0xf; // Modulation freq
  val = READ_REGA(cfgr3);
  if (val & (1 << 24)) // DOWNSPREAD
  {
    pll->flags |= RCC_PLLFLAG_SSCG_DOWN;
  }
  if ((val & (1 << 26)) == 0) // SSCGDIS
  {
    pll->flags |= RCC_PLLFLAG_SSCG_ON;
  }

  /* Output status */
  if (READ_REGA(cfgr4) & (1 << 9))   // FOUTPOSTDIVEN
  {
    pll->postdiv1 = READ_REGA(cfgr6) & 0x7;
    pll->postdiv2 = READ_REGA(cfgr7) & 0x7;
  }
  else
  {
    /* Fout postdiv disabled => resulting div = 1 */
    pll->postdiv1 = 1;
    pll->postdiv2 = 1;
  }

  return 0;
}

/* Configure PLL2 or PLL3 (PLLTS16FFCLAFRAC2) */
static HAL_StatusTypeDef RCC_SetPLL23(RCC_PLLCfgTypeDef *pll)
{
  assert_param(pll != NULL);
  assert_param(pll->id != RCC_CK_NONE);
  assert_param(pll->id < RCC_CK_LAST);
  assert_param(pll->source != RCC_CK_NONE);
  assert_param(pll->source < RCC_CK_LAST);

  void *cfgr1, *cfgr2, *cfgr3, *cfgr4, *cfgr5, *cfgr6, *cfgr7;
  uint32_t ref_id; // PLL ref clock ID
  switch (pll->id)
  {
    case RCC_CK_PLL2:
      cfgr1 = (void *)&RCC->PLL2CFGR1;
      cfgr2 = (void *)&RCC->PLL2CFGR2;
      cfgr3 = (void *)&RCC->PLL2CFGR3;
      cfgr4 = (void *)&RCC->PLL2CFGR4;
      cfgr5 = (void *)&RCC->PLL2CFGR5;
      cfgr6 = (void *)&RCC->PLL2CFGR6;
      cfgr7 = (void *)&RCC->PLL2CFGR7;
      ref_id = RCC_CK_PLL2REF;
      break;
    case RCC_CK_PLL3:
      cfgr1 = (void *)&RCC->PLL3CFGR1;
      cfgr2 = (void *)&RCC->PLL3CFGR2;
      cfgr3 = (void *)&RCC->PLL3CFGR3;
      cfgr4 = (void *)&RCC->PLL3CFGR4;
      cfgr5 = (void *)&RCC->PLL3CFGR5;
      cfgr6 = (void *)&RCC->PLL3CFGR6;
      cfgr7 = (void *)&RCC->PLL3CFGR7;
      ref_id = RCC_CK_PLL3REF;
      break;
    default:
      return HAL_ERROR; // Invalid PLL
  }

  /* Checking PLL source clock ID.
     Note: It can only be one of the MUXSEL external outputs */
  int in_idx;
  in_idx = RCC_FG_GetClockIdx(pll->source, RCC_Flexgen.ref_table, RCC_Flexgen.ref_size);
  if (in_idx == -1)
  {
    return HAL_ERROR;  // Invalid source
  }

  /* PLLTS16FFCLAFRAC2 setup order:
     - Set bypass (supported by PLL1 ?)
     - Disable postdiv (supported by PLL1 ?) then VCO
     - Select source
     - Configure PLL
     - Enable PLL if RCC_PLLSTATE_ON
     - Remove bypass if NOT RCC_PLLSTATE_BYPASS (supported by PLL1 ?)
   */

  /* Check FREF
     Must be running and in good range */
  /* TODO */

  /* Bypass, in case PLL is used */
  SET_BITA(cfgr4, (1 << 10));

  /* Disable post div then PLL */
  CLEAR_BITA(cfgr4, 1 << 9); // FOUTPOSTDIVEN=0
  CLEAR_BITA(cfgr1, 1 << 8); // PLLEN

  /* Select PLL ref source */
  if (RCC_FG_SetSource(ref_id, pll->source) != 0)
  {
    return HAL_ERROR;
  }

  /* Configure PLL */
  uint32_t sscg_dis, dacen, downspread;
  sscg_dis = (pll->flags & RCC_PLLFLAG_SSCG_ON) ? 0 : 1;
  dacen = (pll->flags & RCC_PLLFLAG_DAC_ON) ? 1 : 0;
  downspread = (pll->flags & RCC_PLLFLAG_SSCG_DOWN) ? 1 : 0;
  uint32_t dsmen, postdiven;
  dsmen = (pll->flags & RCC_PLLFLAG_FRACTIONAL) ? 1 : 0;
  if ((!pll->postdiv1 && !pll->postdiv2) ||
      ((pll->postdiv1 == 1) && (pll->postdiv2 == 1)))
  {
    postdiven = 0;  // FOUTPOSTDIVEN=0
  }
  else
    postdiven = 1; // FOUTPOSTDIVEN=1
  WRITE_REGA(cfgr2, (pll->fbdiv << 16) | pll->frefdiv);
  WRITE_REGA(cfgr3, (sscg_dis << 26) | (dacen << 25) | (downspread << 24) | pll->frac_in);
  WRITE_REGA(cfgr4, (1 << 10) | (postdiven << 9) | (dsmen << 8)); // Keep bypass + foutpostdiven
  WRITE_REGA(cfgr5, (pll->sscg_moddepth << 16) | pll->sscg_modfreq);
  WRITE_REGA(cfgr6, pll->postdiv1); // Postdiv1
  WRITE_REGA(cfgr7, pll->postdiv2); // Postdiv2

  if (pll->state & RCC_PLLSTATE_ON)   // VCO ON
  {
    /* Enable PLL */
    SET_BITA(cfgr1, 1 << 8); // PLLEN
    /* Wait for lock */
    if (RCC_Wait(cfgr1, 1 << 24, 1 << 24, PLL_TIMEOUT_VALUE) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }

  /* Remove bypass */
  if ((pll->state & RCC_PLLSTATE_BYPASS) == 0)
  {
    CLEAR_BITA(cfgr4, (1 << 10));  // Remove bypass
  }

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_GetPllCfg(RCC_PLLCfgTypeDef *pll)
{
  switch (pll->id)
  {
    case RCC_CK_PLL4 ... RCC_CK_PLL8:
      if (RCC_FG_GetGFG(pll) != 0)
      {
        return HAL_ERROR;
      }
      break;
    case RCC_CK_PLL2 ... RCC_CK_PLL3:
      if (RCC_GetPLL23(pll) != 0)
      {
        return HAL_ERROR;
      }
      break;
    default:
      return HAL_ERROR; // Invalid clock
  }

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_SetPllCfg(RCC_PLLCfgTypeDef *pll)
{
  switch (pll->id)
  {
    case RCC_CK_PLL4 ... RCC_CK_PLL8:
      if (RCC_FG_SetGFG(pll) != 0)
      {
        return HAL_ERROR;
      }
      break;
    case RCC_CK_PLL2 ... RCC_CK_PLL3:
      if (RCC_SetPLL23(pll) != 0)
      {
        return HAL_ERROR;
      }
      break;
#ifdef CORE_CA35
    case RCC_CK_PLL1:
    {
      /* PLLTS16FFCLAFRAC2 setup order:
         - Set bypass (supported by PLL1 ?)
         - Disable postdiv (supported by PLL1 ?) then VCO
         - Select source
         - Configure PLL
         - Enable PLL if RCC_PLLSTATE_ON
         - Remove bypass if NOT RCC_PLLSTATE_BYPASS (supported by PLL1 ?)
       */
      if (HAL_RCC_Xable(RCC_CK_PLL1, 0) != HAL_OK)
      {
        return HAL_ERROR;
      }

      /* Select PLL1 source
         Reminder: MUXSEL5=>PLL1 */
      int src_idx;
      src_idx = RCC_FG_GetClockIdx(pll->source, RCC_Flexgen.ref_table, RCC_Flexgen.ref_size);
      if (src_idx == -1)
      {
        return HAL_ERROR;
      }
      uint32_t val;
      val = READ_REG(RCC->MUXSELCFGR) & ~(0x3 << 20); // Get current switch value + clear muxsel5
      WRITE_REG(RCC->MUXSELCFGR, val | (src_idx << 20));

      /* TODO: Configuration to be completed.
         Does PLL1 supports fractional ? */

      /* Enable PLL if RCC_PLLSTATE_ON */
      if (pll->state & RCC_PLLSTATE_ON)
        if (HAL_RCC_Xable(RCC_CK_PLL1, 1) != HAL_OK)
        {
          return HAL_ERROR;
        }
    }
    break;
#endif
    default:
      return HAL_ERROR; // Invalid clock
  }

  return HAL_OK;
}

/* Get clock config (status, source & div).
   Returns: HAL status */
HAL_StatusTypeDef HAL_RCC_GetClockCfg(RCC_ClkCfgTypeDef *clk)
{
  switch (clk->id)
  {

    /* Flexiclockgen outputs */
    case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
      /* Get source */
      if (RCC_FG_GetSource(clk->id, &clk->source) != 0)
      {
        return HAL_ERROR;
      }
      /* Get div */
      if (RCC_FG_GetDiv(clk->id, &clk->divvalue) != 0)
      {
        return HAL_ERROR;
      }
      /* Enabled ? */
      int enabled;
      if (RCC_FG_GetStatus(clk->id, &enabled) != 0)
      {
        return HAL_ERROR;
      }
      if (enabled)
      {
        clk->state = RCC_CLKSTATE_ON;
      }
      else
        clk->state = RCC_CLKSTATE_OFF;
      break;

    /* Flexiclockgen observation paths */
    case RCC_CK_OBS0 ... RCC_CK_OBS1:
      if (RCC_FG_GetObs(clk->id, &clk->source, &clk->divvalue) != 0)
      {
        return HAL_ERROR;
      }
      break;

    /* Bus clocks */
    case RCC_CK_ICN_APB1 ... RCC_CK_ICN_APBDBG:
      clk->source = HAL_RCC_GetSource(clk->id);
      clk->state = RCC_CLKSTATE_ON;
      {
        void *addr;
        if (clk->id < RCC_CK_ICN_APB4)
        {
          addr = (void *)&RCC->APB1DIVR + (clk->id - RCC_CK_ICN_APB1) * 4;
        }
        else
          addr = (void *) { &RCC->APB4DIVR + (clk->id - RCC_CK_ICN_APB4) * 4; }
                 /* Possible div values: 1, 2, 4, 8, or 16 */
                 clk->divvalue = 1 << READ_REGA(addr);
      }
      break;

    case RCC_CK_RTC:
      clk->source = HAL_RCC_GetSource(clk->id);
      if (READ_REG(RCC->BDCR) & (1 << 20))
      {
        clk->state = RCC_CLKSTATE_ON;
      }
      else
        clk->state = RCC_CLKSTATE_OFF;
      clk->divvalue = READ_REG(RCC->RTCDIVR) + 1;
      break;

    default:
      return HAL_ERROR;
  }

  return HAL_OK;
}

/* Set clock config (status, source & div).
   Returns: HAL status */
HAL_StatusTypeDef HAL_RCC_SetClockCfg(RCC_ClkCfgTypeDef *clk)
{
  /* Some params checks */
  assert_param(clk != NULL);
  assert_param(clk->id != RCC_CK_NONE);
  assert_param(clk->source != RCC_CK_NONE);
  assert_param(clk->state != RCC_CLKSTATE_UNDEF);

  /* TODO: Must add security to
     - set source then div
     - or div then source
     according to intermediate freq which should NOT be greater than target
     to avoid child IPs hangs */

  switch (clk->id)
  {

    /* Flexiclockgen outputs */
    case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
      /* Configure source */
      if (RCC_FG_SetSource(clk->id, clk->source) != 0)
      {
        return HAL_ERROR;
      }
      /* Configure div */
      if (RCC_FG_SetDiv(clk->id, clk->divvalue) != 0)
      {
        return HAL_ERROR;
      }
      /* Enable */
      if (RCC_FG_Xable(clk->id, 1) != 0)
      {
        return HAL_ERROR;
      }
      break;

    /* Flexiclockgen observation paths */
    case RCC_CK_OBS0 ... RCC_CK_OBS1:
      if (RCC_FG_SetObs(clk->id, clk->source, &clk->divvalue) != 0)
      {
        return HAL_ERROR;
      }
      break;

    /* Bus clocks */
    case RCC_CK_ICN_APB1 ... RCC_CK_ICN_APBDBG:
    {
      /* Check & convert 'divvalue' */
      uint32_t val;
      switch (clk->divvalue)
      {
        case 1:
          val = 0;
          break;
        case 2:
          val = 1;
          break;
        case 4:
          val = 2;
          break;
        case 8:
          val = 3;
          break;
        case 16:
          val = 4;
          break;
        default:
          return HAL_ERROR; // Invalid divider value
      }

      void *addr;
      if (clk->id < RCC_CK_ICN_APB4)
      {
        addr = (void *)&RCC->APB1DIVR + (clk->id - RCC_CK_ICN_APB1) * 4;
      }
      else
        addr = (void *) { &RCC->APB4DIVR + (clk->id - RCC_CK_ICN_APB4) * 4; }

               /* Configure switch & wait for completion */
               WRITE_REGA(addr, val);
      if (RCC_Wait(addr, 1 << 31, 1 << 31, CLOCKSWITCH_TIMEOUT_VALUE) != HAL_OK)
      {
        return HAL_TIMEOUT;
      }
    }
    break;

    case RCC_CK_RTC:
      if (clk->divvalue == 0)
      {
        return HAL_ERROR;  // Invalid div value
      }
      if (clk->divvalue > 64)
      {
        return HAL_ERROR;  // Invalid div value
      }
      WRITE_REG(RCC->RTCDIVR, clk->divvalue - 1); // HSE divider
      /* BDCR->RTCSRC WARNING !
         This field can be written only one time after backup domain reset. */
      RCC_XableProtection(0); // Disable write protection
      uint32_t exp;
      switch (clk->source)
      {
        case RCC_CK_LSE:
          exp = 1;
          break;
        case RCC_CK_LSI:
          exp = 2;
          break;
        case RCC_CK_HSE:
          exp = 3;
          break;
        default:
          exp = 0;
          break; // No source
      }
      /* Modify BDCR & checks RTSRC properly changed */
      MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSRC, (exp << RCC_BDCR_RTCSRC_Pos));
      if ((READ_REG(RCC->BDCR) & RCC_BDCR_RTCSRC) != (exp << RCC_BDCR_RTCSRC_Pos))
      {
        RCC_XableProtection(1); // Reenable write protection
        return HAL_ERROR;
      }
      if (clk->state == RCC_CLKSTATE_ON)
      {
        SET_BIT(RCC->BDCR, RCC_BDCR_RTCCKEN);
      }
      else
        CLEAR_BIT(RCC->BDCR, RCC_BDCR_RTCCKEN);
      RCC_XableProtection(1); // Reenable write protection
      break;

    default:
      return HAL_ERROR;
  }

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_SetCA35SSCfg(RCC_MuxCfgTypeDef *mux)
{
  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_GetCA35SSCfg(RCC_MuxCfgTypeDef *mux)
{
  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_SetBusClockCfg(RCC_ClkCfgTypeDef *clk)
{
  /* Handles the bus dividers, including LSMCUDIV */

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_SetPeriphClockCfg(RCC_ClkCfgTypeDef *clk)
{
  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_GetBusClockCfg(RCC_ClkCfgTypeDef *clk)
{
  /* Handles the bus dividers, including LSMCUDIV */

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_GetPeriphClockCfg(RCC_ClkCfgTypeDef *clk)
{
  return HAL_OK;
}

/* OBI split into two functions: one for LSE and one for HSE */
/**
  * @brief  Enables the HSE Clock Security System.
  * @note   If a failure is detected on the HSE:
  *         - The RCC generates a system reset (nreset). The flag HCSSRSTF is set in order to
  *           allow the application to identify the reset root cause.
  *         - A failure event is generated (rcc_hsecss_fail). This event is connected to the TAMP
  *           block, allowing the protection of backup registers and BKPSRAM.
  *
  * @note   HSECSSON can be activated even when the HSEON is set to ‘0’. The CSS on HSE will be
  *         enabled by the hardware when the HSE is enabled and ready, and HSECSSON is set to ‘1’.
  *
  * @note   The HSECSS is disabled when the HSE is disabled (i.e. when the system is STOP or
  *         STANDBY).
  *
  * @note   It is not possible to clear the bit HSECSSON by software.
  *
  * @note   The bit HSECSSON will be cleared by the hardware when a system reset occurs or when
            the system enters in STANDBY mode
  * @retval None
  */
void HAL_RCC_EnableCSS(uint16_t clk_id)
{
  switch (clk_id)
  {
    case RCC_CK_HSE:
      SET_BIT(RCC->OCENSETR, RCC_OCENSETR_HSECSSON);
      break;
    case RCC_CK_LSE:
      SET_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
      break;
    default:
      break; // Unsupported case
  }
}

void HAL_RCC_EnableHSECSS(void)
{
  SET_BIT(RCC->OCENSETR, RCC_OCENSETR_HSECSSON);
}

void HAL_RCC_EnableLSECSS(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
}

/**
  * @brief  Returns the SYSCLK frequency
  *
  * @note   The system frequency computed by this function is not the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note     If SYSCLK source is CSI, function returns values based on CSI_VALUE(*)
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(**)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(***)
  * @note     If SYSCLK source is PLL, function returns values based on CSI_VALUE(*),
  *           HSI_VALUE(**) or HSE_VALUE(***) multiplied/divided by the PLL factors.
  * @note     (*) CSI_VALUE is a constant defined in stm32mp1xx_hal_conf.h file (default value
  *               4 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSI_VALUE is a constant defined in stm32mp1xx_hal_conf.h file (default value
  *               64 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (***) HSE_VALUE is a constant defined in stm32mp1xx_hal_conf.h file (default value
  *                24 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *
  * @note   This function can be used by the user application to compute the
  *         baud rate for the communication peripherals or configure other parameters.
  *
  * @note   Each time SYSCLK changes, this function must be called to update the
  *         right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *
  * @retval SYSCLK frequency
  */
uint32_t HAL_RCC_GetSysClockFreq(void)
{
  return HAL_RCC_GetFreq(RCC_CK_HSI);
}

HAL_StatusTypeDef HAL_RCC_GetOscCfg(RCC_OscCfgTypeDef *osc)
{
  assert_param(osc != NULL);
  assert_param(osc->id != RCC_CK_NONE);

  /* Cleaning some parameters */
  osc->calibrationvalue = 0;
  osc->divvalue = 0; // Used by CSI only
  osc->drivevalue = 0; // Used by LSE only

  switch (osc->id)
  {
    case  RCC_CK_LSI:
      if (READ_REG(RCC->BDCR) & RCC_BDCR_LSION)
      {
        osc->state = RCC_OSCSTATE_ON;
      }
      else
        osc->state = RCC_OSCSTATE_OFF;
      break;
    case RCC_CK_LSE:
    {
      uint32_t reg;
      reg = READ_REG(RCC->BDCR);
      if ((reg & RCC_BDCR_LSEON) == 0)
      {
        osc->state = RCC_OSCSTATE_OFF;
      }
      else if ((reg & RCC_BDCR_LSEBYP) == 0)
      {
        osc->state = RCC_OSCSTATE_ON;
      }
      else if ((reg & RCC_BDCR_LSEDIGBYP) == 0)
      {
        osc->state = RCC_OSCSTATE_BYPASS;
      }
      else
        osc->state = RCC_OSCSTATE_DIGBYPASS;
      osc->drivevalue = (reg >> 4) & 3;
    }
    break;
    case RCC_CK_CSI:
      if (READ_REG(RCC->D3DCR) & RCC_D3DCR_CSION)
      {
        osc->state = RCC_OSCSTATE_ON;
      }
      else
        osc->state = RCC_OSCSTATE_OFF;
      osc->calibrationvalue =
        (uint32_t)((READ_REG(RCC->CSICFGR) & RCC_CSICFGR_CSITRIM) >> RCC_CSICFGR_CSITRIM_Pos);
      break;
    case RCC_CK_HSE:
      if (READ_REG(RCC->OCENSETR) & RCC_OCENSETR_HSEBYP)
      {
        osc->state = RCC_OSCSTATE_BYPASS;
      }
      else if (READ_REG(RCC->OCENSETR) & RCC_OCENSETR_HSEON)
      {
        osc->state = RCC_OSCSTATE_ON;
      }
      else
        osc->state = RCC_OSCSTATE_OFF;
      break;
    case RCC_CK_HSI:
      if (READ_REG(RCC->OCENSETR) & RCC_OCENSETR_HSION)
      {
        osc->state = RCC_OSCSTATE_ON;
      }
      else
        osc->state = RCC_OSCSTATE_OFF;
      osc->calibrationvalue =
        (uint32_t)((READ_REG(RCC->HSICFGR) & RCC_HSICFGR_HSITRIM) >> RCC_HSICFGR_HSITRIM_Pos);
      break;
  }

  return HAL_OK;
}

/**
  * @brief  Returns the System Core frequency
  *
  * @note   The system frequency computed by this function is not the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constants and the selected clock source
  * @retval System Core frequency
  */
uint32_t HAL_RCC_GetSystemCoreClockFreq(void)
{
// #ifdef CORE_CA35
  // return HAL_RCC_GetMPUSSFreq();
// #else // CORE_CM33
  // return HAL_RCC_GetMCUFreq();
// #endif
  return 12; // FCh: TODO
}

/* OBI: Split this function into GetBusClockFreq & GetPeriphClockFreq & GetPllClockFreq & GetOscClockFreq functions */
/* This function can then be removed */
/* Returns 'clk_id' frequency.
   Should we return 0 if disabled ??? */
uint32_t HAL_RCC_GetFreq(uint32_t clk_id)
{
  uint16_t src_id;
  uint32_t sfreq;
  uint32_t freq = 0;
  switch (clk_id)
  {
    /* Oscillators */
    case RCC_CK_HSE:
    case RCC_CK_HSE_KER:
      freq = HSE_VALUE;
      break;
    case RCC_CK_HSI:
    case RCC_CK_HSI_KER:
      freq = HSI_VALUE;
      break;
    case RCC_CK_LSE:
      freq = LSE_VALUE;
      break;
    case RCC_CK_LSI:
      freq = LSI_VALUE;
      break;
    case RCC_CK_CSI:
    case RCC_CK_CSI_KER:
      /* TODO: Missing 4 vs 16M support. CSI_VALUE should be 16M then */
      freq = CSI_VALUE;
      break;
    /* Flexgen: Muxsel outputs */
    case RCC_CK_PLL4REF ... RCC_CK_PLL3REF:
      RCC_FG_GetSource(clk_id, &src_id);
      freq = HAL_RCC_GetFreq(src_id);
      break;
    /* Flexgen: internal PLLs */
    case RCC_CK_PLL4 ... RCC_CK_PLL8:
    {
      RCC_PLLCfgTypeDef pll;
      pll.id = clk_id;
      RCC_FG_GetGFG(&pll);
      switch (clk_id)
      {
          /* In case of Emulator platform, PLL output clocks are forced. */
          /* PLL frequencies shall be defined in include 'stm32mp2xx_clocks.h' */
          /* in BSP/STM32MP257CXX_EMU directory */
#if defined (USE_STM32MP257CXX_EMU)
#ifdef PLL4_VALUE
        case RCC_CK_PLL4:
          sfreq = PLL4_VALUE;
          break;
#endif /* PLL4_VALUE */
#ifdef PLL5_VALUE
        case RCC_CK_PLL5:
          sfreq = PLL5_VALUE;
          break;
#endif /* PLL5_VALUE */
#ifdef PLL6_VALUE
        case RCC_CK_PLL6:
          sfreq = PLL6_VALUE;
          break;
#endif /* PLL6_VALUE */
#ifdef PLL7_VALUE
        case RCC_CK_PLL7:
          sfreq = PLL7_VALUE;
          break;
#endif /* PLL7_VALUE */
#ifdef PLL8_VALUE
        case RCC_CK_PLL8:
          sfreq = PLL8_VALUE;
          break;
#endif /* PLL8_VALUE */

#elif defined (USE_STM32MP257CXX_FPGA)
        /* In case of FPGA platform, definition of PLL output clocks are given */
        /* by documents Panther_FPGA_ASSY*.pdf delivered in netlist package :  */
        /* "PLL reference clock is not configurable: fixed at 32MHz for all PLLs input" */
        case RCC_CK_PLL4:
        case RCC_CK_PLL5:
        case RCC_CK_PLL6:
        case RCC_CK_PLL7:
        case RCC_CK_PLL8:
          /* "PLL VCO frequency = 32MHz * FBDIV / REFDIV" */
          sfreq = 32000000 * pll.fbdiv / pll.frefdiv;
          break;
#else
        case RCC_CK_PLL4:
        case RCC_CK_PLL5:
        case RCC_CK_PLL6:
        case RCC_CK_PLL7:
        case RCC_CK_PLL8:
          /* TO BE IMPLEMENTED */
          break;
#endif /* defined (USE_STM32MP257CXX_EMU) | defined (USE_STM32MP257CXX_FPGA) */

        default:
          sfreq = 0;
          break;
      }
      if (sfreq == 0)
      {
        src_id = pll.source;
        sfreq = HAL_RCC_GetFreq(src_id);
      }
      freq = sfreq / (pll.postdiv1 * pll.postdiv2);
    }
    break;
    /* Flexgen: Final div outputs */
    case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
    {
      uint32_t div;
      RCC_FG_GetSource(clk_id, &src_id);
      sfreq = HAL_RCC_GetFreq(src_id);
      RCC_FG_GetDiv(clk_id, &div);
      if (div != 0)
      {
        freq = sfreq / div;
      }
    }
    break;
    /* Bus clocks */
    case RCC_CK_ICN_APB1 ... RCC_CK_ICN_APBDBG:
    {
      RCC_ClkCfgTypeDef clk;
      clk.id = clk_id;
      HAL_RCC_GetClockCfg(&clk);
      sfreq = HAL_RCC_GetFreq(clk.source); // Get source freq
      if (clk.divvalue != 0)
      {
        freq = sfreq / clk.divvalue;
      }
    }
    break;
    /* Misc */
    case RCC_CK_RTC:
      src_id = HAL_RCC_GetSource(clk_id);
      sfreq = HAL_RCC_GetFreq(src_id);
      if (src_id == RCC_CK_HSE)
      {
        uint32_t div;
        div = (READ_REG(RCC->RTCDIVR) & 0x3f) + 1; // HSE divider
        freq = sfreq / div;
      }
      else
        freq = sfreq;
      break;
    default:
      break;
  };

  return freq;
}

uint32_t HAL_RCC_GetBusClockFreq(uint32_t clk_id)
{
  uint16_t src_id;
  uint32_t sfreq;
  uint32_t freq = 0;

  /* Add asserts here to limit request to bus clocks */

  switch (clk_id)
  {
    /* Bus clocks */
    case RCC_CK_ICN_APB1 ... RCC_CK_ICN_APBDBG:
    {
      RCC_ClkCfgTypeDef clk;
      clk.id = clk_id;
      HAL_RCC_GetClockCfg(&clk);
      sfreq = HAL_RCC_GetFreq(clk.source); // Get source freq
      if (clk.divvalue != 0)
      {
        freq = sfreq / clk.divvalue;
      }
    }
    break;
    default:
      break;
  };

  return freq;
}

uint32_t HAL_RCC_GetPeriphClockFreq(uint32_t clk_id)
{
  uint16_t src_id;
  uint32_t sfreq;
  uint32_t freq = 0;
  switch (clk_id)
  {

    /* Add asserts here to limit to flexgen 7 to 63 range */

    /* Flexgen: Final div outputs */
    case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
    {
      uint32_t div;
      RCC_FG_GetSource(clk_id, &src_id);
      sfreq = HAL_RCC_GetFreq(src_id);
      RCC_FG_GetDiv(clk_id, &div);
      if (div != 0)
      {
        freq = sfreq / div;
      }
    }
    break;
    default:
      break;
  };

  return freq;
}

uint32_t HAL_RCC_GetPllClockFreq(uint32_t clk_id)
{
  uint16_t src_id;
  uint32_t sfreq;
  uint32_t freq = 0;
  switch (clk_id)
  {

    /* Add asserts here to limit to Pll range */

    /* Flexgen: Muxsel outputs */
    case RCC_CK_PLL4REF ... RCC_CK_PLL3REF:
      RCC_FG_GetSource(clk_id, &src_id);
      freq = HAL_RCC_GetFreq(src_id);
      break;
    /* Flexgen: internal PLLs */
    case RCC_CK_PLL4 ... RCC_CK_PLL8:
    {
      RCC_PLLCfgTypeDef pll;
      pll.id = clk_id;
      RCC_FG_GetGFG(&pll);
      switch (clk_id)
      {
          /* In case of Emulator platform, PLL output clocks are forced. */
          /* PLL frequencies shall be defined in include 'stm32mp2xx_clocks.h' */
          /* in BSP/STM32MP257CXX_EMU directory */
#if defined (USE_STM32MP257CXX_EMU)
#ifdef PLL4_VALUE
        case RCC_CK_PLL4:
          sfreq = PLL4_VALUE;
          break;
#endif /* PLL4_VALUE */
#ifdef PLL5_VALUE
        case RCC_CK_PLL5:
          sfreq = PLL5_VALUE;
          break;
#endif /* PLL5_VALUE */
#ifdef PLL6_VALUE
        case RCC_CK_PLL6:
          sfreq = PLL6_VALUE;
          break;
#endif /* PLL6_VALUE */
#ifdef PLL7_VALUE
        case RCC_CK_PLL7:
          sfreq = PLL7_VALUE;
          break;
#endif /* PLL7_VALUE */
#ifdef PLL8_VALUE
        case RCC_CK_PLL8:
          sfreq = PLL8_VALUE;
          break;
#endif /* PLL8_VALUE */

#elif defined (USE_STM32MP257CXX_FPGA)
        /* In case of FPGA platform, definition of PLL output clocks are given */
        /* by documents Panther_FPGA_ASSY*.pdf delivered in netlist package :  */
        /* "PLL reference clock is not configurable: fixed at 32MHz for all PLLs input" */
        case RCC_CK_PLL4:
        case RCC_CK_PLL5:
        case RCC_CK_PLL6:
        case RCC_CK_PLL7:
        case RCC_CK_PLL8:
          /* "PLL VCO frequency = 32MHz * FBDIV / REFDIV" */
          sfreq = 32000000 * pll.fbdiv / pll.frefdiv;
          break;
#else
        case RCC_CK_PLL4:
        case RCC_CK_PLL5:
        case RCC_CK_PLL6:
        case RCC_CK_PLL7:
        case RCC_CK_PLL8:
          /* TO BE IMPLEMENTED */
          break;
#endif /* defined (USE_STM32MP257CXX_EMU) | defined (USE_STM32MP257CXX_FPGA) */

        default:
          sfreq = 0;
          break;
      }
      if (sfreq == 0)
      {
        src_id = pll.source;
        sfreq = HAL_RCC_GetFreq(src_id);
      }
      freq = sfreq / (pll.postdiv1 * pll.postdiv2);
    }
    break;
    default:
      break;
  };

  return freq;
}

uint32_t HAL_RCC_GetOscClockFreq(uint32_t clk_id)
{
  uint32_t freq = 0;

  /* Add asserts here to limit to osc range */

  switch (clk_id)
  {
    /* Oscillators */
    case RCC_CK_HSE:
    case RCC_CK_HSE_KER:
      freq = HSE_VALUE;
      break;
    case RCC_CK_HSI:
    case RCC_CK_HSI_KER:
      freq = HSI_VALUE;
      break;
    case RCC_CK_LSE:
      freq = LSE_VALUE;
      break;
    case RCC_CK_LSI:
      freq = LSI_VALUE;
      break;
    case RCC_CK_CSI:
    case RCC_CK_CSI_KER:
      /* TODO: Missing 4 vs 16M support. CSI_VALUE should be 16M then */
      freq = CSI_VALUE;
      break;
    default:
      break;
  };

  return freq;
}

uint32_t HAL_RCC_GetCA35SSFreq(void)
{
  return HAL_RCC_GetPllClockFreq(RCC_CK_PLL1);
}

uint32_t HAL_RCC_GetCM33SSFreq(void)
{
  return HAL_RCC_GetPllClockFreq(RCC_CK_PLL2); // OBI: TBC
}

uint32_t HAL_RCC_GetDDRFreq(void)
{
  return HAL_RCC_GetPllClockFreq(RCC_CK_PLL2);
}

uint32_t HAL_RCC_GetGPUFreq(void)
{
  return HAL_RCC_GetPllClockFreq(RCC_CK_PLL3);
}

/* Returns 'clk_id' measured frequency.
   This feature is available for the following clock types only:
   - flexgen input clocks
   - flexgen PLL input clocks
   - flexgen PLL output clocks
   - PLL1 to 3 ref clocks
   - flexgen final div output clocks
   Should we return 0 if disabled ??? */
uint32_t HAL_RCC_GetFreqM(uint32_t clk_id)  /* OBI: renamed to HAL_RCC_MeasureClockFreq, see next function */
{
  /* Selecting ref clock */
  /* TODO: This part must be revisited to select carefully the ref
           clock according to clock to measure */
  uint16_t ref_id;
  if (clk_id == RCC_CK_PLL4REF)
  {
    ref_id = RCC_CK_PLL5REF;
  }
  else
    ref_id = RCC_CK_PLL4REF;

  uint32_t freq;
  RCC_FG_GetMeasure(clk_id, ref_id, &freq);
  return freq;
}

uint32_t HAL_RCC_MeasureClockFreq(uint32_t clk_id)
{
  /* Selecting ref clock */
  /* TODO: This part must be revisited to select carefully the ref
           clock according to clock to measure */
  uint16_t ref_id;
  if (clk_id == RCC_CK_PLL4REF)
  {
    ref_id = RCC_CK_PLL5REF;
  }
  else
    ref_id = RCC_CK_PLL4REF;

  uint32_t freq;
  RCC_FG_GetMeasure(clk_id, ref_id, &freq);
  return freq;
}

/* Returns source clock ID or 0 for 'no source' or error */
uint16_t HAL_RCC_GetSource(uint32_t clk_id)
{
  uint16_t src_id = 0;
  switch (clk_id)
  {
    /* Flexgen: Muxsel outputs */
    case RCC_CK_PLL4REF ... RCC_CK_PLL3REF:
      RCC_FG_GetSource(clk_id, &src_id);
      break;
    /* Flexgen: internal PLLs */
    case RCC_CK_PLL4 ... RCC_CK_PLL8:
      /* Static connection to 'REF' */
      src_id = RCC_CK_PLL4REF + (clk_id - RCC_CK_PLL4);
      break;
    /* Flexgen: Final div outputs */
    case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
      RCC_FG_GetSource(clk_id, &src_id);
      break;
    /* Bus clocks */
    case RCC_CK_ICN_APB1 ... RCC_CK_ICN_APB3:
      src_id = RCC_CK_ICN_MCU;
      break;
    case RCC_CK_ICN_APB4 ... RCC_CK_ICN_APBDBG:
      src_id = RCC_CK_ICN_SDMMC;
      break;
    case RCC_CK_RTC:
      switch ((READ_REG(RCC->BDCR) >> 16) & 0x3)
      {
        case 1:
          src_id = RCC_CK_LSE;
          break;
        case 2:
          src_id = RCC_CK_LSI;
          break;
        case 3:
          src_id = RCC_CK_HSE;
          break;
        default: // No clock
          break;
      }
      break;
    default:
      break;
  };

  return src_id;
}

/* Returns 1 if enabled, 0 if disabled or error */
int HAL_RCC_IsEnabled(uint32_t clk_id)
{
  assert_param(clk_id != RCC_CK_NONE);
  assert_param(clk_id < RCC_CK_LAST);

  int enabled = 0;
  switch (clk_id)
  {
    /* Oscillators */
    case  RCC_CK_LSI:
      if (READ_REG(RCC->BDCR) & RCC_BDCR_LSION)
      {
        enabled = 1;
      }
      break;
    case RCC_CK_LSE:
      if (READ_REG(RCC->BDCR) & RCC_BDCR_LSEON)
      {
        enabled = 1;
      }
      break;
    case RCC_CK_CSI:
    case RCC_CK_CSI_KER:
      if (READ_REG(RCC->D3DCR) & RCC_D3DCR_CSION)
      {
        enabled = 1;
      }
      break;
    case RCC_CK_HSE:
    case RCC_CK_HSE_KER:
      if (READ_REG(RCC->OCENSETR) & RCC_OCENSETR_HSEON)
      {
        enabled = 1;
      }
      break;
    case RCC_CK_HSI:
    case RCC_CK_HSI_KER:
      if (READ_REG(RCC->OCENSETR) & RCC_OCENSETR_HSION)
      {
        enabled = 1;
      }
      break;
    /* PLLs */
    case RCC_CK_PLL4 ... RCC_CK_PLL8:
      RCC_FG_GetStatus(clk_id, &enabled);
      break;
    /* Flexgen div outputs */
    case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
      RCC_FG_GetStatus(clk_id, &enabled);
      break;
    /* Always enabled (no gating capability) */
    case RCC_CK_PLL4REF ... RCC_CK_PLL8REF:
    case RCC_CK_PLL1REF ... RCC_CK_PLL3REF:
    case RCC_CK_ICN_APB1 ... RCC_CK_ICN_APBDBG:
    case RCC_CK_I2S:
    case RCC_CK_SPDIF:
      enabled = 1;
      break;
  }

  return enabled;
}

/* OBI: what do we do with this function?
   Returns 1 if full clock tree is enabled.
   If a source is disabled returns 0. */
int HAL_RCC_IsRunning(uint32_t clk_id)
{
  assert_param(clk_id != RCC_CK_NONE);
  assert_param(clk_id < RCC_CK_LAST);

  /* Is source running ? */
  uint16_t src_id;
  src_id = HAL_RCC_GetSource(clk_id);
  if (src_id != RCC_CK_NONE)
  {
    if (!HAL_RCC_IsRunning(src_id))
    {
      return 0;
    }
  }

  /* Source path is running.
     What about local status ? */
  if (HAL_RCC_IsEnabled(clk_id))
  {
    return 1;
  }
  return 0;
}

/* Reroute 'src1_id' child clocks to use 'src2_id' instead.
   Note: Rerouting is done only if clock is enabled.
   Note: If new freq is higher than current, divider is modified to not go above.
   Returns: 0=OK, -1=ERROR */
int HAL_RCC_RerouteChilds(uint16_t src1_id, uint16_t src2_id)
{
  assert_param(src1_id != RCC_CK_NONE);
  assert_param(src1_id < RCC_CK_LAST);
  assert_param(src2_id != RCC_CK_NONE);
  assert_param(src2_id < RCC_CK_LAST);

  /* TODO: Should we check there if new source is enabled ? */
  /* TODO: Should be reroute even if clock disabled ? */

  uint32_t src1_freq, src2_freq;
  src1_freq = HAL_RCC_GetFreq(src1_id);
  src2_freq = HAL_RCC_GetFreq(src2_id);

  uint16_t clk_id;
  for (clk_id = 1; clk_id < RCC_CK_LAST; clk_id++)
  {
    /* Only crossbar outputs supported so far */
    if ((clk_id < RCC_Flexgen.fdiv_first) || (clk_id > RCC_Flexgen.fdiv_last))
    {
      continue;
    }

    /* TODO: Does this makes sense for final div outputs ? */
    if ((clk_id == src1_id) || (clk_id == src2_id))
    {
      continue;
    }
    if (HAL_RCC_GetSource(clk_id) != src1_id)
    {
      continue;
    }

    if (!HAL_RCC_IsEnabled(clk_id))
    {
      continue;  // Disabled. Ignoring...
    }

    /* 'clk_id' is using 'src1_id'. Changing its source */
    switch (clk_id)
    {
      /* Flexgen: Muxsel outputs */
      case RCC_CK_PLL4REF ... RCC_CK_PLL3REF:
        if (RCC_FG_SetSource(clk_id, src2_id) != 0)
        {
          return -1;
        }
        break;
      /* Flexgen: Final div outputs */
      case RCC_CK_ICN_MCU ... RCC_CK_CPU1EXT2F:
      {
        uint32_t div;
        RCC_FG_GetDiv(clk_id, &div);
        uint32_t cur_freq, new_freq;
        cur_freq = src1_freq / div;
        new_freq = src2_freq / div;
        if (new_freq > cur_freq)
        {
          div = src2_freq / cur_freq;
          if (src2_freq % cur_freq)
          {
            div++;
          }
          RCC_FG_SetDiv(clk_id, div);
        }
      }
      if (RCC_FG_SetSource(clk_id, src2_id) != 0)
      {
        return -1;
      }
      break;
      /* Bus clocks */
      /* TODO: No sense. Following clocks have static parent */
      case RCC_CK_ICN_APB1 ... RCC_CK_ICN_APBDBG:
      {
        RCC_ClkCfgTypeDef clk;
        clk.id = clk_id;
        clk.source = src2_id;
        if (HAL_RCC_SetClockCfg(&clk) != HAL_OK)
        {
          return -1;
        }
      }
      break;
      default:
        return -1; // Error
    }
  }
  return 0;
}

/**
  * @brief This function handles the general (rcc_cpu1_it/rcc_cpu2_it)
  *        and processor wakeup (rcc_cpu1_wkup_it/rcc_cpu2_wkup_it) RCC interrupts
  * @retval None
  */
void HAL_RCC_IRQHandler(void)
{
  void *reg;
#ifdef CORE_CA35
  reg = (void *)&RCC->C1CIFCLRR;
#else // M33
  reg = (void *)&RCC->C2CIFCLRR;
#endif

  uint32_t flags;
  flags = READ_REGA(reg);
#if 0 /* Is this rather a processing to be implemented in HAL_RCC_Callback(flags)
         when flags&RCC_IT_LSECSS=RCC_IT_LSECSS ? */
  if (__HAL_RCC_GET_IT(RCC_IT_LSECSS) != RESET)
  {
    SET_BIT(flags, RCC_IT_LSECSS);

    /* Enable write access to Backup domain */
    SET_BIT(PWR->CR1, PWR_CR1_DBP);

    /* Clear LSECSSON bit */
    HAL_RCCEx_DisableLSECSS();
    /* Clear LSEON bit */
    CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);

    /* Clear the LSECSSF bit */
    __HAL_RCC_CLEAR_IT(RCC_IT_LSECSS);
  }
#endif

  /* Process all RCC flags */
  if (flags)
  {
    /* Clear the RCC interrupt bits */
    __HAL_RCC_CLEAR_IT(flags);

    /* RCC interrupt user callback */
    HAL_RCC_Callback(flags);
  }
}

/**
  * @brief  RCC global interrupt callback
  * @param  Flags: It contains the flags which were set during the RCC_IRQHandler
  *                before cleaning them
  * @retval None
  */
__weak void HAL_RCC_Callback(uint32_t Flags)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RCC_Callback could be implemented in the user file
  */
}

__weak void HAL_CSS_Callback(uint32_t Flags)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RCC_Callback could be implemented in the user file
  */
}

/**
  * @brief This function handles the RCC Wake up interrupt (rcc_mcu_wkup_irq/rcc_mpu_wkup_irq)
  * @retval None
  */
void HAL_RCC_WAKEUP_IRQHandler(void)
{
  /* Check whether RCC WKUP interrupt flag is set */
  if (__HAL_RCC_GET_IT(RCC_IT_WKUP) != RESET)
  {
    /* Clear RCC WKUP interrupt flag bit */
    __HAL_RCC_CLEAR_IT(RCC_IT_WKUP);

    /* RCC WKUP interrupt user callback */
    HAL_RCC_WAKEUP_Callback();
  }
}

void HAL_HSIMON_IRQHandler(void)
{
}

__weak void HAL_HSIMON_Callback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RCC_WAKEUP_Callback could be implemented in the user file
  */
}

/**
  * @brief  RCC WAKEUP interrupt callback
  * @retval None
  */
__weak void HAL_RCC_WAKEUP_Callback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RCC_WAKEUP_Callback could be implemented in the user file
  */
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/***** MP2 RIF SPECIFIC : TO BE MERGED TO DIFFERENT "defgroup" HERE ABOVE *****/
/** @defgroup RCC_RIF_HAL RCC_RIF
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Read a register in same device to be sure that previous write operation */
/* into RCC_RxSEMCR is completed (without breaking processor pipeline).    */
__STATIC_INLINE HAL_StatusTypeDef WAIT_MUTEX_WRITTEN_IN_RCC_RxSEMCR(uint32_t LocalResIndex)
{
  /* Therefore, we take this opportunity to check (by reading register RCC_RxCIDCFGR[i]) */
  /* that a dynamic CID filtering is effectively configured for present local resource. */
  if ((READ_REG(RCC->R[LocalResIndex].CIDCFGR) & RCC_LOCALRES_ATTR_CID_TYPE_Msk) == RCC_LOCALRES_CID_TYPE_DYNAMIC)
  {
    return HAL_OK;
  }
  else
    return HAL_ERROR;
}

/* Exported macros -----------------------------------------------------------*/

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
/**
  * @brief  Configure Security/Privilege/CID Access Filtering of one RCC Local Resource
  *         Lock this configuration if requested
  * @param  LocalResIndex Value in @ref RCC_LL_EC_RIF_LOCALRES
  * @param  LocalResAttributes Value set by an "OR" operation between :
  *         - one or none Lock configuration argument
  *           @arg RCC_LOCALRES_LOCKED
  *         - one or none Security filtering argument
  *           @arg RCC_LOCALRES_SEC
  *           @arg RCC_LOCALRES_NOSEC
  *         - one or none Privilege filtering argument
  *           @arg RCC_LOCALRES_PRIV
  *           @arg RCC_LOCALRES_NOPRIV
  *         - none or one Static CID filtering argument among
  *           @arg RCC_LOCALRES_CID_STATIC_0
  *           @arg RCC_LOCALRES_CID_STATIC_1
  *           @arg RCC_LOCALRES_CID_STATIC_2
  *           @arg RCC_LOCALRES_CID_STATIC_3
  *           @arg RCC_LOCALRES_CID_STATIC_4
  *           @arg RCC_LOCALRES_CID_STATIC_5
  *           @arg RCC_LOCALRES_CID_STATIC_6
  *           @arg RCC_LOCALRES_CID_STATIC_7
  *           or else a constant Dynamic CID filtering argument among
  *           @arg RCC_LOCALRES_CID_DYNAMIC_NONE
  *           @arg RCC_LOCALRES_CID_DYNAMIC_ALL
  *           or else an "OR" combination of Dynamic CID filtering arguments among
  *           @arg RCC_LOCALRES_CID_DYNAMIC_0
  *           @arg RCC_LOCALRES_CID_DYNAMIC_1
  *           @arg RCC_LOCALRES_CID_DYNAMIC_2
  *           @arg RCC_LOCALRES_CID_DYNAMIC_3
  *           @arg RCC_LOCALRES_CID_DYNAMIC_4
  *           @arg RCC_LOCALRES_CID_DYNAMIC_5
  *           @arg RCC_LOCALRES_CID_DYNAMIC_6
  *           @arg RCC_LOCALRES_CID_DYNAMIC_7
  *
  * @note If Lock configuration argument is not provided,
  *       Security/Privilege/CID filtering configuration is not locked.
  *       If Security filtering configuration argument is not provided,
  *       no security filtering configuration is applied.
  *       If Privilege filtering configuration argument is not provided,
  *       no privilege filtering configuration is applied.
  *       If neither static CID filtering argument,
  *       nor dynamic CID filtering argument is provided,
  *       CID filtering is disabled.
  * @retval HAL Status in {HAL_OK; HAL_ERROR}.
  */
HAL_StatusTypeDef HAL_RCC_ConfigAttributes(uint32_t LocalResIndex, uint32_t LocalResAttributes)
{
  /* Initialize HAL status */
  HAL_StatusTypeDef hal_status = HAL_BUSY;

  /* Split LOCK/PRIV/SEC/CID attributes */
  uint32_t priv_attr = LocalResAttributes & RCC_LOCALRES_ATTR_PRIV_Msk;
  uint32_t lock_attr = LocalResAttributes & RCC_LOCALRES_ATTR_LOCK_Msk;
  uint32_t sec_attr  = LocalResAttributes & RCC_LOCALRES_ATTR_SEC_Msk;
  uint32_t cid_attr  = LocalResAttributes & (RCC_LOCALRES_ATTR_CID_TYPE_Msk |
                                             RCC_LOCALRES_ATTR_STATIC_CID_Msk |
                                             RCC_LOCALRES_ATTR_DYNAMIC_CID_Msk);

  /* Check input parameters : */
  /* - valid range of LocalResIndex */
  assert_param(IS_LOCALRES_INDEX(LocalResIndex));
  /* - valid value of PRIV attribute */
  assert_param(IS_PRIV_ATTRIBUTE(priv_attr));
  /* - valid value of LOCK attribute */
  assert_param(IS_LOCK_ATTRIBUTE(lock_attr));
  /* - valid value of SEC attribute */
  assert_param(IS_SEC_ATTRIBUTE(sec_attr));
  /* - valid values of CID type, Static CID and Dynamic CID attributes */
  assert_param(IS_CID_ATTRIBUTE(cid_attr));

  /* Take into account that TDCID is allowed to set any configuration */
  /* that is to say modifying security and privilege filtering */
  /* even if current CID filtering prevents to do it. */
#if defined(CORE_CA35)
  LL_RCC_SetLocalResSCid(LocalResIndex, RCC_LOCALRES_CID_STATIC_1);
#endif /* defined(CORE_CA35) */
#if defined(CORE_CM33)
  LL_RCC_SetLocalResSCid(LocalResIndex, RCC_LOCALRES_CID_STATIC_2);
#endif /* defined(CORE_CM33) */

  /* Manage PRIV filtering attribute */
  if (priv_attr == RCC_LOCALRES_PRIV)
  {
    LL_RCC_EnableLocalResPrivilege(LocalResIndex);
  }
  else if (priv_attr == RCC_LOCALRES_NOPRIV)
  {
    LL_RCC_DisableLocalResPrivilege(LocalResIndex);
  }

  /* Manage SEC filtering attribute */
  if (sec_attr == RCC_LOCALRES_SEC)
  {
    LL_RCC_EnableLocalResSecure(LocalResIndex);
  }
  else if (sec_attr == RCC_LOCALRES_NOSEC)
  {
    LL_RCC_DisableLocalResSecure(LocalResIndex);
  }
  /* Manage CID filtering attributes */
  switch (cid_attr & RCC_LOCALRES_ATTR_CID_TYPE_Msk)
  {
    case RCC_LOCALRES_CID_TYPE_STATIC:
      LL_RCC_SetLocalResSCid(LocalResIndex, cid_attr);
      break;
    case RCC_LOCALRES_CID_TYPE_DYNAMIC:
      LL_RCC_SetLocalResWLCid(LocalResIndex, cid_attr);
      break;
    case RCC_LOCALRES_CID_TYPE_NONE:
      LL_RCC_SetLocalResNoCid(LocalResIndex);
      break;
    default :
      hal_status = HAL_ERROR;
      break;
  }
  /* Manage LOCK configuration attribute */
  if (lock_attr == RCC_LOCALRES_LOCKED)
  {
    LL_RCC_LockLocalResConfig(LocalResIndex);
  }

  /* return HAL status */
  if (hal_status == HAL_BUSY)
  {
    hal_status = HAL_OK;
  }
  return hal_status;
}
#endif /* defined(CORTEX_IN_SECURE_STATE) */

/**
  * @brief  Get Security/Privilege/CID Access Filtering configuration of one RCC Local Resource
  * @param  LocalResIndex Value in @ref RCC_LL_EC_RIF_LOCALRES
  * @param  pLocalResAttributes Returned value composed of following bitfield :
  *         . bits [31:24] Locked configuration state
  *         . bits [23:16] CID whitelist (dynamic CIDs list)
  *         . bits [15:12] Security filtering state
  *         . bits [11:8]  Privilege filtering state
  *         . bit  7       Reserved
  *         . bits [6:4]   Static CID
  *         . bits [3:2]   Reserved
  *         . bits [1:0]   CID filtering type
  *         to be compared with following reference values
  *         - linked to Lock configuration state
  *           @arg RCC_LOCALRES_LOCKED
  *         - linked to Security filtering state
  *           @arg RCC_LOCALRES_SEC
  *           @arg RCC_LOCALRES_NOSEC
  *         - linked to Privilege filtering state
  *           @arg RCC_LOCALRES_PRIV
  *           @arg RCC_LOCALRES_NOPRIV
  *         - linked to Static CID filtering configuration
  *           @arg RCC_LOCALRES_CID_STATIC_0
  *           @arg RCC_LOCALRES_CID_STATIC_1
  *           @arg RCC_LOCALRES_CID_STATIC_2
  *           @arg RCC_LOCALRES_CID_STATIC_3
  *           @arg RCC_LOCALRES_CID_STATIC_4
  *           @arg RCC_LOCALRES_CID_STATIC_5
  *           @arg RCC_LOCALRES_CID_STATIC_6
  *           @arg RCC_LOCALRES_CID_STATIC_7
  *         - linked to Dynamic CID filtering configuration
  *           @arg RCC_LOCALRES_CID_DYNAMIC_NONE
  *           @arg RCC_LOCALRES_CID_DYNAMIC_ALL
  *           @arg RCC_LOCALRES_CID_DYNAMIC_0
  *           @arg RCC_LOCALRES_CID_DYNAMIC_1
  *           @arg RCC_LOCALRES_CID_DYNAMIC_2
  *           @arg RCC_LOCALRES_CID_DYNAMIC_3
  *           @arg RCC_LOCALRES_CID_DYNAMIC_4
  *           @arg RCC_LOCALRES_CID_DYNAMIC_5
  *           @arg RCC_LOCALRES_CID_DYNAMIC_6
  *           @arg RCC_LOCALRES_CID_DYNAMIC_7
  * @retval HAL Status in {HAL_OK; HAL_ERROR}.
  */
HAL_StatusTypeDef HAL_RCC_GetConfigAttributes(uint32_t LocalResIndex, uint32_t *pLocalResAttributes)
{
  uint32_t local_res_attr;

  /* Initialize HAL status */
  HAL_StatusTypeDef hal_status = HAL_BUSY;

  /* Check input parameters : */
  /* - not-null pointer for pLocalResAttributes */
  if (pLocalResAttributes == NULL)
  {
    return HAL_ERROR;
  }
  /* - valid range of LocalResIndex */
  assert_param(IS_LOCALRES_INDEX(LocalResIndex));

  /* Initialize attributes value to None */
  local_res_attr = RCC_LOCALRES_ATTR_NONE;
  /* and LocalResAttributes value to Invalid */
  *pLocalResAttributes = RCC_LOCALRES_ATTR_INVALID;

  /* Get local resource lock state */
  if (LL_RCC_IsLockedLocalResConfig(LocalResIndex) == 1)
  {
    /* to set lock configuration attribute accordingly */
    local_res_attr |= RCC_LOCALRES_LOCKED;
  }

  /* Get local resource security filtering state */
  if (LL_RCC_IsEnabledLocalResSecure(LocalResIndex) == 1)
  {
    /* to set security filtering attribute accordingly */
    local_res_attr |= RCC_LOCALRES_SEC;
  }
  else
  {
    local_res_attr |= RCC_LOCALRES_NOSEC;
  }

  /* Get local resource privilege filtering state */
  if (LL_RCC_IsEnabledLocalResPrivilege(LocalResIndex) == 1)
  {
    /* to set privilege filtering attribute accordingly */
    local_res_attr |= RCC_LOCALRES_PRIV;
  }
  else
  {
    local_res_attr |= RCC_LOCALRES_NOPRIV;
  }

  /* Get local resource CID filtering settings */
  /* to set None/Static/Dynamic CID filtering attributes accordingly */
  local_res_attr |= LL_RCC_GetLocalResCid(LocalResIndex);

  /* Output LocalResAttributes value */
  *pLocalResAttributes = local_res_attr;

  /* return HAL status */
  if (hal_status == HAL_BUSY)
  {
    hal_status = HAL_OK;
  }
  return hal_status;
}

/**
  * @brief  Take semaphore to access RCC Local Resource.
  *         Check semaphore has been effectively taken.
  * @param  LocalResIndex Value in @ref RCC_LL_EC_RIF_LOCALRES
  * @note   If operation fails, return error status.
  * @retval HAL Status in {HAL_OK; HAL_ERROR}.
  */
HAL_StatusTypeDef HAL_RCC_TakeLocalResSemaphore(uint32_t LocalResIndex)
{
  /* Initialize HAL status */
  HAL_StatusTypeDef hal_status = HAL_BUSY;

  /* Check input parameter : LocalResIndex in valid range */
  assert_param(IS_LOCALRES_INDEX(LocalResIndex));

  /* For local resource : */
  /* - check semaphore initial state */
  /*   and set output status accordingly */
  if ((LL_RCC_GetLocalResSem(LocalResIndex) & RCC_RxSEMCR_SEM_MUTEX) == RCC_RxSEMCR_SEM_MUTEX)
  {
    /*   <=> if already taken, then output error status */
    return HAL_ERROR;
  }
  /* - take semaphore */
  LL_RCC_TakeLocalResSem(LocalResIndex);
  /* - once mutex has been updated in target register, */
  WAIT_MUTEX_WRITTEN_IN_RCC_RxSEMCR(LocalResIndex);
  /*   check semaphore has been taken */
  /*   and set output status accordingly */
  /*   <=> if semaphore has not been taken by : */
  /*       - CID1 in A35 case, */
#if defined(CORE_CA35)
  if (LL_RCC_GetLocalResSem(LocalResIndex) != RCC_LOCALRES_TAKEN_SEMCID1)
#endif /* defined(CORE_CA35) */
    /*       - CID2 in M33 case, */
#if defined(CORE_CM33)
    if (LL_RCC_GetLocalResSem(LocalResIndex) != RCC_LOCALRES_TAKEN_SEMCID2)
#endif /* defined(CORE_CM33) */
    {
      /*       then output error status */
      return HAL_ERROR;
    }
  /* return HAL status */
  if (hal_status == HAL_BUSY)
  {
    hal_status = HAL_OK;
  }
  return hal_status;
}

/**
  * @brief  Release semaphore used to access RCC Local Resource.
  *         Check semaphore has been effectively released.
  * @param  LocalResIndex Value in @ref RCC_LL_EC_RIF_LOCALRES
  * @note   If operation fails, return error status.
  * @retval HAL Status in {HAL_OK; HAL_ERROR}.
  */
HAL_StatusTypeDef HAL_RCC_ReleaseLocalResSemaphore(uint32_t LocalResIndex)
{
  /* Initialize HAL status */
  HAL_StatusTypeDef hal_status = HAL_BUSY;

  /* Check input parameter : LocalResIndex in valid range */
  assert_param(IS_LOCALRES_INDEX(LocalResIndex));

  /* For local resource : */
  /* - check semaphore initial state */
  /*   and set output status accordingly */
  /*   <=> if semaphore has not been taken by : */
  /*       - CID1 in A35 case, */
#if defined(CORE_CA35)
  if (LL_RCC_GetLocalResSem(LocalResIndex) != RCC_LOCALRES_TAKEN_SEMCID1)
#endif /* defined(CORE_CA35) */
    /*       - CID2 in M33 case, */
#if defined(CORE_CM33)
    if (LL_RCC_GetLocalResSem(LocalResIndex) != RCC_LOCALRES_TAKEN_SEMCID2)
#endif /* defined(CORE_CM33) */
    {
      /*       then output error status */
      return HAL_ERROR;
    }
  /* - release semaphore */
  LL_RCC_ReleaseLocalResSem(LocalResIndex);
  /* - once mutex has been updated in target register, */
  WAIT_MUTEX_WRITTEN_IN_RCC_RxSEMCR(LocalResIndex);
  /*   check semaphore has been released */
  /*   and set output status accordingly */
  /*   <=> if semaphore has not been released by : */
  /*       - CID1 in A35 case, */
#if defined(CORE_CA35)
  if (LL_RCC_GetLocalResSem(LocalResIndex) != RCC_LOCALRES_RELEASED_SEMCID1)
#endif /* defined(CORE_CA35) */
    /*       - CID2 in M33 case, */
#if defined(CORE_CM33)
    if (LL_RCC_GetLocalResSem(LocalResIndex) != RCC_LOCALRES_RELEASED_SEMCID2)
#endif /* defined(CORE_CM33) */
    {
      /*       then output error status */
      return HAL_ERROR;
    }
  /* return HAL status */
  if (hal_status == HAL_BUSY)
  {
    hal_status = HAL_OK;
  }
  return hal_status;
}
#endif // 191021
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
