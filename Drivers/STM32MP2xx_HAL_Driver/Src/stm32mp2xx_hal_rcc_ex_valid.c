/**
  ******************************************************************************
  * @file    stm32mp2xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extended RCC HAL module driver.
  *
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

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal.h"

#ifdef HAL_RCC_VALID_MODULE_ENABLED

/** @addtogroup STM32MP2xx_HAL_Driver
  * @{
  */

/** @defgroup RCCEx RCCEx
  * @brief RCC HAL module driver
  * @{
  */

/**
  * @brief  Compute PLL parameters
  * @param  fref: Input frequency
  * @param  fvco: Expected VCO frequency (before any postdiv)
  * @param  pll: pointer to a prefilled RCC_PLLCfgTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_GetPLLParams(uint32_t fref, uint32_t fvco, RCC_PLLCfgTypeDef *pll)
{
  if (!fref || !fvco || !pll)
  {
    return HAL_ERROR;
  }

  if ((pll->flags & RCC_PLLFLAG_FRACTIONAL) == 0)
  {
    /* Integer mode
       FOUT = (FREF*FBDIV) / (REFDIV*POSTDIV)
       FVCO = FOUT*POSTDIV = (FREF*FBDIV) / REFDIV
       FREF: from 5 to 1200Mhz
       FVCO: From 800 to 3200Mhz
       FBDIV: From 16 to 640
     */
    if (fref < 5000000)
    {
      return HAL_ERROR;
    }
    if (fref > 1200000000)
    {
      return HAL_ERROR;
    }

    return HAL_ERROR; // Currently unsupported
  }
  else
  {
    /* Fractional mode.
       FREF: from 10 to 1200Mhz
       FVCO: From 800 to 3200Mhz
       FBDIV: From 20 to 320
     */
    if (fref < 10000000)
    {
      return HAL_ERROR;
    }
    if (fref > 1200000000)
    {
      return HAL_ERROR;
    }

    return HAL_ERROR; // Currently unsupported
  }

  return HAL_OK;
}

/**
  * @brief  Configures PLL
  * @param  pll: pointer to an RCC_PLLCfgTypeDef structure
  *
  * @retval HAL status
  */

#if 0 // FCh: Obsolete function. Replaced by HAL_RCC_SetPllCfg()
/**
  * @brief  Configures PLL2
  * @param  pll2: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */

HAL_StatusTypeDef RCCEx_PLL2_Config(RCC_PLLInitTypeDef *pll2)
{
#if O_CODE
  uint32_t tickstart = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PLL(pll2->PLLState));
  if ((pll2->PLLState) != RCC_PLL_NONE)
  {
    /* Check if the PLL is used as system clock or not (MPU, MCU, AXISS)*/
    if (!__IS_PLL2_IN_USE()) /* If not used then */
    {
      if ((pll2->PLLState) == RCC_PLL_ON)
      {
        /* Check the parameters */
        assert_param(IS_RCC_PLLMODE(pll2->PLLMODE));
        assert_param(IS_RCC_PLL12SOURCE(pll2->PLLSource));
        assert_param(IS_RCC_PLLM2_VALUE(pll2->PLLM));
        if (pll2->PLLMODE == RCC_PLL_FRACTIONAL)
        {
          assert_param(IS_RCC_PLLN2_FRAC_VALUE(pll2->PLLN));
        }
        else
        {
          assert_param(IS_RCC_PLLN2_INT_VALUE(pll2->PLLN));
        }
        assert_param(IS_RCC_PLLP2_VALUE(pll2->PLLP));
        assert_param(IS_RCC_PLLQ2_VALUE(pll2->PLLQ));
        assert_param(IS_RCC_PLLR2_VALUE(pll2->PLLR));

        /* @TODO: Erase this switch once SoC available */
#if !defined(USE_FPGA_BOARD)
        /* Check that PLL2 OSC clock source is already set */
        if ((__HAL_RCC_GET_PLL12_SOURCE() != RCC_PLL12SOURCE_HSI) &&
            (__HAL_RCC_GET_PLL12_SOURCE() != RCC_PLL12SOURCE_HSE))
        {
          return HAL_ERROR;
        }
#endif

        /*Disable the post-dividers*/
        __HAL_RCC_PLL2CLKOUT_DISABLE(RCC_PLL2_DIVP | RCC_PLL2_DIVQ | RCC_PLL2_DIVR);
        /* Disable the main PLL. */
        __HAL_RCC_PLL2_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) != RESET)
        {
          if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /*The PLL configuration below must be done before enabling the PLL:
        -Selection of PLL clock entry (HSI or HSE)
        -Frequency input range (PLLxRGE)
        -Division factors (DIVMx, DIVNx, DIVPx, DIVQx & DIVRx)
        Once the PLL is enabled, these parameters can not be changed.
        If the User wants to change the PLL parameters he must disable the concerned PLL (PLLxON=0) and wait for the
        PLLxRDY flag to be at 0.
        The PLL configuration below can be done at any time:
        -Enable/Disable of output clock dividers (DIVPxEN, DIVQxEN & DIVRxEN)
        -Fractional Division Enable (PLLxFRACNEN)
        -Fractional Division factor (FRACNx)*/

        /* Do not change pll src if already in use */
        if (__IS_PLL1_IN_USE())
        {
          if (pll2->PLLSource != __HAL_RCC_GET_PLL12_SOURCE())
          {
            return HAL_ERROR;
          }
        }
        else
        {
          /* Configure PLL1 and PLL2 clock source */
          __HAL_RCC_PLL12_SOURCE(pll2->PLLSource);
        }

        /* Configure the PLL2 multiplication and division factors. */
        __HAL_RCC_PLL2_CONFIG(
          pll2->PLLM,
          pll2->PLLN,
          pll2->PLLP,
          pll2->PLLQ,
          pll2->PLLR);

        /* Configure the Fractional Divider */
        __HAL_RCC_PLL2FRACV_DISABLE(); //Set FRACLE to ‘0’
        /* In integer or clock spreading mode the application shall ensure that a 0 is loaded into the SDM */
        if ((pll2->PLLMODE == RCC_PLL_SPREAD_SPECTRUM) || (pll2->PLLMODE == RCC_PLL_INTEGER))
        {
          /* Do not use the fractional divider */
          __HAL_RCC_PLL2FRACV_CONFIG(0); //Set FRACV to '0'
        }
        else
        {
          /* Configure PLL  PLL2FRACV  in fractional mode*/
          __HAL_RCC_PLL2FRACV_CONFIG(pll2->PLLFRACV);
        }
        __HAL_RCC_PLL2FRACV_ENABLE(); //Set FRACLE to ‘1’

        /* Configure the Spread Control */
        if (pll2->PLLMODE == RCC_PLL_SPREAD_SPECTRUM)
        {
          assert_param(IS_RCC_INC_STEP(pll2->INC_STEP));
          assert_param(IS_RCC_SSCG_MODE(pll2->SSCG_MODE));
          assert_param(IS_RCC_RPDFN_DIS(pll2->RPDFN_DIS));
          assert_param(IS_RCC_TPDFN_DIS(pll2->TPDFN_DIS));
          assert_param(IS_RCC_MOD_PER(pll2->MOD_PER));

          __HAL_RCC_PLL2CSGCONFIG(pll2->MOD_PER, pll2->TPDFN_DIS, pll2->RPDFN_DIS,
                                  pll2->SSCG_MODE, pll2->INC_STEP);
          __HAL_RCC_PLL2_SSMODE_ENABLE();
        }
        else
        {
          __HAL_RCC_PLL2_SSMODE_DISABLE();
        }

        /* Enable the PLL2. */
        __HAL_RCC_PLL2_ENABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) == RESET)
        {
          if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
        /*Enable the post-dividers*/
        __HAL_RCC_PLL2CLKOUT_ENABLE(RCC_PLL2_DIVP | RCC_PLL2_DIVQ | RCC_PLL2_DIVR);
      }
      else
      {
        /*Disable the post-dividers*/
        __HAL_RCC_PLL2CLKOUT_DISABLE(RCC_PLL2_DIVP | RCC_PLL2_DIVQ | RCC_PLL2_DIVR);
        /* Disable the PLL2. */
        __HAL_RCC_PLL2_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) != RESET)
        {
          if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    else
    {
      return HAL_ERROR;
    }
  }
#endif
  return HAL_OK;
}
#endif

#if 0 // FCh: Obsolete function. Replaced by HAL_RCC_SetPllCfg()
/**
  * @brief  Configures PLL3
  * @param  pll3: pointer to a RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef RCCEx_PLL3_Config(RCC_PLLInitTypeDef *pll3)
{
#if O_CODE
  uint32_t tickstart = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PLL(pll3->PLLState));
  if ((pll3->PLLState) != RCC_PLL_NONE)
  {
    /* Check if the PLL is used as system clock or not (MPU, MCU, AXISS)*/
    if (!__IS_PLL3_IN_USE()) /* If not used then*/
    {
      if ((pll3->PLLState) == RCC_PLL_ON)
      {
        /* Check the parameters */
        assert_param(IS_RCC_PLLMODE(pll3->PLLMODE));
        assert_param(IS_RCC_PLL3SOURCE(pll3->PLLSource));
        assert_param(IS_RCC_PLLM1_VALUE(pll3->PLLM));
        if (pll3->PLLMODE == RCC_PLL_FRACTIONAL)
        {
          assert_param(IS_RCC_PLLN3_FRAC_VALUE(pll3->PLLN));
        }
        else
        {
          assert_param(IS_RCC_PLLN3_INT_VALUE(pll3->PLLN));
        }
        assert_param(IS_RCC_PLLP3_VALUE(pll3->PLLP));
        assert_param(IS_RCC_PLLQ3_VALUE(pll3->PLLQ));
        assert_param(IS_RCC_PLLR3_VALUE(pll3->PLLR));

        /*Disable the post-dividers*/
        __HAL_RCC_PLL3CLKOUT_DISABLE(RCC_PLL3_DIVP | RCC_PLL3_DIVQ | RCC_PLL3_DIVR);
        /* Disable the main PLL. */
        __HAL_RCC_PLL3_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL3RDY) != RESET)
        {
          if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /*The PLL configuration below must be done before enabling the PLL:
        -Selection of PLL clock entry (HSI or CSI or HSE)
        -Frequency input range (PLLxRGE)
        -Division factors (DIVMx, DIVNx, DIVPx, DIVQx & DIVRx)
        Once the PLL is enabled, these parameters can not be changed.
        If the User wants to change the PLL parameters he must disable the concerned PLL (PLLxON=0) and wait for the
        PLLxRDY flag to be at 0.
        The PLL configuration below can be done at any time:
        -Enable/Disable of output clock dividers (DIVPxEN, DIVQxEN & DIVRxEN)
        -Fractional Division Enable (PLLxFRACNEN)
        -Fractional Division factor (FRACNx)*/

        /* Configure PLL3 clock source */
        __HAL_RCC_PLL3_SOURCE(pll3->PLLSource);

        /* @TODO: Erase this switch once SoC available */
#if !defined(USE_FPGA_BOARD)
        /* Wait till PLL SOURCE is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL3SRCRDY) == RESET)
        {
          if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
#endif

        /* Select PLL3 input reference frequency range */
        __HAL_RCC_PLL3_IFRANGE(pll3->PLLRGE) ;

        /* Configure the PLL3 multiplication and division factors. */
        __HAL_RCC_PLL3_CONFIG(
          pll3->PLLM,
          pll3->PLLN,
          pll3->PLLP,
          pll3->PLLQ,
          pll3->PLLR);

        /* Configure the Fractional Divider */
        __HAL_RCC_PLL3FRACV_DISABLE(); //Set FRACLE to ‘0’
        /* In integer or clock spreading mode the application shall ensure that a 0 is loaded into the SDM */
        if ((pll3->PLLMODE == RCC_PLL_SPREAD_SPECTRUM) || (pll3->PLLMODE == RCC_PLL_INTEGER))
        {
          /* Do not use the fractional divider */
          __HAL_RCC_PLL3FRACV_CONFIG(0); //Set FRACV to '0'
        }
        else
        {
          /* Configure PLL  PLL3FRACV  in fractional mode*/
          __HAL_RCC_PLL3FRACV_CONFIG(pll3->PLLFRACV);
        }
        __HAL_RCC_PLL3FRACV_ENABLE(); //Set FRACLE to ‘1’

        /* Configure the Spread Control */
        if (pll3->PLLMODE == RCC_PLL_SPREAD_SPECTRUM)
        {
          assert_param(IS_RCC_INC_STEP(pll3->INC_STEP));
          assert_param(IS_RCC_SSCG_MODE(pll3->SSCG_MODE));
          assert_param(IS_RCC_RPDFN_DIS(pll3->RPDFN_DIS));
          assert_param(IS_RCC_TPDFN_DIS(pll3->TPDFN_DIS));
          assert_param(IS_RCC_MOD_PER(pll3->MOD_PER));

          __HAL_RCC_PLL3CSGCONFIG(pll3->MOD_PER, pll3->TPDFN_DIS, pll3->RPDFN_DIS,
                                  pll3->SSCG_MODE, pll3->INC_STEP);
          __HAL_RCC_PLL3_SSMODE_ENABLE();
        }
        else
        {
          __HAL_RCC_PLL3_SSMODE_DISABLE();
        }

        /* Enable the PLL3. */
        __HAL_RCC_PLL3_ENABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL3RDY) == RESET)
        {
          if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
        /* Enable the post-dividers */
        __HAL_RCC_PLL3CLKOUT_ENABLE(RCC_PLL3_DIVP | RCC_PLL3_DIVQ | RCC_PLL3_DIVR);
      }
      else
      {
        /*Disable the post-dividers*/
        __HAL_RCC_PLL3CLKOUT_DISABLE(RCC_PLL3_DIVP | RCC_PLL3_DIVQ | RCC_PLL3_DIVR);
        /* Disable the PLL3. */
        __HAL_RCC_PLL3_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL3RDY) != RESET)
        {
          if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    else
    {
      return HAL_ERROR;
    }
  }
#endif
  return HAL_OK;
}
#endif

#if 0 // FCh: Obsolete function. Replaced by HAL_RCC_SetPllCfg()
/**
  * @brief  Configures PLL4
  * @param  pll4: pointer to a RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef RCCEx_PLL4_Config(RCC_PLLInitTypeDef *pll4)
{
#if O_CODE
  uint32_t tickstart = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PLL(pll4->PLLState));
  if ((pll4->PLLState) != RCC_PLL_NONE)
  {

    if ((pll4->PLLState) == RCC_PLL_ON)
    {
      /* Check the parameters */
      assert_param(IS_RCC_PLLMODE(pll4->PLLMODE));
      assert_param(IS_RCC_PLL4SOURCE(pll4->PLLSource));
      assert_param(IS_RCC_PLLM4_VALUE(pll4->PLLM));
      if (pll4->PLLMODE == RCC_PLL_FRACTIONAL)
      {
        assert_param(IS_RCC_PLLN4_FRAC_VALUE(pll4->PLLN));
      }
      else
      {
        assert_param(IS_RCC_PLLN4_INT_VALUE(pll4->PLLN));
      }
      assert_param(IS_RCC_PLLP4_VALUE(pll4->PLLP));
      assert_param(IS_RCC_PLLQ4_VALUE(pll4->PLLQ));
      assert_param(IS_RCC_PLLR4_VALUE(pll4->PLLR));

      /*Disable the post-dividers*/
      __HAL_RCC_PLL4CLKOUT_DISABLE(RCC_PLL4_DIVP | RCC_PLL4_DIVQ | RCC_PLL4_DIVR);
      /* Disable the main PLL. */
      __HAL_RCC_PLL4_DISABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till PLL is ready */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL4RDY) != RESET)
      {
        if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }

      /*The PLL configuration below must be done before enabling the PLL:
      -Selection of PLL clock entry (HSI or CSI or HSE)
      -Frequency input range (PLLxRGE)
      -Division factors (DIVMx, DIVNx, DIVPx, DIVQx & DIVRx)
      Once the PLL is enabled, these parameters can not be changed.
      If the User wants to change the PLL parameters he must disable the concerned PLL (PLLxON=0) and wait for the
      PLLxRDY flag to be at 0.
      The PLL configuration below can be done at any time:
      -Enable/Disable of output clock dividers (DIVPxEN, DIVQxEN & DIVRxEN)
      -Fractional Division Enable (PLLxFRACNEN)
      -Fractional Division factor (FRACNx)*/

      /* Configure PLL4 and PLL4 clock source */
      __HAL_RCC_PLL4_SOURCE(pll4->PLLSource);

      /* @TODO: Erase this switch once SoC available */
#if !defined(USE_FPGA_BOARD)
      /* Wait till PLL SOURCE is ready */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL4SRCRDY) == RESET)
      {
        if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
#endif

      /* Select PLL4 input reference frequency range */
      __HAL_RCC_PLL4_IFRANGE(pll4->PLLRGE) ;

      /* Configure the PLL4 multiplication and division factors. */
      __HAL_RCC_PLL4_CONFIG(
        pll4->PLLM,
        pll4->PLLN,
        pll4->PLLP,
        pll4->PLLQ,
        pll4->PLLR);

      /* Configure the Fractional Divider */
      __HAL_RCC_PLL4FRACV_DISABLE(); //Set FRACLE to ‘0’
      /* In integer or clock spreading mode the application shall ensure that a 0 is loaded into the SDM */
      if ((pll4->PLLMODE == RCC_PLL_SPREAD_SPECTRUM) || (pll4->PLLMODE == RCC_PLL_INTEGER))
      {
        /* Do not use the fractional divider */
        __HAL_RCC_PLL4FRACV_CONFIG(0); //Set FRACV to '0'
      }
      else
      {
        /* Configure PLL  PLL4FRACV  in fractional mode*/
        __HAL_RCC_PLL4FRACV_CONFIG(pll4->PLLFRACV);
      }
      __HAL_RCC_PLL4FRACV_ENABLE(); //Set FRACLE to ‘1’

      /* Configure the Spread Control */
      if (pll4->PLLMODE == RCC_PLL_SPREAD_SPECTRUM)
      {
        assert_param(IS_RCC_INC_STEP(pll4->INC_STEP));
        assert_param(IS_RCC_SSCG_MODE(pll4->SSCG_MODE));
        assert_param(IS_RCC_RPDFN_DIS(pll4->RPDFN_DIS));
        assert_param(IS_RCC_TPDFN_DIS(pll4->TPDFN_DIS));
        assert_param(IS_RCC_MOD_PER(pll4->MOD_PER));

        __HAL_RCC_PLL4CSGCONFIG(pll4->MOD_PER, pll4->TPDFN_DIS, pll4->RPDFN_DIS,
                                pll4->SSCG_MODE, pll4->INC_STEP);
        __HAL_RCC_PLL4_SSMODE_ENABLE();
      }
      else
      {
        __HAL_RCC_PLL4_SSMODE_DISABLE();
      }

      /* Enable the PLL4. */
      __HAL_RCC_PLL4_ENABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till PLL is ready */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL4RDY) == RESET)
      {
        if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
      /* Enable PLL4P Clock output. */
      __HAL_RCC_PLL4CLKOUT_ENABLE(RCC_PLL4_DIVP | RCC_PLL4_DIVQ | RCC_PLL4_DIVR);
    }
    else
    {
      /*Disable the post-dividers*/
      __HAL_RCC_PLL4CLKOUT_DISABLE(RCC_PLL4_DIVP | RCC_PLL4_DIVQ | RCC_PLL4_DIVR);
      /* Disable the PLL4. */
      __HAL_RCC_PLL4_DISABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till PLL is ready */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL4RDY) != RESET)
      {
        if ((int32_t)(HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
  }
#endif
  return HAL_OK;
}
#endif

#if 0 // FCh: Obsolete. Replace by HAL_RCC_EnableCSS()
/**
  * @brief  Enables the LSE Clock Security System.
  * @note   After reset BDCR register is write-protected and the DBP bit in the
  *         PWR control register 1 (PWR_CR1) has to be set before it can be written.
  * @note   Prior to enable the LSE Clock Security System, LSE oscillator is to be enabled
  *         with HAL_RCC_OscConfig() and the LSE oscillator clock is to be selected as RTC
  *         clock with HAL_RCCEx_PeriphCLKConfig().
  * @retval None
  */
void HAL_RCCEx_EnableLSECSS(void)
{
#if O_CODE
  /* Set LSECSSON bit */
  SET_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
#endif
}
#endif

#if 0 // FCh: Obsolete. LSECSS can't be disabled by SW.
/**
  * @brief  Disables the LSE Clock Security System.
  * @note   LSE Clock Security System can only be disabled after a LSE failure detection.
  * @note   After reset BDCR register is write-protected and the DBP bit in the
  *         PWR control register 1 (PWR_CR1) has to be set before it can be written.
  * @retval None
  */
void HAL_RCCEx_DisableLSECSS(void)
{
#if O_CODE
  /* Unset LSECSSON bit */
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
#endif
}
#endif

/* FCh: Obsolete function. LSE drive is done part during HAL_RCC_SetOscCfg() call */
#if 0
HAL_StatusTypeDef RCC_LSEDriveConfig(uint32_t LseDriveValue)
{
#if O_CODE
  uint32_t count = 0;
  HAL_StatusTypeDef result = HAL_OK;

  /* Check the LSEDrive parameter */
  assert_param(IS_RCC_LSEDRIVE(LseDriveValue));

#define LSEDRIVE_PRE_NUM 4   //Number of possible LSE driving capabilities
  const uint32_t  LSEDRIVE_PRE[LSEDRIVE_PRE_NUM] = {RCC_LSEDRIVE_LOW, RCC_LSEDRIVE_MEDIUMLOW, RCC_LSEDRIVE_MEDIUMHIGH, RCC_LSEDRIVE_HIGH};

  /* Modify the LSE Driving capability if needed */
  if (LseDriveValue != __HAL_RCC_GET_LSEDRIVE())
  {
    switch (__HAL_RCC_GET_LSEDRIVE())
    {
      case RCC_LSEDRIVE_LOW:
        count = 0;
        break;

      case RCC_LSEDRIVE_MEDIUMLOW:
        count = 1;
        break;

      case RCC_LSEDRIVE_MEDIUMHIGH:
        count = 2;
        break;

      case RCC_LSEDRIVE_HIGH:
        count = 3;
        break;
    }

    /* Enable write access to Backup domain */
    SET_BIT(PWR->CR1, PWR_CR1_DBP);
    /* @TODO: Analyze if using HAL_PWR_EnableBkUpAccess instead */
    //HAL_PWR_EnableBkUpAccess();

    if (__HAL_RCC_GET_LSEDRIVE() < LseDriveValue)  // LSEDRIVE in device is smaller than desired
    {
      while ((count < LSEDRIVE_PRE_NUM) && (__HAL_RCC_GET_LSEDRIVE() != LseDriveValue))
      {
        count++;
        __HAL_RCC_LSEDRIVE_CONFIG(LSEDRIVE_PRE[count]);
        /* @TODO: Use 7 NOPs here not 1ms  */
        HAL_Delay(1);
      }
    }
    else
    {
      while ((count > 0) && (__HAL_RCC_GET_LSEDRIVE() != LseDriveValue))
      {
        count--;
        __HAL_RCC_LSEDRIVE_CONFIG(LSEDRIVE_PRE[count]);
        /* @TODO: Use 7 NOPs here not 1ms  */
        HAL_Delay(1);
      }
    }
  }//LSE Driving capability modified

  if (LseDriveValue != __HAL_RCC_GET_LSEDRIVE())
  {
    result = HAL_ERROR;
  }

  return result;
#endif
  return 0;
}
#endif

#ifdef CORE_CA35
/**
  * @brief  Control the enable boot function when the system exits from STANDBY
  * @param  RCC_BootCx: Boot Core to be enabled (set to 1)
  *         This parameter can be one (or both) of the following values:
  *            @arg RCC_BOOT_C1: CA35 core selection
  *            @arg RCC_BOOT_C2: CM33 core selection
  *
  * @note   Next combinations are possible:
  *           RCC_BOOT_C1   RCC_BOOT_C2  Expected result
  *               0              0       MPU boots, MCU does not boot
  *               0              1       Only MCU boots
  *               1              0       Only MPU boots
  *               1              1       MPU and MCU boot
  *
  * @note   This function is reset when a system reset occurs, but not when the
  *         circuit exits from STANDBY (rst_app reset)
  * @note   This function can only be called by the CA35
  * @retval None
  */
void HAL_RCCEx_EnableBootCore(uint32_t RCC_BootCx)
{
  assert_param(IS_RCC_BOOT_CORE(RCC_BootCx));
  SET_BIT(RCC->STBYBOOTCR, RCC_BootCx);
}

/**
  * @brief  Control the disable boot function when the system exits from STANDBY
  * @param  RCC_BootCx: Boot Core to be disabled (set to 0)
  *         This parameter can be one (or both) of the following values:
  *            @arg RCC_BOOT_C1: CA35 core selection
  *            @arg RCC_BOOT_C2: CM33 core selection
  *
  * @note   Next combinations are possible:
  *           RCC_BOOT_C1   RCC_BOOT_C2  Expected result
  *               0              0       MPU boots, MCU does not boot
  *               0              1       Only MCU boots
  *               1              0       Only MPU boots
  *               1              1       MPU and MCU boot
  *
  * @note   This function is reset when a system reset occurs, but not when the
  *         circuit exits from STANDBY (rst_app reset)
  * @note   This function can only be called by the CA35
  * @retval None
  */
void HAL_RCCEx_DisableBootCore(uint32_t RCC_BootCx)
{
  assert_param(IS_RCC_BOOT_CORE(RCC_BootCx));
  CLEAR_BIT(RCC->STBYBOOTCR, RCC_BootCx);
}

/**
  * @brief  The MCU will be set in HOLD_BOOT when the next MCU core reset occurs
  * @retval None
  */
void HAL_RCCEx_HoldBootMCU(void)
{
  CLEAR_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU2);
}

/**
  * @brief  The MCU will not be in HOLD_BOOT mode when the next MCU core reset occurs.
  * @note   If the MCU was in HOLD_BOOT it will cause the MCU to boot.
  * @retval None
  */
void HAL_RCCEx_BootMCU(void)
{
  SET_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU2);
}

#endif /* CORE_CA35 */

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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

