/**
  ******************************************************************************
  * @file    Templates/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32mp_cubemx_conf.h"

/** @addtogroup STM32MP1xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void SystemTZC_Config(void);

/* Private functions ---------------------------------------------------------*/

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

  /* Configure the system clock */
  SystemClock_Config();

  /*  Configure USART for logging messages  */
  UART_Config();

  /*****************************/
  /* Test environment settings */
  /*****************************/

  /* Configure the system power */
  SystemPower_Config();

  /* TZC configuration */
  SystemTZC_Config();

  /* Insert 1 second delay */
  HAL_Delay(1000);

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
  /* Insert 1 second delay */
  HAL_Delay(1000);
  }
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStructure;
  RCC_OscInitTypeDef RCC_OscInitStructure;

  /* Reset clocks to a known state */
  HAL_RCC_DeInit();

  /* Enable all available oscillators*/
  RCC_OscInitStructure.OscillatorType = (RCC_OSCILLATORTYPE_HSI
      | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_CSI |
      RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE);

  RCC_OscInitStructure.HSIState = RCC_HSI_ON;
  RCC_OscInitStructure.HSEState = RCC_HSE_ON;
  RCC_OscInitStructure.LSEState = RCC_LSE_ON;
  RCC_OscInitStructure.LSIState = RCC_LSI_ON;
  RCC_OscInitStructure.CSIState = RCC_CSI_ON;

  RCC_OscInitStructure.HSICalibrationValue = 0x00; //Default reset value
  RCC_OscInitStructure.CSICalibrationValue = 0x10; //Default reset value
  RCC_OscInitStructure.HSIDivValue = RCC_HSI_DIV1; //Default value

  /* PLL configuration */
  RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL.PLLSource = RCC_PLL12SOURCE_HSE;
  RCC_OscInitStructure.PLL.PLLM = 3;
  RCC_OscInitStructure.PLL.PLLN = 81;
  RCC_OscInitStructure.PLL.PLLP = 1;
  RCC_OscInitStructure.PLL.PLLQ = 2;
  RCC_OscInitStructure.PLL.PLLR = 2;
  RCC_OscInitStructure.PLL.PLLFRACV = 0x800;
  RCC_OscInitStructure.PLL.PLLMODE = RCC_PLL_FRACTIONAL;

  RCC_OscInitStructure.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL2.PLLSource = RCC_PLL12SOURCE_HSE;
  RCC_OscInitStructure.PLL2.PLLM = MX_PLL2_M;
  RCC_OscInitStructure.PLL2.PLLN = MX_PLL2_N;
  RCC_OscInitStructure.PLL2.PLLP = MX_PLL2_P;
  RCC_OscInitStructure.PLL2.PLLQ = MX_PLL2_Q;
  RCC_OscInitStructure.PLL2.PLLR = MX_PLL2_R;
  RCC_OscInitStructure.PLL2.PLLFRACV = MX_PLL2_FRACV;
  RCC_OscInitStructure.PLL2.PLLMODE = MX_PLL2_MODE;

  RCC_OscInitStructure.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStructure.PLL4.PLLState = RCC_PLL_NONE;

  /* Enable access to RTC and backup registers */
  SET_BIT(PWR->CR1, PWR_CR1_DBP);
  /* Configure LSEDRIVE value */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);

  if (HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK) {
    /* HAL RCC configuration error */
    Error_Handler();
  }

  /* Select PLLx as MPU, AXI and MCU clock sources */
  RCC_ClkInitStructure.ClockType = (RCC_CLOCKTYPE_MPU | RCC_CLOCKTYPE_ACLK |
  RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK4 |
  RCC_CLOCKTYPE_PCLK5 | RCC_CLOCKTYPE_PCLK1 |
  RCC_CLOCKTYPE_PCLK6 |
  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);

  RCC_ClkInitStructure.MPUInit.MPU_Clock = RCC_MPUSOURCE_PLL1;
  RCC_ClkInitStructure.AXISSInit.AXI_Clock = RCC_AXISSOURCE_PLL2;
  RCC_ClkInitStructure.AXISSInit.AXI_Div = RCC_AXI_DIV1;
  RCC_ClkInitStructure.MPUInit.MPU_Div = RCC_MPU_DIV2;
  RCC_ClkInitStructure.APB4_Div = RCC_APB4_DIV2;
  RCC_ClkInitStructure.APB5_Div = RCC_APB5_DIV4;
  RCC_ClkInitStructure.MLAHBInit.MLAHB_Clock = RCC_MLAHBSSOURCE_HSI;
  RCC_ClkInitStructure.MLAHBInit.MLAHB_Div = RCC_MLAHB_DIV1;
  RCC_ClkInitStructure.APB1_Div = RCC_APB1_DIV2;
  RCC_ClkInitStructure.APB2_Div = RCC_APB2_DIV2;
  RCC_ClkInitStructure.APB3_Div = RCC_APB3_DIV2;
  RCC_ClkInitStructure.APB6_Div = RCC_APB6_DIV1;

  RCC->MLAHBDIVR = RCC_MLAHB_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStructure) != HAL_OK) {
    /* HAL RCC configuration error */
    Error_Handler();
  }

  /*
   Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
   (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
   The I/O Compensation Cell activation  procedure requires :
   - The activation of the CSI clock
   - The activation of the SYSCFG clock
   - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

   To do this please uncomment the following code
   */

  /*
   __HAL_RCC_CSI_ENABLE() ;

   __HAL_RCC_SYSCFG_CLK_ENABLE() ;

   HAL_EnableCompensationCell();
   */
}

/**
  * @brief  This function handles TZC configuration.
  * @param  None
  * @retval None
  */
void SystemTZC_Config(void)
{
  log_dbg("TZC configuration \n\r");

  /*Enable TZC clock*/
  __HAL_RCC_TZC_CLK_ENABLE();

  /* Configure TZC to allow  DDR Region0 R/W  non secure for all IDs */
  TZC->GATE_KEEPER =0;
  TZC->REG_ID_ACCESSO=0xFFFFFFFF;  //Allow DDR Region0 R/W  non secure for all IDs
  TZC->REG_ATTRIBUTESO = 0xC0000003;
  TZC->GATE_KEEPER |=3;  //Enable the access in secure Mode  // filter 0 request close

  /* Enable TZPC & backup ram for security tests */
  __HAL_RCC_ETZPC_CLK_ENABLE();
  __HAL_RCC_BKPSRAM_CLK_ENABLE();

  /* Disable both read and write access speculations */
  TZC->SPECULATION_CTRL = 3;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
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
