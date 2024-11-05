/*
******************************************************************************
**
**  File        : LinkerScript_EL3_EL1_common.ld
**
**  Abstract    : Linker script for STM32MP2 series
**
**                Common linker script to configure memory regions :
**                - in SYSRAM, for A35 EL3 initiator (in OPEN sample case)
**                - in SYSRAM or DDR, for A35 EL3 launcher
**                  and A35_* EL1 application(s) (in CLOSED sample case)
**                - unique EL1 area = shared by both cores in SMP or single core modes
**                - split into two EL1 areas = one for each core in AMP mode
**
**  Target      : STMicroelectronics STM32 MP2
**
**  Distribution: The file is distributed "as is" without any warranty
**                of any kind.
**
*****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2022 STMicroelectronics</center></h2>
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
*****************************************************************************
*/

/* Select memory in which code has to be placed and offset of code */
/* from start of this memory by skipping : */

/* EL3 SYSRAM mapping compatible with BOOTROM loading (from storage devices) : */
/* - BOOTROM data area at offset range [0x00000;0x023FF] : size = 9K           */
BOOTROM_DATA_SIZE = 0x2400;
/* - image header (CLOSED sample case) area at offset range [0x02400;0x025FF] : size = 0.5K = 512B  */
IMAGE_HEADER_SIZE = 0x200;
/* - EL3 initiator (OPEN sample case) area at offset range [0x02400;0x025FF] : size = 0.5K = 512B  */
EL3_OPEN_INITIATOR_OFFSET = BOOTROM_DATA_SIZE;
EL3_OPEN_INITIATOR_SIZE = IMAGE_HEADER_SIZE;
/* - EL3 launcher area at offset range [0x02600;0x05FFF] : size = 14.5K        */
EL3_START_OFFSET = BOOTROM_DATA_SIZE + IMAGE_HEADER_SIZE;
EL3_LAUNCHER_SIZE = 0x3A00;
EL3_LENGTH = BOOTROM_DATA_SIZE + IMAGE_HEADER_SIZE + EL3_LAUNCHER_SIZE;

/* EL1 SYSRAM mapping for A35_0 & A35_1 running : */
/* - in SMP or single core modes, A35_* application area at offset range [0x06000;0x3FFFF] : size = 232K */
/* - in AMP (independent) mode, */
/*   . A35_0 application area at offset range [0x06000;0x25FFF] : size = 128K */
A35_0_EL1_SYSRAM_LENGTH = 128K;
/*   . A35_1 application area at offset range [0x26000;0x3FFFF] : size = 104K */
/* - in any mode, cached memory addresses can be derived from basic ones */
/*   by applying a 0x10000000 offset */
CACHED_SYSRAM_OFFSET = 0x10000000;

/* EL3 DDR mapping : keep same EL3 default length (start offset is 0x0) */

/* EL1 DDR mapping for A35_0 & A35_1 running : */
/* - in SMP mode, A35_* application area at offset range [0x06000;0x3FFFFFFF] : size = 1024M - 24K */
/* - in AMP (independent) mode : */
/*   . set A35_0 application in first half after EL3 area : size = 512M - 24K */
A35_0_EL1_DDR_LENGTH = 512M - EL3_LENGTH;
/*   . set A35_1 application in second half : size = 512M */

MEMORY
{
  /* EL3-EL1 SYSRAM mapping : */
  /* - EL3 for OPEN sample only */
  SYSRAM_EL3_INITIATOR    (rwx) : ORIGIN = 0x20000000 + EL3_OPEN_INITIATOR_OFFSET, LENGTH = EL3_OPEN_INITIATOR_SIZE
  SYSRAM_EL3_NO_INITIATOR (rwx) : ORIGIN = 0x20000000 + EL3_OPEN_INITIATOR_OFFSET, LENGTH = 256K - EL3_OPEN_INITIATOR_OFFSET

  /* - EL3 for CLOSED and OPEN samples */
  SYSRAM_EL3_LAUNCHER     (rwx) : ORIGIN = 0x0E000000 + EL3_START_OFFSET, LENGTH = 256K - EL3_START_OFFSET
  SYSRAM_EL3_NO_LAUNCHER  (rwx) : ORIGIN = 0x0E000000 + EL3_START_OFFSET, LENGTH = 256K - EL3_START_OFFSET

  SYSRAM_EL1  (rwx) : ORIGIN = 0x20000000 + EL3_LENGTH, LENGTH = 256K - EL3_START_OFFSET

  SYSRAM0_EL1 (rwx) : ORIGIN = 0x20000000 + EL3_LENGTH, LENGTH = A35_0_EL1_SYSRAM_LENGTH
  SYSRAM1_EL1 (rwx) : ORIGIN = 0x20000000 + EL3_LENGTH + A35_0_EL1_SYSRAM_LENGTH, LENGTH = 256K - EL3_LENGTH - A35_0_EL1_SYSRAM_LENGTH

  /* EL3-EL1 DDR mapping */
  DDR_EL3_LAUNCHER  (rwx) : ORIGIN = 0x80000000, LENGTH = EL3_LENGTH

  DDR_EL1  (rwx) : ORIGIN = 0x80000000 + EL3_LENGTH, LENGTH = 1024M - EL3_LENGTH

  DDR0_EL1 (rwx) : ORIGIN = 0x80000000 + EL3_LENGTH, LENGTH = A35_0_EL1_DDR_LENGTH
  DDR1_EL1 (rwx) : ORIGIN = 0x80000000 + EL3_LENGTH + A35_0_EL1_DDR_LENGTH, LENGTH = 1024M - EL3_LENGTH - A35_0_EL1_DDR_LENGTH

  SRAM1 (rwx) : ORIGIN = 0x20000000 + 0x00040000,  LENGTH = 128K
}
