/**
  * STM32MP135C LPDDR2 4Gb configuration
  * LPDDR2 4Gb, 16-bit, 533MHz.
  * Reference used NT6TL256M16BA-G0 from NANYA
  *
  * DDR type / Platform	LPDDR2
  * freq		533MHz
  * width	16
  * datasheet	0
  * DDR density	4
  * timing mode	optimized
  * Scheduling/QoS options : type = 6
  * address mapping : RBC
  * Tc > + 85C : N
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef STM32MP13XX_LPDDR2_4GBIT_H
#define STM32MP13XX_LPDDR2_4GBIT_H

#define DDR_MEM_NAME "LPDDR2 4Gb 533MHz v1.53"
#define DDR_MEM_SPEED 533000
#define DDR_MEM_SIZE 0x20000000

#define DDR_MSTR 0x00040004
#define DDR_MRCTRL0 0x00000010
#define DDR_MRCTRL1 0x00000000
#define DDR_DERATEEN 0x00000001
#define DDR_DERATEINT 0x00800000
#define DDR_PWRCTL 0x00000000
#define DDR_PWRTMG 0x00400010
#define DDR_HWLPCTL 0x00000000
#define DDR_RFSHCTL0 0x00210000
#define DDR_RFSHCTL3 0x00000000
#define DDR_RFSHTMG 0x00400046
#define DDR_CRCPARCTL0 0x00000000
#define DDR_DRAMTMG0 0x111B1217
#define DDR_DRAMTMG1 0x00040623
#define DDR_DRAMTMG2 0x04080C0F
#define DDR_DRAMTMG3 0x0050500C
#define DDR_DRAMTMG4 0x0A02060C
#define DDR_DRAMTMG5 0x05050808
#define DDR_DRAMTMG6 0x02020006
#define DDR_DRAMTMG7 0x00000202
#define DDR_DRAMTMG8 0x00004405
#define DDR_DRAMTMG14 0x0000004B
#define DDR_ZQCTL0 0xC2000040
#define DDR_DFITMG0 0x02060104
#define DDR_DFITMG1 0x00000202
#define DDR_DFILPCFG0 0x07000000
#define DDR_DFIUPD0 0xC0400003
#define DDR_DFIUPD1 0x00000000
#define DDR_DFIUPD2 0x00000000
#define DDR_DFIPHYMSTR 0x00000001
#define DDR_ADDRMAP1 0x00090909
#define DDR_ADDRMAP2 0x00000000
#define DDR_ADDRMAP3 0x00000000
#define DDR_ADDRMAP4 0x00001F00
#define DDR_ADDRMAP5 0x08080808
#define DDR_ADDRMAP6 0x0F0F0808
#define DDR_ADDRMAP9 0x00000000
#define DDR_ADDRMAP10 0x00000000
#define DDR_ADDRMAP11 0x00000000
#define DDR_ODTCFG 0x04000400
#define DDR_ODTMAP 0x00000000
#define DDR_SCHED 0x00000F01
#define DDR_SCHED1 0x00000000
#define DDR_PERFHPR1 0x00000001
#define DDR_PERFLPR1 0x04000200
#define DDR_PERFWR1 0x08000400
#define DDR_DBG0 0x00000000
#define DDR_DBG1 0x00000000
#define DDR_DBGCMD 0x00000000
#define DDR_POISONCFG 0x00000000
#define DDR_PCCFG 0x00000010
#define DDR_PCFGR_0 0x00000000
#define DDR_PCFGW_0 0x00000000
#define DDR_PCFGQOS0_0 0x00100009
#define DDR_PCFGQOS1_0 0x00000020
#define DDR_PCFGWQOS0_0 0x01100B03
#define DDR_PCFGWQOS1_0 0x01000200
#define DDR_PGCR 0x01442E02
#define DDR_PTR0 0x0022AA5B
#define DDR_PTR1 0x01B1A068
#define DDR_PTR2 0x042A16E7
#define DDR_ACIOCR 0x38400812
#define DDR_DXCCR 0x00000910
#define DDR_DSGCR 0x9100025F
#define DDR_DCR 0x0000000C
#define DDR_DTPR0 0x46D7ABD8
#define DDR_DTPR1 0x114600D8
#define DDR_DTPR2 0x0004104B
#define DDR_MR0 0x00000000
#define DDR_MR1 0x000000C3
#define DDR_MR2 0x00000006
#define DDR_MR3 0x00000003
#define DDR_ODTCR 0x00000000
#define DDR_ZQ0CR1 0x00000018
#define DDR_DX0GCR 0x0000C881
#define DDR_DX1GCR 0x0000C881

#endif
