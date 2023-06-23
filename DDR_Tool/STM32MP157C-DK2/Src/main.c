/**
  ******************************************************************************
  * @file    Templates/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2023 STMicroelectronics.
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

  /* Init LED3 */
  BSP_LED_Init(LED_GREEN);

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
  /* Toggle LED3 ON */
  BSP_LED_Toggle(LED_GREEN);

  /* Insert 1 second delay */
  HAL_Delay(1000);

  /* Toggle LED3 OFF */
  BSP_LED_Toggle(LED_GREEN);
  }
}


/*
Component              | Parent  |   Frequency     | Status | Comment                           |
                       |         |                 | Note: the status is given before EN gate   |
-----------------------|---------|-----------------|--------|-----------------------------------|
LSI                    |N.A.     |    0.032000 MHz |   ON   | Mandatory for IWDG, DAC
  DAC                  |LSI      |    0.032000 MHz |   ON   |
LSE                    |N.A.     |    0.032768 MHz |   ON   | Mandatory for TEMP
  CEC                  |LSE      |    0.032768 MHz |   ON   |
HSI                    |N.A.     |   64.000000 MHz |   ON   |
  MCO1                 |HSI      |   64.000000 MHz |   OFF  |
HSE                    |N.A.     |   24.000000 MHz |   ON   |
  DSIPLL               |HSE      |  125.000000 MHz |   OFF  | DSI DPHY PLL max frequency
    BLCLK              |DSIPLL   |  125.000000 MHz |   OFF  | DSI lanebyte clock
  RTCDIV               |HSE      |    0.923077 MHz |   ON   |
    RTC                |RTCDIV   |    0.923077 MHz |   ON   |
  PLL1                 |HSE      | 1296.000000 MHz |   ON   |
    PLL1P              |PLL1     |  648.000000 MHz |   ON   |
      Cortex-A7        |PLL1P    |  648.000000 MHz |   ON   |
        MCO2           |Cortex-A7|  648.000000 MHz |   OFF  |
  PLL2                 |HSE      | 1056.000000 MHz |   ON   |
    PLL2P              |PLL2     |  264.000000 MHz |   ON   |
      AXI              |PLL2P    |  264.000000 MHz |   ON   |
        FMC            |AXI      |  264.000000 MHz |   ON   | NAND flash
        QSPI           |AXI      |  264.000000 MHz |   ON   | NOR flash
        AHB5           |AXI      |  264.000000 MHz |   ON   |
          HASH1        |AHB5     |  264.000000 MHz |   ON   |
          GPIOZ        |AHB5     |  264.000000 MHz |   ON   |
        AHB6           |AXI      |  264.000000 MHz |   ON   |
          USBH         |AHB6     |  264.000000 MHz |   ON   | USB Host hub (Type A)
          APB4         |AHB6     |  132.000000 MHz |   ON   |
            STGEN      |APB4     |  132.000000 MHz |   ON   |
          APB5         |AHB6     |   66.000000 MHz |   ON   | Needs to be <67MHz for BSEC
            BSEC       |APB5     |   66.000000 MHz |   ON   |
            ETZPC      |APB5     |   66.000000 MHz |   ON   |
            TZC400     |APB5     |   66.000000 MHz |   ON   |
            I2C4       |APB5     |   66.000000 MHz |   ON   | PMIC
            SPI6       |APB5     |   66.000000 MHz |   ON   |
            USART1     |APB5     |   66.000000 MHz |   ON   |
        DEBUG          |AXI      |  132.000000 MHz |   ON   | JTAG & Coresight
    PLL2Q              |PLL2     |  528.000000 MHz |   ON   |
      GPU              |PLL2Q    |  528.000000 MHz |   ON   |
    PLL2R              |PLL2     |  528.000000 MHz |   ON   |
      DDR              |PLL2R    |  528.000000 MHz |   ON   | DDR3
  PLL3                 |HSE      |  688.000000 MHz |   ON   |
    PLL3P              |PLL3     |  172.000000 MHz |   ON   |
      MLAHB            |PLL3P    |  172.000000 MHz |   ON   | <200MHz
        Cortex-M4      |MLAHB    |  172.000000 MHz |   ON   |
        MCUSRAM        |MLAHB    |  172.000000 MHz |   ON   |
        AHB1           |MLAHB    |  172.000000 MHz |   ON   | <200MHz
          DMA1/2       |AHB1     |  172.000000 MHz |   ON   |
        AHB2           |MLAHB    |  172.000000 MHz |   ON   | <200MHz
        APB1           |MLAHB    |   86.000000 MHz |   ON   |
          I2C2         |APB1     |   86.000000 MHz |   ON   | RPI2ID, USBHub, AudCodec,Cam, MFX, Disp, Eth
          I2C5         |APB1     |   86.000000 MHz |   ON   | RPI2
          UART4        |APB1     |   86.000000 MHz |   ON   | Linux console
          USART3       |APB1     |   86.000000 MHz |   ON   | RPI2
        TIM2           |MLAHB    |  172.000000 MHz |   ON   | TIMG1
        TIM12          |MLAHB    |  172.000000 MHz |   ON   | TIMG1
        APB2           |MLAHB    |   86.000000 MHz |   ON   |
        TIM8           |MLAHB    |  172.000000 MHz |   ON   | TIMG2
        APB3           |MLAHB    |   86.000000 MHz |   ON   |
        AHB3           |MLAHB    |  172.000000 MHz |   ON   | <200MHz
          DCMI         |AHB3     |  172.000000 MHz |   ON   | Camera
        AHB4           |MLAHB    |  172.000000 MHz |   ON   | <200MHz
          PWR          |AHB4     |  172.000000 MHz |   ON   |
          RCC          |AHB4     |  172.000000 MHz |   ON   |
          EXTI         |AHB4     |  172.000000 MHz |   ON   |
    PLL3Q              |PLL3     |   49.142857 MHz |   ON   |
      SPDIF            |PLL3Q    |   49.142857 MHz |   ON   | SPDIF RX
      DFSDM            |PLL3Q    |   49.142857 MHz |   ON   | Digital micro
      SAI2             |PLL3Q    |   49.142857 MHz |   ON   | AudCodec 48kHz (use PLL4Q 79MHz for 44.1kHz)
      SAI4             |PLL3Q    |   49.142857 MHz |   ON   | SPDIF TX 48kHz (use PLL4Q 79MHz for 44.1kHz)
    PLL3R              |PLL3     |  172.000000 MHz |   ON   |
      SDMMC1           |PLL3R    |  172.000000 MHz |   ON   | µSD card
      SDMMC2           |PLL3R    |  172.000000 MHz |   ON   | eMMC
      SDMMC3           |PLL3R    |  172.000000 MHz |   ON   | Wifi
  PLL4                 |HSE      |  632.000000 MHz |   ON   |
    PLL4P              |PLL4     |  126.400002 MHz |   ON   |
    PLL4Q              |PLL4     |   79.000000 MHz |   ON   |
      LCD              |PLL4Q    |   79.000000 MHz |   ON   | LTDC & DSI display pixel clock
      ADC              |PLL4Q    |   79.000000 MHz |   ON   | Use ADC internal divider to be <72MHz
      FDCAN            |PLL4Q    |   79.000000 MHz |   ON   |
    PLL4R              |PLL4     |   63.200001 MHz |   ON   |
  USBPHYC              |HSE      |   24.000000 MHz |   ON   | USB PHY Ctrl for USB Host and OTG
    USBPLL             |USBPHYC  |   48.000000 MHz |   ON   |
      USBO             |USBPLL   |   48.000000 MHz |   ON   | USB OTG (micro AB)
CSI                    |N.A.     |    4.000000 MHz |   ON   | Mandatory for IO compensation
ck_per                 |N.A.     |    0.000000 MHz |   OFF  |
ETH                    |N.A.     |    0.000000 MHz |   OFF  | ETH clocked by RGMII PHY on EVAL
-----------------------|---------|-----------------|--------|-----------------------------------*/
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
  RCC_OscInitStructure.PLL.PLLM = 2;
  RCC_OscInitStructure.PLL.PLLN = 54;
  RCC_OscInitStructure.PLL.PLLP = 1;
  RCC_OscInitStructure.PLL.PLLQ = 1;
  RCC_OscInitStructure.PLL.PLLR = 1;
  RCC_OscInitStructure.PLL.PLLFRACV = 0;
  RCC_OscInitStructure.PLL.PLLMODE = RCC_PLL_INTEGER;

  RCC_OscInitStructure.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL2.PLLSource = RCC_PLL12SOURCE_HSE;
  RCC_OscInitStructure.PLL2.PLLM = UTIL_PLL2_M;
  RCC_OscInitStructure.PLL2.PLLN = UTIL_PLL2_N;
  RCC_OscInitStructure.PLL2.PLLP = UTIL_PLL2_P;
  RCC_OscInitStructure.PLL2.PLLQ = UTIL_PLL2_Q;
  RCC_OscInitStructure.PLL2.PLLR = UTIL_PLL2_R;
  RCC_OscInitStructure.PLL2.PLLFRACV = UTIL_PLL2_FRACV;
  RCC_OscInitStructure.PLL2.PLLMODE = UTIL_PLL2_MODE;

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
  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);

  RCC_ClkInitStructure.MPUInit.MPU_Clock = RCC_MPUSOURCE_PLL1;
  RCC_ClkInitStructure.AXISSInit.AXI_Clock = RCC_AXISSOURCE_PLL2;
  RCC_ClkInitStructure.AXISSInit.AXI_Div = RCC_AXI_DIV1;
  RCC_ClkInitStructure.MPUInit.MPU_Div = RCC_MPU_DIV2;
  RCC_ClkInitStructure.APB4_Div = RCC_APB4_DIV2;
  RCC_ClkInitStructure.APB5_Div = RCC_APB5_DIV4;
  RCC_ClkInitStructure.MCUInit.MCU_Clock = RCC_MCUSSOURCE_HSI;
  RCC_ClkInitStructure.MCUInit.MCU_Div = RCC_MCU_DIV1;
  RCC_ClkInitStructure.APB1_Div = RCC_APB1_DIV2;
  RCC_ClkInitStructure.APB2_Div = RCC_APB2_DIV2;
  RCC_ClkInitStructure.APB3_Div = RCC_APB3_DIV2;

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
  __HAL_RCC_TZC1_CLK_ENABLE();
  __HAL_RCC_TZC2_CLK_ENABLE();

  /* Configure TZC to allow  DDR Region0 R/W  non secure for all IDs */
  TZC->GATE_KEEPER =0;
  TZC->REG_ID_ACCESSO=0xFFFFFFFF;  //Allow DDR Region0 R/W  non secure for all IDs
  TZC->REG_ATTRIBUTESO = 0xC0000003;
  TZC->GATE_KEEPER |=3;  //Enable the access in secure Mode  // filter 0 request close

  /* Enable TZPC & backup ram for security tests */
  __HAL_RCC_TZPC_CLK_ENABLE();
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
