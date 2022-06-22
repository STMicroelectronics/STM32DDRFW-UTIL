/**
  * STM32MP157C DDR3 8Gb configuration
  * 2x DDR3L 4Gb each, 16-bit, 533MHz, Single Die Package in flyby topology.
  * Reference used NT5CC256M16DP-DI from NANYA
  *
  * DDR type / Platform	DDR3/3L
  * freq		533MHz
  * width	32
  * datasheet	0  = MT41J256M16-187 / DDR3-1066 bin G
  * DDR density	8
  * timing mode	optimized
  * Scheduling/QoS options : type = 2
  * address mapping : RBC
  * Tc > + 85C : N
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
#ifndef STM32MP1XX_DDR3_8GBIT_H
#define STM32MP1XX_DDR3_8GBIT_H

#include "stm32mp1xx-ddr3-4Gb.h"

#undef DDR_MEM_NAME
#undef DDR_MEM_SIZE
#undef DDR_MSTR
#undef DDR_ADDRMAP1
#undef DDR_ADDRMAP3
#undef DDR_ADDRMAP5
#undef DDR_ADDRMAP6
#undef DDR_DX2GCR
#undef DDR_DX3GCR

#define DDR_MEM_NAME "DDR3-1066/888 bin G 2x4Gb 533MHz v1.45"
#define DDR_MEM_SIZE 0x40000000

#define DDR_MSTR 0x00040401
#define DDR_ADDRMAP1 0x00080808
#define DDR_ADDRMAP3 0x00000000
#define DDR_ADDRMAP5 0x07070707
#define DDR_ADDRMAP6 0x0F070707
#define DDR_DX2GCR 0x0000CE81
#define DDR_DX3GCR 0x0000CE81

#endif
