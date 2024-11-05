/**
  ******************************************************************************
  * @file    Templates/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32mp_util_conf.h"

/** @addtogroup STM32MP2xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* External functions ------------------------------------------------------- */
void _Reset_EL3(void) asm("_Reset_EL3");

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* STGEN (HSI) default frequency according to selected platform */
#define STGEN_DEFAULT_FREQUENCY_IN_HZ HSI_VALUE

/* Function pointer */
void (*p_function)(void);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* STGEN frequency : same as HSI frequency */
static uint32_t STGENFreqHz = STGEN_DEFAULT_FREQUENCY_IN_HZ;

/* Private function prototypes -----------------------------------------------*/
static uint32_t SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/
void Mon_EndOfApplication( void );
void Mon_A35SystemClockConfig( void );

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This project  as template implementation that User may integrate
     in his application, to enhance the performance in case of use of AXI interface
     with several masters. */

  /* STM32MP1xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  DDR_InitTypeDef iddr;

  HAL_Init();

  /* Dummy delay to ensure SysTick is running fine
   * If it is not we will be stuck here
   */
  valid_delay_us(10 * 1000);

  /* Configure A35 system clock, because some parts are only reachable in EL3 */
  Mon_A35SystemClockConfig();

  /* Configure the system clock */
  SystemClock_Config();

  /* Update System clock variable SystemCoreClock for debug */
  SystemCoreClockUpdate();

  /*  Configure USART for logging messages  */
  UART_Config();

  /*****************************/
  /* Test environment settings */
  /*****************************/

  /* Configure the system power */
  SystemPower_Config();

  /* Init LED3 */
  BSP_LED_Init(LED_GREEN);

  /* Insert 1 second delay */
  valid_delay_us(100 * 1000);

  iddr.wakeup_from_standby = false;
  iddr.self_refresh = false;
  iddr.zdata = 0;
  iddr.clear_bkp = false;

  if (HAL_DDR_Init(&iddr) != HAL_OK)
  {
    printf("DDR Initialization KO\n\r");
    return 1;
  }

  /* Infinite loop */
  while (1)
  {
  /* Toggle LED3 ON */
  BSP_LED_Toggle(LED_GREEN);

  /* Insert 1 second delay */
  valid_delay_us(100 * 1000);

  /* Toggle LED3 OFF */
  BSP_LED_Toggle(LED_GREEN);
  }
}

void Mon_A35SystemClockConfig( void )
{
#define STGENC_START_VAL              0x3UL

   volatile uint32_t reg32_val;

   /* Start Secure Timestamp Generator (STGEN) */
   /* (clock source of A35 secure/non-secure physical timers) */
   /* by assuming its source clock is HSI at 32MHz/64MHz : */
   /* - enable STGENRW/STGENRO clock */
   /*   by setting RCC_STGENCFGR bit 1 "STGENEN" */
   WRITE_REG(RCC->STGENCFGR,
               READ_REG(RCC->STGENCFGR)|RCC_STGENCFGR_STGENEN);
   /* - set STGEN frequency value to 32MHz/64MHz in dedicated register */
   WRITE_REG(STGENC->CNTFID0, STGENFreqHz);
   /* - set DBGMCU_CRA35 to halt timestamp generation on debug */
   /*   by setting its bit 1 "DBG_STGEN_STOP" */
   WRITE_REG(DBGMCU->CRA35,
               READ_REG(DBGMCU->CRA35)|DBGMCU_CRA35_DBG_STGEN_STOP);
   /* - start STGENC */
   /*   by setting CNTCR bit 0 "EN" */
   /*   and bit 1 "HDBG" (to halt timestamp generation on debug) */
   WRITE_REG(STGENC->CNTCR,STGENC_START_VAL);

   /* Set CNTFRQ_EL0 register according to STGEN frequency in Hz */
   /* see ARM DDI0487B.a page 2673 #D7.5.1 "CNTFRQ_EL0" */
   reg32_val = READ_REG(STGENC->CNTFID0);
   asm volatile("MSR CNTFRQ_EL0, %0" : "=r" (reg32_val));

   return;
}

static uint32_t SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll1InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll4InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll5InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll6InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll7InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll8InitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE |
                                     RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.LSEDriveValue = RCC_LSEDRIVE_MEDIUMHIGH;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    printf("%s: Failed to enable oscillators.\n\r", __func__);
    while (1);
  }

  /* 1000MHz */
  RCC_Pll1InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll1InitStruct.PLLMode = 0;
  RCC_Pll1InitStruct.FBDIV = 25;
  RCC_Pll1InitStruct.FREFDIV = 1;
  RCC_Pll1InitStruct.POSTDIV1 = 1;
  RCC_Pll1InitStruct.POSTDIV2 = 1;
  RCC_Pll1InitStruct.PLLState = RCC_PLL_ON;

  /* 1200MHz */
  RCC_Pll4InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll4InitStruct.PLLMode = 0;
  RCC_Pll4InitStruct.FBDIV = 60;
  RCC_Pll4InitStruct.FREFDIV = 2;
  RCC_Pll4InitStruct.POSTDIV1 = 1;
  RCC_Pll4InitStruct.POSTDIV2 = 1;
  RCC_Pll4InitStruct.PLLState = RCC_PLL_ON;

  /* 532MHz */
  RCC_Pll5InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll5InitStruct.PLLMode = 0;
  RCC_Pll5InitStruct.FBDIV = 53;
  RCC_Pll5InitStruct.FREFDIV = 4;
  RCC_Pll5InitStruct.FRACIN = 3355500;
  RCC_Pll5InitStruct.POSTDIV1 = 1;
  RCC_Pll5InitStruct.POSTDIV2 = 1;
  RCC_Pll5InitStruct.PLLState = RCC_PLL_ON;

  /* 500MHz */
  RCC_Pll6InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll6InitStruct.PLLMode = 0;
  RCC_Pll6InitStruct.FBDIV = 25;
  RCC_Pll6InitStruct.FREFDIV = 1;
  RCC_Pll6InitStruct.POSTDIV1 = 2;
  RCC_Pll6InitStruct.POSTDIV2 = 1;
  RCC_Pll6InitStruct.PLLState = RCC_PLL_ON;

  /* 491.5MHz */
  RCC_Pll7InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll7InitStruct.PLLMode = 0;
  RCC_Pll7InitStruct.FBDIV = 49;
  RCC_Pll7InitStruct.FREFDIV = 2;
  RCC_Pll7InitStruct.FRACIN = 2516600;
  RCC_Pll7InitStruct.POSTDIV1 = 2;
  RCC_Pll7InitStruct.POSTDIV2 = 1;
  RCC_Pll7InitStruct.PLLState = RCC_PLL_ON;

  /* 451.5MHz */
  RCC_Pll8InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll8InitStruct.PLLMode = 0;
  RCC_Pll8InitStruct.FBDIV = 45;
  RCC_Pll8InitStruct.FREFDIV = 2;
  RCC_Pll8InitStruct.FRACIN = 2516600;
  RCC_Pll8InitStruct.POSTDIV1 = 2;
  RCC_Pll8InitStruct.POSTDIV2 = 1;
  RCC_Pll8InitStruct.PLLState = RCC_PLL_ON;

  /* Feed A35 on another source than PLL1 */
  HAL_RCCEx_CA35SS_SetCA35SSClockSource(A35_SOURCE_EXT);

  if (HAL_RCCEx_CA35SS_PLL1Config(&RCC_Pll1InitStruct) != HAL_OK)
  {
    printf("%s: Failed to enable PLL1.\n\r", __func__);
    printf("%s: Staying on external clock.\n\r", __func__);
    while (1);
  }

  /* Switch back A35 on PLL1 */
  HAL_RCCEx_CA35SS_SetCA35SSClockSource(A35_SOURCE_PLL1);

  if (HAL_RCCEx_PLL4Config(&RCC_Pll4InitStruct) != HAL_OK)
  {
    printf("%s: Failed to enable PLL4.\n\r", __func__);
    while (1);
  }

  if (HAL_RCCEx_PLL5Config(&RCC_Pll5InitStruct) != HAL_OK)
  {
    printf("%s: Failed to enable PLL5.\n\r", __func__);
    while (1);
  }

  if (HAL_RCCEx_PLL6Config(&RCC_Pll6InitStruct) != HAL_OK)
  {
    printf("%s: Failed to enable PLL6.\n\r", __func__);
    while (1);
  }

  if (HAL_RCCEx_PLL7Config(&RCC_Pll7InitStruct) != HAL_OK)
  {
    printf("%s: Failed to enable PLL7.\n\r", __func__);
    while (1);
  }

  if (HAL_RCCEx_PLL8Config(&RCC_Pll8InitStruct) != HAL_OK)
  {
    printf("%s: Failed to enable PLL8.\n\r", __func__);
    while (1);
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_ICN_HS_MCU | RCC_CLOCKTYPE_ICN_LS_MCU |
                                RCC_CLOCKTYPE_ICN_SDMMC | RCC_CLOCKTYPE_ICN_DDR |
                                RCC_CLOCKTYPE_ICN_DISPLAY | RCC_CLOCKTYPE_ICN_HCL |
                                RCC_CLOCKTYPE_ICN_NIC | RCC_CLOCKTYPE_ICN_VID |
                                RCC_CLOCKTYPE_ICN_APB1 | RCC_CLOCKTYPE_ICN_APB2 |
                                RCC_CLOCKTYPE_ICN_APB3 | RCC_CLOCKTYPE_ICN_APB4 |
                                RCC_CLOCKTYPE_ICN_APBDBG;

  RCC_ClkInitStruct.ICN_HS_MCU.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_HS_MCU.Div = 4;
  RCC_ClkInitStruct.ICN_SDMMC.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_SDMMC.Div = 6;
  RCC_ClkInitStruct.ICN_DDR.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_DDR.Div = 2;
  RCC_ClkInitStruct.ICN_DISPLAY.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_DISPLAY.Div = 4;
  RCC_ClkInitStruct.ICN_HCL.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_HCL.Div = 4;
  RCC_ClkInitStruct.ICN_NIC.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_NIC.Div = 3;
  RCC_ClkInitStruct.ICN_VID.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_VID.Div = 2;
  RCC_ClkInitStruct.ICN_LSMCU_Div = RCC_LSMCU_DIV2;
  RCC_ClkInitStruct.APB1_Div = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2_Div = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB3_Div = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB4_Div = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APBDBG_Div = RCC_APBDBG_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, 0) != HAL_OK)
  {
   printf("%s: Failed to configure bus clocks.\n\r", __func__);
    while (1);
  }

  return 0;
}

typedef struct
{
  uint32_t PLLSource;   /*!< The new source of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Clock_Source                */
  uint8_t  PLLState;    /*!< The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_State                       */
  uint8_t  FREFDIV;
  uint16_t FBDIV;
  uint32_t FRACIN;
  uint32_t POSTDIV1;
  uint32_t POSTDIV2;
} PLLInitTypeDef;

static void GetPLLxConfig(PLLInitTypeDef  *pll_config, uint32_t PLL_num)
{
  switch (PLL_num)
  {
    case 1:
      pll_config->PLLState = ((CA35SSC->PLL_ENABLE_RW & CA35SSC_PLL_ENABLE_RW_PLL_EN) == CA35SSC_PLL_ENABLE_RW_PLL_EN ? RCC_PLL_ON : RCC_PLL_OFF);
      pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL5_Msk) >> RCC_MUXSELCFGR_MUXSEL5_Pos;
      pll_config->FREFDIV = (CA35SSC->PLL_FREQ1_RW & CA35SSC_PLL_FREQ1_RW_REFDIV_Msk) >> CA35SSC_PLL_FREQ1_RW_REFDIV_Pos;
      pll_config->FBDIV = (CA35SSC->PLL_FREQ1_RW & CA35SSC_PLL_FREQ1_RW_FBDIV_Msk) >> CA35SSC_PLL_FREQ1_RW_FBDIV_Pos;
      pll_config->FRACIN = 0;
      pll_config->POSTDIV1 = (CA35SSC->PLL_FREQ2_RW & CA35SSC_PLL_FREQ2_RW_POSTDIV1_Msk) >> CA35SSC_PLL_FREQ2_RW_POSTDIV1_Pos;
      pll_config->POSTDIV2 = (CA35SSC->PLL_FREQ2_RW & CA35SSC_PLL_FREQ2_RW_POSTDIV2_Msk) >> CA35SSC_PLL_FREQ2_RW_POSTDIV2_Pos;
      break;
    case 4:
      pll_config->PLLState = ((RCC->PLL4CFGR1 & RCC_PLL4CFGR1_PLLEN) == RCC_PLL4CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
      pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL0_Msk) >> RCC_MUXSELCFGR_MUXSEL0_Pos;
      pll_config->FREFDIV = (RCC->PLL4CFGR2 & RCC_PLL4CFGR2_FREFDIV_Msk) >> RCC_PLL4CFGR2_FREFDIV_Pos;
      pll_config->FBDIV = (RCC->PLL4CFGR2 & RCC_PLL4CFGR2_FBDIV_Msk) >> RCC_PLL4CFGR2_FBDIV_Pos;
      pll_config->FRACIN = (RCC->PLL4CFGR3 & RCC_PLL4CFGR3_FRACIN_Msk) >> RCC_PLL4CFGR3_FRACIN_Pos;
      pll_config->POSTDIV1 = (RCC->PLL4CFGR6 & RCC_PLL4CFGR6_POSTDIV1_Msk) >> RCC_PLL4CFGR6_POSTDIV1_Pos;
      pll_config->POSTDIV2 = (RCC->PLL4CFGR7 & RCC_PLL4CFGR7_POSTDIV2_Msk) >> RCC_PLL4CFGR7_POSTDIV2_Pos;
      break;
    case 5:
      pll_config->PLLState = ((RCC->PLL5CFGR1 & RCC_PLL5CFGR1_PLLEN) == RCC_PLL5CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
      pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL0_Msk) >> RCC_MUXSELCFGR_MUXSEL0_Pos;
      pll_config->FREFDIV = (RCC->PLL5CFGR2 & RCC_PLL5CFGR2_FREFDIV_Msk) >> RCC_PLL5CFGR2_FREFDIV_Pos;
      pll_config->FBDIV = (RCC->PLL5CFGR2 & RCC_PLL5CFGR2_FBDIV_Msk) >> RCC_PLL5CFGR2_FBDIV_Pos;
      pll_config->FRACIN = (RCC->PLL5CFGR3 & RCC_PLL5CFGR3_FRACIN_Msk) >> RCC_PLL5CFGR3_FRACIN_Pos;
      pll_config->POSTDIV1 = (RCC->PLL5CFGR6 & RCC_PLL5CFGR6_POSTDIV1_Msk) >> RCC_PLL5CFGR6_POSTDIV1_Pos;
      pll_config->POSTDIV2 = (RCC->PLL5CFGR7 & RCC_PLL5CFGR7_POSTDIV2_Msk) >> RCC_PLL5CFGR7_POSTDIV2_Pos;
      break;
    case 6:
      pll_config->PLLState = ((RCC->PLL6CFGR1 & RCC_PLL6CFGR1_PLLEN) == RCC_PLL6CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
      pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL0_Msk) >> RCC_MUXSELCFGR_MUXSEL0_Pos;
      pll_config->FREFDIV = (RCC->PLL6CFGR2 & RCC_PLL6CFGR2_FREFDIV_Msk) >> RCC_PLL6CFGR2_FREFDIV_Pos;
      pll_config->FBDIV = (RCC->PLL6CFGR2 & RCC_PLL6CFGR2_FBDIV_Msk) >> RCC_PLL6CFGR2_FBDIV_Pos;
      pll_config->FRACIN = (RCC->PLL6CFGR3 & RCC_PLL6CFGR3_FRACIN_Msk) >> RCC_PLL6CFGR3_FRACIN_Pos;
      pll_config->POSTDIV1 = (RCC->PLL6CFGR6 & RCC_PLL6CFGR6_POSTDIV1_Msk) >> RCC_PLL6CFGR6_POSTDIV1_Pos;
      pll_config->POSTDIV2 = (RCC->PLL6CFGR7 & RCC_PLL6CFGR7_POSTDIV2_Msk) >> RCC_PLL6CFGR7_POSTDIV2_Pos;
      break;
    case 7:
      pll_config->PLLState = ((RCC->PLL7CFGR1 & RCC_PLL7CFGR1_PLLEN) == RCC_PLL7CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
      pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL0_Msk) >> RCC_MUXSELCFGR_MUXSEL0_Pos;
      pll_config->FREFDIV = (RCC->PLL7CFGR2 & RCC_PLL7CFGR2_FREFDIV_Msk) >> RCC_PLL7CFGR2_FREFDIV_Pos;
      pll_config->FBDIV = (RCC->PLL7CFGR2 & RCC_PLL7CFGR2_FBDIV_Msk) >> RCC_PLL7CFGR2_FBDIV_Pos;
      pll_config->FRACIN = (RCC->PLL7CFGR3 & RCC_PLL7CFGR3_FRACIN_Msk) >> RCC_PLL7CFGR3_FRACIN_Pos;
      pll_config->POSTDIV1 = (RCC->PLL7CFGR6 & RCC_PLL7CFGR6_POSTDIV1_Msk) >> RCC_PLL7CFGR6_POSTDIV1_Pos;
      pll_config->POSTDIV2 = (RCC->PLL7CFGR7 & RCC_PLL7CFGR7_POSTDIV2_Msk) >> RCC_PLL7CFGR7_POSTDIV2_Pos;
      break;
    case 8:
      pll_config->PLLState = ((RCC->PLL8CFGR1 & RCC_PLL8CFGR1_PLLEN) == RCC_PLL8CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
      pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL0_Msk) >> RCC_MUXSELCFGR_MUXSEL0_Pos;
      pll_config->FREFDIV = (RCC->PLL8CFGR2 & RCC_PLL8CFGR2_FREFDIV_Msk) >> RCC_PLL8CFGR2_FREFDIV_Pos;
      pll_config->FBDIV = (RCC->PLL8CFGR2 & RCC_PLL8CFGR2_FBDIV_Msk) >> RCC_PLL8CFGR2_FBDIV_Pos;
      pll_config->FRACIN = (RCC->PLL8CFGR3 & RCC_PLL8CFGR3_FRACIN_Msk) >> RCC_PLL8CFGR3_FRACIN_Pos;
      pll_config->POSTDIV1 = (RCC->PLL8CFGR6 & RCC_PLL8CFGR6_POSTDIV1_Msk) >> RCC_PLL8CFGR6_POSTDIV1_Pos;
      pll_config->POSTDIV2 = (RCC->PLL8CFGR7 & RCC_PLL8CFGR7_POSTDIV2_Msk) >> RCC_PLL8CFGR7_POSTDIV2_Pos;
      break;
    default:
      pll_config->PLLState = 0;
      pll_config->PLLSource = 0;
      pll_config->FREFDIV = 0;
      pll_config->FBDIV = 0;
      pll_config->FRACIN = 0;
      pll_config->POSTDIV1 = 0;
      pll_config->POSTDIV2 = 0;
      break;
  }
}

static uint32_t ComputePLLClockFreq(PLLInitTypeDef *pll)
{
  uint32_t source_freq;
  uint64_t pll_output;

  switch (pll->PLLSource)
  {
    case 0 : /* HSI */
      source_freq = HSI_VALUE;
      break;
    case 1 : /* HSE */
      source_freq = HSE_VALUE;
      break;
    case 2 : /* MSI */
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }

  /* Compute PLL frequency from PLL parameters according to fractional mode selection */
  /* Note : keep maximum computing precision by doubling integer resolution */
  /*        and process numerator before applying dividers */
  if (0 == pll->FRACIN)
    pll_output = (uint64_t)source_freq * (uint64_t)pll->FBDIV;
  else
  {
    pll_output = (uint64_t)source_freq * ((1<<24)*(uint64_t)pll->FBDIV + (uint64_t)pll->FRACIN);
    pll_output /= (1<<24);
  }
  pll_output /= (uint64_t)(pll->FREFDIV * pll->POSTDIV1 * pll->POSTDIV2);

  return (uint32_t)pll_output;
}

uint32_t SystemCoreClock = HSI_VALUE;

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the Cortex-A35 clock.
  *         This clock can be ck_cpu1_ext2f, ck_cpu1_ext2f/2 or PLL1
  *         CAUTION: A35 SYSTICK is not based on the clock of Cortex-A35.
  *           It is based on STGEN IP which has it is own clock
  *           (ck_ker_stgen flexgen channel) based on HSI by default
  *
  * @note   Each time the Cortex-A35 clock changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t        xbar_source;
  uint32_t        xbar_source_freq;
  PLLInitTypeDef  pll_config;
  uint32_t        source_clock;

  /* Check CA35 source (can be EXT2F or PLL1) */
  source_clock = CA35SSC->CHGCLKREQ_RW & CA35SSC_CHGCLKREQ_RW_ARM_CHGCLKREQ;

  if (source_clock == 0)
  {
    /* Return PLL1 frequency */
    GetPLLxConfig(&pll_config, 1);
    SystemCoreClock = ComputePLLClockFreq(&pll_config);
  }
  else
  {
    /* Identify xbar source for ck_cpu1_ext2f flexgen channel (channel 63) */
    xbar_source = (uint32_t)(READ_BIT(RCC->XBARxCFGR[63], RCC_XBARxCFGR_XBARxSEL_Msk));

    /* Then retrieve the source frequency */
    switch (xbar_source)
    {
      case 0: /* PLL4 */
        GetPLLxConfig(&pll_config, 4);
        xbar_source_freq = ComputePLLClockFreq(&pll_config);
        break;

      case 1 : /* PLL5 */
        GetPLLxConfig(&pll_config, 5);
        xbar_source_freq = ComputePLLClockFreq(&pll_config);
        break;

      case 2 :/* PLL6 */
        GetPLLxConfig(&pll_config, 6);
        xbar_source_freq = ComputePLLClockFreq(&pll_config);
        break;

      case 3 : /* PLL7 */
        GetPLLxConfig(&pll_config, 7);
        xbar_source_freq = ComputePLLClockFreq(&pll_config);
        break;

      case 4 : /* PLL8 */
        GetPLLxConfig(&pll_config, 8);
        xbar_source_freq = ComputePLLClockFreq(&pll_config);
        break;

      case 5 : /* HSI */
      case 8 : /* HSI_KER */
        xbar_source_freq = HSI_VALUE;
        break;

      case 7 : /* MSI */
      case 10 : /* MSI_KER */
        xbar_source_freq = MSI_VALUE;
        break;

      case 6 : /* HSE */
      case 9 : /* HSE_KER */
        xbar_source_freq = HSE_VALUE;
        break;

      case 13 : /* LSI */
        xbar_source_freq = LSI_VALUE;
        break;

      case 14 : /* LSE */
        xbar_source_freq = LSE_VALUE;
        break;

      default:
        xbar_source_freq = 0;
        break;
    }

    SystemCoreClock = xbar_source_freq / (((RCC->FINDIVxCFGR[63] & RCC_FINDIVxCFGR_FINDIVx_Msk) + 1) * ((RCC->PREDIVxCFGR[63] & RCC_PREDIVxCFGR_PREDIVx_Msk) + 1));
    if ((CA35SSC->CHGCLKREQ_RW & CA35SSC_CHGCLKREQ_RW_ARM_DIVSELACK) != CA35SSC_CHGCLKREQ_RW_ARM_DIVSELACK)
    {
      SystemCoreClock /= 2;
    }
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(__attribute__((unused))uint8_t* file,
                   __attribute__((unused))uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

#ifdef __GNUC__
/* With GCC/RAISONANCE, small log_info (option LD Linker->Libraries->Small log_info
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __attribute__(( weak )) __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int __attribute__(( weak )) fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#if defined (__LOG_UART_IO_) || defined (__LOG_TRACE_IO_)
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
#if defined (__LOG_UART_IO_)
extern UART_HandleTypeDef huart;
  HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
#endif
#if defined (__LOG_TRACE_IO_)
	log_buff(ch);
#endif
	return ch;
}
#endif

/**
  * @}
  */

/**
  * @}
  */
