/**
  ******************************************************************************
  * @file    ddr_tests.h
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    15-April-2021
  * @brief   Header for ddr_tests.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DDR_TESTS_H
#define __DDR_TESTS_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t DDR_Test_Databus(unsigned long addr_in);
uint32_t DDR_Test_DatabusWalk0(unsigned long loop, unsigned long addr);
uint32_t DDR_Test_DatabusWalk1(unsigned long loop, unsigned long addr);
uint32_t DDR_Test_AddressBus(unsigned long size, unsigned long addr_in);
uint32_t DDR_Test_MemDevice(unsigned long size, unsigned long addr_in);
uint32_t DDR_Test_SimultaneousSwitchingOutput(unsigned long size,
                                              unsigned long addr_in);
uint32_t DDR_Test_Noise(unsigned long pattern_in, unsigned long addr_in);
uint32_t DDR_Test_NoiseBurst(unsigned long size_in, unsigned long pattern_in,
                             unsigned long addr_in);
uint32_t DDR_Test_Random(unsigned long size, unsigned long loop_in,
                         unsigned long addr_in);
uint32_t DDR_Test_FrequencySelectivePattern(unsigned long size,
                                            unsigned long addr_in);
uint32_t DDR_Test_BlockSequential(unsigned long size, unsigned long loop_in,
                                  unsigned long addr_in);
uint32_t DDR_Test_Checkerboard(unsigned long size, unsigned long loop_in,
                               unsigned long addr_in);
uint32_t DDR_Test_BitSpread(unsigned long size, unsigned long loop_in,
                            unsigned long addr_in);
uint32_t DDR_Test_BitFlip(unsigned long size, unsigned long loop_in,
                          unsigned long addr_in);
uint32_t DDR_Test_WalkingZeroes(unsigned long size, unsigned long loop_in,
                                unsigned long addr_in);
uint32_t DDR_Test_WalkingOnes(unsigned long size, unsigned long loop_in,
                              unsigned long addr_in);
#ifdef TEST_INFINITE_ENABLE
uint32_t DDR_Test_Infinite_write(unsigned long pattern_in,
                                 unsigned long addr_in);
uint32_t DDR_Test_Infinite_read(unsigned long pattern_in,
                                unsigned long addr_in);
#endif
#endif /* __DDR_TESTS_H */
