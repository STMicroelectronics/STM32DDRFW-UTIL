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
uint32_t DDR_Test_Databus(uint32_t addr_in);
uint32_t DDR_Test_DatabusWalk0(uint32_t loop, uint32_t addr);
uint32_t DDR_Test_DatabusWalk1(uint32_t loop, uint32_t addr);
uint32_t DDR_Test_AddressBus(uint32_t size, uint32_t addr_in);
uint32_t DDR_Test_MemDevice(uint32_t size, uint32_t addr_in);
uint32_t DDR_Test_SimultaneousSwitchingOutput(uint32_t size,
                                                  uint32_t addr_in);
uint32_t DDR_Test_Noise(uint32_t pattern_in, uint32_t addr_in);
uint32_t DDR_Test_NoiseBurst(uint32_t size_in, uint32_t pattern_in,
                                 uint32_t addr_in);
uint32_t DDR_Test_Random(uint32_t size, uint32_t loop_in,
                             uint32_t addr_in);
uint32_t DDR_Test_FrequencySelectivePattern(uint32_t size,
                                                uint32_t addr_in);
uint32_t DDR_Test_BlockSequential(uint32_t size, uint32_t loop_in,
                                      uint32_t addr_in);
uint32_t DDR_Test_Checkerboard(uint32_t size, uint32_t loop_in,
                                   uint32_t addr_in);
uint32_t DDR_Test_BitSpread(uint32_t size, uint32_t loop_in,
                                uint32_t addr_in);
uint32_t DDR_Test_BitFlip(uint32_t size, uint32_t loop_in,
                              uint32_t addr_in);
uint32_t DDR_Test_WalkingZeroes(uint32_t size, uint32_t loop_in,
                                    uint32_t addr_in);
uint32_t DDR_Test_WalkingOnes(uint32_t size, uint32_t loop_in,
                                  uint32_t addr_in);
#ifdef TEST_INFINITE_ENABLE
uint32_t DDR_Test_Infinite_write(uint32_t pattern_in,
                                  uint32_t addr_in);
uint32_t DDR_Test_Infinite_read(uint32_t pattern_in,
                                  uint32_t addr_in);
#endif
#endif /* __DDR_TESTS_H */
