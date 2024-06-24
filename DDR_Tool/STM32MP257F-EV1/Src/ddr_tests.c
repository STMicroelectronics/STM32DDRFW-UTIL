/**
  ******************************************************************************
  * @file    ddr_tests.c
  * @author  MCD Application Team
  * @brief   This file provides all the HAL DDR test functions.
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
#include "stm32_device_hal.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "log.h"
#include "ddr_tests.h"

#include "stm32mp_util_conf.h"
#include "stm32mp_util_ddr_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DDR_BASE_ADDR                        0x80000000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int get_addr(unsigned long addr_in, uintptr_t **addr)
{
  if (addr_in != 0UL)
  {
    if (addr_in < DDR_MEM_BASE)
    {
      printf("Address too low: 0x%lx\n\r", addr_in);
      return -1;
    }

    if ((addr_in & 0x3UL) != 0UL)
    {
      printf("Unaligned address: 0x%lx\n\r", addr_in);
      return -1;
    }

    *addr = (uintptr_t *)addr_in;
  }
  else
  {
    *addr = (uintptr_t *)DDR_MEM_BASE;
  }

  return 0;
}

static void get_nb_loop(unsigned long loop_in, uint32_t *nb_loop,
                        uint32_t default_nb_loop)
{
  if (loop_in != 0)
  {
    if (loop_in == 0xFFFFFFFF)
    {
      printf("Warning: infinite loop requested\n\r");
    }

    if (loop_in > 0xFFFFFFFF)
    {
      printf("Warning: incorrect loop_number, forced to default value\n\r");
      *nb_loop = default_nb_loop;
    }
    else
    {
      *nb_loop = (uint32_t)loop_in;
    }
  }
  else
  {
    *nb_loop = default_nb_loop;
  }
}

static int get_buf_size(unsigned long size_in, unsigned long *size,
                        unsigned long default_size, unsigned long min_size)
{
  if (size_in != 0)
  {
    if ((size_in < min_size) || (size_in > (unsigned long)DDR_MEM_SIZE))
    {
      printf("Invalid size: 0x%lx\n\r", size_in);
      printf("  (range = 0x%lx..0x%lx)\n\r", min_size, (unsigned long)DDR_MEM_SIZE);
      return -1;
    }

    if ((size_in & (min_size - 1)) != 0)
    {
      printf("Unaligned size: 0x%lx (min=0x%lx)\n\r", size_in, min_size);
      return -1;
    }

    *size = size_in;
  }
  else
  {
    *size = default_size;
  }

  return 0;
}

static bool is_power_of_2(unsigned long n)
{
  return ((n != 0) && ((n & (n - 1)) == 0));
}

static void get_pattern(unsigned long pattern_in, unsigned long *pattern,
		unsigned long default_pattern)
{
  if (pattern_in != 0)
  {
    *pattern = pattern_in;
  }
  else
  {
    *pattern = default_pattern;
  }
}

static int test_loop_end(uint32_t *loop, uint32_t nb_loop)
{
  (*loop)++;
  if (nb_loop && *loop >= nb_loop)
  {
    return 1;
  }

  return 0;
}

/**
* @brief test_databus.
* @par Test Description
*   Verifies each data line by walking 1 on fixed address.
*   Test the data bus wiring in a memory region by performing
*   a walking 1's test at a fixed address within that region.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - None
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_Databus(unsigned long addr_in)
{
  unsigned long pattern;
  uintptr_t *addr = NULL;

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  for (pattern = 1U; pattern != 0U; pattern <<= 1)
  {
    *addr = pattern;

    if (*addr != pattern)
    {
      printf("  test_databus KO @ 0x%lx \n\r", (unsigned long)addr);
      return 2;
    }
  }

  return 0;
}

static int test_databuswalk(uint8_t mode, unsigned long loop_in, unsigned long addr_in)
{
  int i;
  uintptr_t *addr = NULL;
  unsigned long data;
  unsigned long error = 0U;
  uint32_t loop = 0;
  uint32_t nb_loop;

  get_nb_loop(loop_in, &nb_loop, 100);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  while (error == 0U)
  {
    for (i = 0; i < (int)(sizeof(unsigned long) * 8); i++)
    {
      unsigned long pattern = mode ? (1 << i) : ~(1 << i);

      *(addr + sizeof(unsigned long) * i) = pattern;
    }

    for (i = 0; i < (int)(sizeof(unsigned long) * 8); i++)
    {
      unsigned long pattern = mode ? (1 << i) : ~(1 << i);

      data = *(addr + sizeof(unsigned long) * i);
      if (pattern !=  data)
      {
        error |= 1 << i;
        printf("  0x%lx: error 0x%lx expected 0x%lx => error:0x%lx\n\r",
               (unsigned long)(addr + sizeof(unsigned long) * i), data, pattern, error);
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }

    for (i = 0; i < (int)(sizeof(unsigned long) * 8); i++)
    {
      *(addr + sizeof(unsigned long) * i) = 0;
    }
  }

  if (error != 0U)
  {
    printf("  test_databuswalk%d KO\n\r", mode);
    return 2;
  }

  return 0;
}

/**
* @brief test_databuswalk0.
* @par Test Description
*   Verifies each data bus signal can be driven low (32 word burst).
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_DatabusWalk0(unsigned long loop, unsigned long addr)
{
  return test_databuswalk(0, loop, addr);
}

/**
* @brief test_databuswalk1.
* @par Test Description
*   Verifies each data bus signal can be driven high (32 word burst).
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_DatabusWalk1(unsigned long loop, unsigned long addr)
{
  return test_databuswalk(1, loop, addr);
}

/**
* @brief test_addrbus.
* @par Test Description
*   Verifies each relevant bits of the address and checking for aliasing.
*   Test the address bus wiring in a memory region by performing
*   a walking 1's test on the relevant bits of the address and
*   checking for aliasing. This test will find single-bit address
*   failures such as stuck -high, stuck-low, and shorted pins.
*   For best results, the selected base address should have enough
*   LSB 0's to guarantee single address bit changes.  For example,
*   to test a 64-Kbyte region, select a base address on a 64-Kbyte
*   boundary.  Also, select the region size as a power-of-two
*   --if at all possible.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_AddressBus(unsigned long size_in, unsigned long addr_in)
{
  uintptr_t *addr = NULL;
  unsigned long size;
  unsigned long dflt_size;
  unsigned long addressmask;
  unsigned long offset;
  unsigned long testoffset = 0;
  unsigned long pattern     = 0xAAAAAAAAAAAAAAAA;
  unsigned long antipattern = 0x5555555555555555;
  unsigned long data;

  if (!is_power_of_2(DDR_MEM_SIZE))
  {
    /* Get the bigger 2^n value of DDR_MEM_SIZE */
    if ((DDR_MEM_SIZE & 0x100000000) == 0x100000000)
    {
      dflt_size = 0x100000000;
    }
    else if ((DDR_MEM_SIZE & 0x80000000) == 0x80000000)
    {
      dflt_size = 0x80000000;
    }
    else if ((DDR_MEM_SIZE & 0x40000000) == 0x40000000)
    {
      dflt_size = 0x40000000;
    }
    else if ((DDR_MEM_SIZE & 0x20000000) == 0x20000000)
    {
      dflt_size = 0x20000000;
    }
    else if ((DDR_MEM_SIZE & 0x10000000) == 0x10000000)
    {
      dflt_size = 0x10000000;
    }
    else
    {
      printf("DDR size too low for this test (0x%lx)\n\r",
             (unsigned long)DDR_MEM_SIZE);

      return 2;
    }
  }
  else
  {
    dflt_size = DDR_MEM_SIZE;
  }

  if (get_buf_size(size_in, &size, dflt_size, 4) != 0)
  {
    return 1;
  }

  if (!is_power_of_2(size))
  {
    printf("size 0x%lx is not a power of 2\n\r", size);
    return 2;
  }

  if (get_addr(addr_in, &addr) != 0)
  {
    return 3;
  }

  addressmask = size - 1;

  /* Write the default pattern at each of the power-of-two offsets. */
  for (offset = 1U;
       ((offset & addressmask) != 0U) &&
       ((offset * sizeof(unsigned long)) != dflt_size);
       offset <<= 1)
  {
    *(addr + offset) = pattern;
  }

  /* Check for address bits stuck high. */
  *(addr + testoffset) = antipattern;

  for (offset = 1U;
       ((offset & addressmask) != 0U) &&
       ((offset * sizeof(unsigned long)) != dflt_size);
       offset <<= 1)
  {
    data = *(addr + offset);
    if (data != pattern)
    {
      printf("  test_addrbus KO @ 0x%lx \n\r", (unsigned long)(addr + offset));
      printf("  read 0x%lx instead of 0x%lx\n\r", data, pattern);
      return 4;
    }
  }

  *(addr + testoffset) = pattern;

  /* Check for address bits stuck low or shorted. */
  for (testoffset = 1U;
       ((testoffset & addressmask) != 0U) &&
       ((testoffset * sizeof(unsigned long)) != dflt_size);
       testoffset <<= 1)
  {
    *(addr + testoffset) = antipattern;

    data = *addr;
    if (data != pattern)
    {
      printf("  test_addrbus KO @ 0x%lx \n\r", (unsigned long)(addr + testoffset));
      printf("  read 0x%lx instead of 0x%lx\n\r", data, pattern);
      return 5;
    }

    for (offset = 1U;
         ((offset & addressmask) != 0U) &&
         ((offset * sizeof(unsigned long)) != dflt_size);
         offset <<= 1)
    {
     data = *(addr + offset);
     if ((data != pattern) && (offset != testoffset))
      {
        printf("  test_addrbus KO @ 0x%lx \n\r", (unsigned long)(addr + offset));
        printf("  read 0x%lx instead of 0x%lx\n\r", data, pattern);
        return 6;
      }
    }

    *(addr + testoffset) = pattern;
  }

  return 0;
}

/**
* @brief test_memdevice.
* @par Test Description
*   Test the integrity of a physical memory device (test every storage
*   bit in the region) by performing an increment/decrement test over
*   the entire region. In the process every storage bit in the device
*   is tested as a zero and a one.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_MemDevice(unsigned long size_in, unsigned long addr_in)
{
  uintptr_t *addr = NULL;
  unsigned long size;
  unsigned long nb_words;
  unsigned long offset;
  unsigned long pattern;
  unsigned long antipattern;

  if (get_buf_size(size_in, &size, 4 * 1024, 4) != 0)
  {
    return 1;
  }

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  nb_words = size / sizeof(unsigned long);

  /* Fill memory with a known pattern. */
  for (pattern = 1, offset = 0; offset < nb_words;
       pattern++, offset += sizeof(unsigned long))
  {
    *(addr + offset) = pattern;
  }

  /* Check each location and invert it for the second pass. */
  for (pattern = 1, offset = 0; offset < nb_words;
       pattern++, offset += sizeof(unsigned long))
  {
    if (*(addr + offset) != pattern)
    {
      printf("  test_memdevice KO @ 0x%lx \n\r", (unsigned long)(addr + offset));
      return 3;
    }

    antipattern = ~pattern;
    *(addr + offset) = antipattern;
  }

  /* Check each location for the inverted pattern and zero it. */
  for (pattern = 1, offset = 0; offset < nb_words;
       pattern++, offset += sizeof(unsigned long))
  {
    antipattern = ~pattern;
    if (*(addr + offset) != antipattern)
    {
      printf("  test_memdevice KO @ 0x%lx \n\r", (unsigned long)(addr + offset));
      return 4;
    }
  }

  return 0;
}

/**
* @brief test_sso.
* @par Test Description
*   Test the Simultaneous Switching Output.
*   Verifies succes sive reads and writes to the same memory word,
*   holding one bit constant while toggling all other data bits
*   simultaneously
*   => stress the data bus over an address range
*   The CPU writes to each address in the given range.
*   For each bit, first the CPU holds the bit at 1 while toggling the
*   other bits, and then the CPU holds the bit at 0 while toggling
*   the other bits.
*   After each write, the CPU reads the address that was written
*   to verify that it contains the correct data.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_SimultaneousSwitchingOutput(unsigned long size_in,
                                              unsigned long addr_in)
{
  int i, j;
  uintptr_t *addr = NULL;
  unsigned long size;
  unsigned long remaining;
  unsigned long offset;
  unsigned long data = 0;

  if (get_buf_size(size_in, &size, 4 * 1024, 4) != 0)
  {
    return 1;
  }

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  //printf("running sso at 0x%lx length 0x%lx\n\r", (uint32_t)addr, size);
  offset = 0;
  remaining = size;

  while (remaining) {
    for (i = 0; i < (int)(sizeof(unsigned long) * 8); i++) {
      /* write pattern. */
      for (j = 0; j < 6; j++) {
        switch (j)
        {
          case 0:
          case 2:
            data = 1 << i;
            break;
          case 3:
          case 5:
            data = ~(1 << i);
            break;
          case 1:
            data = ~0x0;
            break;
          case 4:
            data = 0x0;
            break;
        }

        *(addr + offset) = data;

        if (*(addr + offset) != data)
        {
          printf("  test_sso KO @ 0x%lx \n\r", (unsigned long)(addr + offset));
          return 3;
        }
      }
    }
    offset ++;
    remaining -= sizeof(unsigned long);
  }

  return 0;
}

static void do_noise(unsigned long addr, unsigned long pattern,
                     unsigned long *result)
{
  __asm volatile (
//                "PUSH {x2-x10}            \n"
                  "STP x2, x3, [sp, #-16]!  \n"
                  "STP x4, x5, [sp, #-16]!  \n"
                  "STP x6, x7, [sp, #-16]!  \n"
                  "STP x8, x9, [sp, #-16]!  \n"
                  "STP x10, x11, [sp, #-16]!\n"

                  "MOV x0, %[addr]          \n"
                  "MOV x1, %[pattern]       \n"
                  "MOV x12, %[result]       \n"

                  "MVN x2, x1               \n"

                  "STR x1, [x0]             \n"
                  "LDR x3, [x0]             \n"
                  "STR x2, [x0]             \n"
                  "LDR x4, [x0]             \n"

                  "STR x1, [x0]             \n"
                  "LDR x5, [x0]             \n"
                  "STR x2, [x0]             \n"
                  "LDR x6, [x0]             \n"

                  "STR x1, [x0]             \n"
                  "LDR x7, [x0]             \n"
                  "STR x2, [x0]             \n"
                  "LDR x8, [x0]             \n"

                  "STR x1, [x0]             \n"
                  "LDR x9, [x0]             \n"
                  "STR x2, [x0]             \n"
                  "LDR x10, [x0]            \n"

//                "STMIA x12!, {x3-x10}     \n"
                  "STP x3, x4, [x12], #16   \n"
                  "STP x5, x6, [x12], #16   \n"
                  "STP x7, x8, [x12], #16   \n"
                  "STP x9, x10, [x12], #16  \n"

//                "POP {x2-x10}             \n"
                  "LDP x2, x3, [sp], #16    \n"
                  "LDP x4, x5, [sp], #16    \n"
                  "LDP x6, x7, [sp], #16    \n"
                  "LDP x8, x9, [sp], #16    \n"
                  "LDP x10, x11, [sp], #16  \n"
                  ::[addr]    "r" (addr),
                    [pattern] "r" (pattern),
                    [result]  "r" (result)
                  : "x0", "x1", "x12");
}


/**
* @brief test_noise.
* @par Test Description
*   Verifies r/w while forcing switching of all data bus lines.
*   optimised 4 iteration write/read/write/read cycles...
*   for pattern and inversed pattern.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_Noise(unsigned long pattern_in, unsigned long addr_in)
{
  unsigned long pattern;
  unsigned long dflt_pattern = 0xFFFFFFFFFFFFFFFF;
  uintptr_t *addr = NULL;
  unsigned long result[8] = {0,0,0,0,0,0,0,0};
  int i;

  get_pattern(pattern_in, &pattern, dflt_pattern);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  do_noise((unsigned long)addr, pattern, result);

  for (i = 0; i < 8;)
  {
    if (*(&result[i++]) != pattern)
    {
      printf("  test_noise KO @ 0x%lx \n\r", result[i - 1]);
      return 2;
    }

    if (*(&result[i++]) != ~pattern)
    {
      printf("  test_noise KO @ 0x%lx \n\r", result[i - 1]);
      return 3;
    }
  }

  return 0;
}

static void do_noiseburst(unsigned long addr, unsigned long pattern,
                          unsigned long bufsize)
{
  __asm volatile (
//                "PUSH {x2-x8}             \n"
                  "STP x2, x3, [sp, #-16]!  \n"
                  "STP x4, x5, [sp, #-16]!  \n"
                  "STP x6, x7, [sp, #-16]!  \n"
                  "STP x8, x9, [sp, #-16]!  \n"

                  "MOV x0, %[addr]          \n"
                  "MOV x1, %[pattern]       \n"
                  "MOV x10, %[bufsize]      \n"

                  "MVN x2, x1               \n"
                  "MOV x3, x1               \n"
                  "MOV x4, x2               \n"
                  "MOV x5, x1               \n"
                  "MOV x6, x2               \n"
                  "MOV x7, x1               \n"
                  "MOV x8, x2               \n"

                  "loop1:                   \n"

//                "STMIA x0!, {x1-x8}       \n"
                  "STP x1, x2, [x0], #16    \n"
                  "STP x3, x4, [x0], #16    \n"
                  "STP x5, x6, [x0], #16    \n"
                  "STP x7, x8, [x0], #16    \n"

//                "STMIA x0!, {x1-x8}       \n"
                  "STP x1, x2, [x0], #16    \n"
                  "STP x3, x4, [x0], #16    \n"
                  "STP x5, x6, [x0], #16    \n"
                  "STP x7, x8, [x0], #16    \n"

//                "STMIA x0!, {x1-x8}       \n"
                  "STP x1, x2, [x0], #16    \n"
                  "STP x3, x4, [x0], #16    \n"
                  "STP x5, x6, [x0], #16    \n"
                  "STP x7, x8, [x0], #16    \n"

//                "STMIA x0!, {x1-x8}       \n"
                  "STP x1, x2, [x0], #16    \n"
                  "STP x3, x4, [x0], #16    \n"
                  "STP x5, x6, [x0], #16    \n"
                  "STP x7, x8, [x0], #16    \n"

                  "SUBS x10, x10, #256      \n"
                  "BGE loop1                \n"

//                "POP {x2-x8}              \n"
                  "LDP x2, x3, [sp], #16    \n"
                  "LDP x4, x5, [sp], #16    \n"
                  "LDP x6, x7, [sp], #16    \n"
                  "LDP x8, x9, [sp], #16    \n"
                  ::[addr]    "r" (addr),
                    [pattern] "r" (pattern),
                    [bufsize] "r" (bufsize)
                  : "x0", "x1", "x10");
}

#define DDR_CHUNK_SIZE  0x08000000

/**
* @brief test_noiseburst.
* @par Test Description
*   Verifies r/w while forcing switching of all data bus lines.
*   optimised write loop witrh store multiple to use burst
*   for pattern and inversed pattern.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_NoiseBurst(unsigned long size_in, unsigned long pattern_in,
                             unsigned long addr_in)
{
  unsigned long pattern;
  uintptr_t *addr = NULL;
  unsigned long offset;
  unsigned long bufsize;
  unsigned long data;
  unsigned long remaining;
  unsigned long size;
  unsigned long i;

  if (get_buf_size(size_in, &bufsize, 4 * 1024, 128) != 0)
  {
    return 1;
  }

  get_pattern(pattern_in, &pattern, 0xFFFFFFFF);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  offset = 0;
  remaining = bufsize;
  size = DDR_CHUNK_SIZE;

  while (remaining)
  {
    if (remaining < size)
    {
      size = remaining;
    }

    do_noiseburst((unsigned long)(addr + offset), pattern, size);
    remaining -= size;
    offset += size;
  }

  for (i = 0; i < bufsize / sizeof(unsigned long);)
  {
    data = *(addr + i);
    if (data != pattern)
    {
      printf("  test_noiseburst KO @ 0x%lx\n\r", (unsigned long)(addr + i));
      printf("  read 0x%lx instead of 0x%lx\n\r", data, pattern);
      return 3;
    }

    i++;

    data = *(addr + i);
    if (data != ~pattern)
    {
      printf("  test_noiseburst KO @ 0x%lx\n\r", (unsigned long)(addr + i));
      printf("  read 0x%lx instead of 0x%lx\n\r", data, pattern);
      return 4;
    }

    i++;
  }

  return 0;
}

/**
* @brief test_random.
* @par Test Description
*   Verifies r/w with pseudo-ramdom value on one region
*   + write the region (individual access)
*   + memcopy to the 2nd region (try to use burst)
*   + verify the 2 regions
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_Random(unsigned long size_in, unsigned long loop_in,
                         unsigned long addr_in)
{
  unsigned long value = 0;
  unsigned long data;
  unsigned long offset;
  uintptr_t *addr = NULL;
  unsigned long error = 0U;
  uint32_t loop = 0;
  uint32_t nb_loop;
  unsigned long bufsize_bytes;
  unsigned long bufsize_words;
  unsigned int seed;

  if (get_buf_size(size_in, &bufsize_bytes, 4 * 1024, 8) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 10);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  bufsize_bytes /= 2;
  bufsize_words = bufsize_bytes/sizeof(unsigned long);

  while (error == 0U)
  {
    seed = rand();
    srand(seed);

    for (offset = 0; offset < bufsize_words; offset ++)
    {
      data = rand();
      *(addr + offset) = data;
    }

    memcpy((void *)((unsigned long)addr + bufsize_bytes), addr, bufsize_bytes);

    srand(seed);

    for (offset = 0; offset < (2 * bufsize_words); offset ++)
    {
      if (offset == bufsize_words)
      {
        srand(seed);
      }

      value = rand();
      data = *(addr + offset);
      if (data != value)
      {
        error++;
        printf("  loop %d: error @ 0x%lx: 0x%lx expected 0x%lx\n\r",
               loop, (unsigned long)(addr + offset), data, value);
        break;
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  if (error != 0U)
  {
    printf("  test_random KO\n\r");
    return 3;
  }

  return 0;
}

#define DDR_PATTERN_SIZE  8

/* pattern test, optimized loop for read/write pattern (array of 8 u32) */
static void test_loop_in(const unsigned long *pattern, unsigned long offset,
                         unsigned long testsize)
{
  __asm volatile (
//                "PUSH {x3-x10}            \n"
                  "STP x3, x4, [sp, #-16]!  \n"
                  "STP x5, x6, [sp, #-16]!  \n"
                  "STP x7, x8, [sp, #-16]!  \n"
                  "STP x9, x10, [sp, #-16]! \n"

                  "MOV x0, %[pattern]       \n"
                  "MOV x1, %[offset]        \n"
                  "MOV x2, %[testsize]      \n"

//                "LDMIA x0!, {x3-x10}      \n"
                  "LDP x3, x4, [x0], #16    \n"
                  "LDP x5, x6, [x0], #16    \n"
                  "LDP x7, x8, [x0], #16    \n"
                  "LDP x9, x10, [x0], #16   \n"

                  "loop2:                   \n"

//                "STMIA x1!, {x3-x10}      \n"
                  "STP x3, x4, [x1], #16    \n"
                  "STP x5, x6, [x1], #16    \n"
                  "STP x7, x8, [x1], #16    \n"
                  "STP x9, x10, [x1], #16   \n"

//                "STMIA x1!, {x3-x10}      \n"
                  "STP x3, x4, [x1], #16    \n"
                  "STP x5, x6, [x1], #16    \n"
                  "STP x7, x8, [x1], #16    \n"
                  "STP x9, x10, [x1], #16   \n"

//                "STMIA x1!, {x3-x10}      \n"
                  "STP x3, x4, [x1], #16    \n"
                  "STP x5, x6, [x1], #16    \n"
                  "STP x7, x8, [x1], #16    \n"
                  "STP x9, x10, [x1], #16   \n"

//                "STMIA x1!, {x3-x10}      \n"
                  "STP x3, x4, [x1], #16    \n"
                  "STP x5, x6, [x1], #16    \n"
                  "STP x7, x8, [x1], #16    \n"
                  "STP x9, x10, [x1], #16   \n"

                  "SUBS x2, x2, #256        \n"
                  "BGE loop2                \n"

//                "POP {x3-x10}             \n"
                  "LDP x3, x4, [sp], #16    \n"
                  "LDP x5, x6, [sp], #16    \n"
                  "LDP x7, x8, [sp], #16    \n"
                  "LDP x9, x10, [sp], #16   \n"
                    ::[pattern]  "r" (pattern),
                      [offset]   "r" (offset),
                      [testsize] "r" (testsize)
                    : "x0", "x1", "x2");
}

static int test_loop(const unsigned long *pattern, uintptr_t *address,
                     const unsigned long bufsize)
{
  unsigned long i;
  int j;
  unsigned long offset;
  unsigned long testsize;
  unsigned long remaining;

  offset = (unsigned long)address;
  remaining = bufsize;

  while (remaining)
  {
    testsize = bufsize > 0x1000000 ? 0x1000000 : bufsize;

    test_loop_in(pattern, offset, testsize);

    offset += testsize;
    remaining -= testsize;
  }

  offset = 0;

  for (i = 0; i < bufsize / sizeof(unsigned long); i += DDR_PATTERN_SIZE)
  {
    for (j = 0; j < DDR_PATTERN_SIZE; j++, offset++)
    {
      if (*(address + offset) != pattern[j])
      {
        printf("  test_freqpattern KO @ 0x%lx\n\r", (unsigned long)(address + offset));
        return 1;
      }
    }
  }

  return 0;
}

const unsigned long pattern_div1_x16[DDR_PATTERN_SIZE] = {
  0x0000FFFF0000FFFF, 0x0000FFFF0000FFFF, 0x0000FFFF0000FFFF, 0x0000FFFF0000FFFF,
  0x0000FFFF0000FFFF, 0x0000FFFF0000FFFF, 0x0000FFFF0000FFFF, 0x0000FFFF0000FFFF
};

const unsigned long pattern_div2_x16[DDR_PATTERN_SIZE] = {
  0xFFFFFFFFFFFFFFFF, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000
};

const unsigned long pattern_div4_x16[DDR_PATTERN_SIZE] = {
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000, 0x0000000000000000
};

const unsigned long pattern_div4_x32[DDR_PATTERN_SIZE] = {
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000
};

const unsigned long pattern_mostly_zero_x16[DDR_PATTERN_SIZE] = {
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000FFFF0000FFFF,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000
};

const unsigned long pattern_mostly_zero_x32[DDR_PATTERN_SIZE] = {
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000
};

const unsigned long pattern_mostly_one_x16[DDR_PATTERN_SIZE] = {
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x0000FFFF0000FFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF
};

const unsigned long pattern_mostly_one_x32[DDR_PATTERN_SIZE] = {
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF
};

#define DDR_NB_PATTERN                      5
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_HALF    DDRC_MSTR_DATA_BUS_WIDTH_0
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_QUARTER DDRC_MSTR_DATA_BUS_WIDTH_1

/**
* @brief test_freqpattern.
* @par Test Description
*   Frequency Selective Pattern Test.
*   Stress data bus by performing successive write 8-word burst
*   operations using mostly zero/one patterns and frequency divider
*   patterns (F/1, F/2, F/4) for 16 and 32 data bus width.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_FrequencySelectivePattern(unsigned long size,
                                            unsigned long addr_in)
{
  const unsigned long * const patterns_x16[DDR_NB_PATTERN] = {
    pattern_div1_x16,
    pattern_div2_x16,
    pattern_div4_x16,
    pattern_mostly_zero_x16,
    pattern_mostly_one_x16,
  };
  const unsigned long * const patterns_x32[DDR_NB_PATTERN] = {
    pattern_div2_x16,
    pattern_div4_x16,
    pattern_div4_x32,
    pattern_mostly_zero_x32,
    pattern_mostly_one_x32
  };

  int i;
  int bus_width;
  int ret = 0;
  const unsigned long **patterns;
  unsigned long bufsize;
  uintptr_t *addr = NULL;

  if (get_buf_size(size, &bufsize, 4 * 1024, 128) != 0)
  {
    return 1;
  }

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  switch (READ_REG(DDRC->MSTR) & DDRC_MSTR_DATA_BUS_WIDTH_Msk)
  {
    case DDRCTRL_MSTR_DATA_BUS_WIDTH_HALF:
    case DDRCTRL_MSTR_DATA_BUS_WIDTH_QUARTER:
      bus_width = 16;
      break;
    default:
      bus_width = 32;
      break;
  }

  patterns = (const unsigned long **)(bus_width == 16 ? patterns_x16 : patterns_x32);

  for (i = 0; i < DDR_NB_PATTERN; i++)
  {
    ret = test_loop(patterns[i], addr, bufsize);
    if (ret != 0)
    {
      printf("  test_freqpattern KO\n\r");
      return 3;
    }
  }

  return 0;
}

/* pattern test with size, loop for write pattern */
static int test_loop_size(const unsigned long *pattern, unsigned long size,
                          uintptr_t *address, const unsigned long bufsize,
                          __attribute__((unused))uint32_t loop_nb,
                          __attribute__((unused))uint32_t loop)
{
  unsigned long i, j;
  uintptr_t *addr = (uintptr_t *)address;

  for (i = 0; i < bufsize / sizeof(unsigned long); i += size)
  {
    for (j = 0; j < size ; j++, addr++)
    {
      *addr = pattern[j];
    }

  }

  addr = address;
  for (i = 0; i < bufsize / sizeof(unsigned long); i += size)
  {
    for (j = 0; j < size; j++, addr++)
    {
      if (*addr != pattern[j])
      {
        printf("  test KO @ 0x%lx\n\r", (unsigned long)addr);
        return 1;
      }
    }
  }

  return 0;
}

/**
* @brief test_blockseq.
* @par Test Description
*   Block Sequential Test.
*   Test incremental pattern.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_BlockSequential(unsigned long size, unsigned long loop_in,
                                  unsigned long addr_in)
{
  unsigned long bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uintptr_t *addr = NULL;
  unsigned long value;
  unsigned long i;
  int ret;

  if (get_buf_size(size, &bufsize, 4 * 1024, 4) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 1);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  while (1)
  {
    for (i = 0; i < 256; i++)
    {
      value = i | i << 8 | i << 16 | i << 24 | i << 32 | i << 40 | i << 48 | i << 56;
      ret = test_loop_size(&value, 1, addr, bufsize, 256, i);
      if (ret != 0)
      {
        printf("  test_blockseq KO\n\r");
        return 3;
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}

/**
* @brief test_checkboard.
* @par Test Description
*   Checker board test.
*   Test checker pattern.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_Checkerboard(unsigned long size, unsigned long loop_in,
                               unsigned long addr_in)
{
  unsigned long bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uintptr_t *addr = NULL;
  unsigned long checkboard[2];
  int i;
  int ret;

  checkboard[0] = 0x5555555555555555;
  checkboard[1] = 0xAAAAAAAAAAAAAAAA;

  if (get_buf_size(size, &bufsize, 4 * 1024, 8) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 1);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  while (1)
  {
    for (i = 0; i < 2; i++)
    {
      ret = test_loop_size(checkboard, 2, addr, bufsize, 2, i);
      if (ret != 0)
      {
        printf("  test_checkboard KO\n\r");
        return 3;
      }

      checkboard[0] = ~checkboard[0];
      checkboard[1] = ~checkboard[1];
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}

/**
* @brief test_bitspread.
* @par Test Description
*   Test Bit Spread pattern.
*   Try to catch bad bits which are dependent on the current values of
*   surrounding bits in either the same word32.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_BitSpread(unsigned long size, unsigned long loop_in,
                            unsigned long addr_in)
{
  unsigned long bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uintptr_t *addr = NULL;
  unsigned long bitspread[4];
  int i;
  int j;
  int ret;

  if (get_buf_size(size, &bufsize, 4 * 1024, 32) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 1);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  while (1)
  {
    for (i = 1; i < (int)(sizeof(unsigned long) * 8); i++)
    {
      for (j = 0; j < i; j++)
      {
        if (i < 32)
        {
          bitspread[0] = (1 << i) | (1 << j);
        }
        else
        {
          bitspread[0] = (1 << (63 - i)) | (1 << (63 - j));
        }

        bitspread[1] = bitspread[0];
        bitspread[2] = ~bitspread[0];
        bitspread[3] = ~bitspread[0];

        ret = test_loop_size(bitspread, 4, addr, bufsize, 32, i);
        if (ret != 0)
        {
          printf("  test_bitspread KO\n\r");
          return 3;
        }
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}

/**
* @brief test_bitflip.
* @par Test Description
*   Test Bit Flip pattern.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_BitFlip(unsigned long size, unsigned long loop_in,
                          unsigned long addr_in)
{
  unsigned long bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uintptr_t *addr = NULL;
  unsigned long bitflip[4];
  int i;
  int ret;

  if (get_buf_size(size, &bufsize, 4 * 1024, 32) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 1);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  while (1)
  {
    for (i = 0; i < (int)(sizeof(unsigned long) * 8); i++)
    {
      bitflip[0] = 1 << i;
      bitflip[1] = bitflip[0];
      bitflip[2] = ~bitflip[0];
      bitflip[3] = bitflip[2];

      ret = test_loop_size(bitflip, 4, addr, bufsize, 32, i);
      if (ret != 0)
      {
        printf("  test_bitflip KO\n\r");
        return 3;
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}

/**
* @brief test_walkbit0.
* @par Test Description
*   Test Walking Zeroes pattern
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_WalkingZeroes(unsigned long size, unsigned long loop_in,
                                unsigned long addr_in)
{
  unsigned long bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uintptr_t *addr = NULL;
  unsigned long value;
  int i;
  int ret;
  int depth;

  if (get_buf_size(size, &bufsize, 4 * 1024, 4) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 1);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  depth = sizeof(unsigned long) * 8;

  while (1)
  {
    for (i = 0; i < depth * 2 ; i++)
    {
      if (i < depth)
      {
        value = 1 << i;
      }
      else
      {
        value = 1 << ((depth * 2) - 1 - i);
      }

      ret = test_loop_size(&value, 1, addr, bufsize, (depth * 2) -1, i);
      if (ret != 0)
      {
        printf("  test_walkbit0 KO\n\r");
        return 3;
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}

/**
* @brief test_walkbit1.
* @par Test Description
*   Test Walking Ones pattern.
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_WalkingOnes(unsigned long size, unsigned long loop_in,
                              unsigned long addr_in)
{
  unsigned long bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uintptr_t *addr = NULL;
  unsigned long value;
  int i;
  int ret;
  int depth;

  if (get_buf_size(size, &bufsize, 4 * 1024, 4) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 1);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  depth = sizeof(unsigned long) * 8;

  while (1)
  {
    for (i = 0; i < depth * 2; i++)
    {
      if (i < depth)
      {
        value = ~(1 << i);
      }
      else
      {
        value = ~(1 << ((depth * 2) - 1 - i));
      }

      ret = test_loop_size(&value, 1, addr, bufsize, (depth * 2) - 1, i);
      if (ret != 0)
      {
        printf("  test_walkbit1 KO\n\r");
        return 3;
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}

#ifdef TEST_INFINITE_ENABLE
/**
* @brief test infinite write access to DDR
* @par Test Description
*   Continuous write the same pattern at the same address
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_Infinite_write(unsigned long pattern_in, unsigned long addr_in)
{
  uintptr_t *addr = NULL;
  unsigned long data;
  uint32_t loop = 0;
  uint32_t nb_loop = 0xFFFFFFFFU;
  int i, size = 1024 * 1024;
  bool random = false;
  volatile uint32_t go_loop = 1U;
  unsigned long dflt_pattern = 0xA5A5AA55AAAA5555;

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  get_pattern(pattern_in, &data, dflt_pattern);

  if ((unsigned long)addr == 0xC8888888)
  {
    printf("running random\n\r");
    random = true;
  }
  else
  {
    printf("running at 0x%lx with pattern 0x%lx\n\r", (unsigned long)addr, data);
  }

  while (go_loop != 0U)
  {
    for (i = 0; i < size; i++)
    {
      if (random)
      {
        addr = (uintptr_t *)(unsigned long)((rand() & (DDR_MEM_SIZE - 1) & ~0x3));
        data = rand();
      }

      *addr = data;
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}

/**
* @brief test infinite read access to DDR
* @par Test Description
*   Continuous read the same pattern at the same address
* @par Test Hardware Connection
* - None
* @par Required preconditions
* - None
* @par Expected result
* - None
* @par Called functions
* - xxx
* @par Used Peripherals
* - None,...
* @retval
*  0: Test passed
*  Value different from 0: Test failed
*  None(0xFF): if the result is deduced by the user: waveform, event...
*/
uint32_t DDR_Test_Infinite_read(unsigned long pattern_in, unsigned long addr_in)
{
  uintptr_t *addr = NULL;
  unsigned long data;
  uint32_t loop = 0;
  uint32_t nb_loop = 0xFFFFFFFFU;
  int i, size = 1024 * 1024;
  bool random = false;
  volatile uint32_t go_loop = 1U;
  unsigned long dflt_pattern = 0xA5A5AA55AAAA5555;

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  get_pattern(pattern_in, &data, dflt_pattern);

  if ((unsigned long)addr == 0xC8888888)
  {
    printf("running random\n\r");
    random = true;
  }
  else
  {
    printf("running at 0x%lx with pattern 0x%lx\n\r", (unsigned long)addr, data);
    *addr = data;
  }

  while (go_loop != 0U)
  {
    for (i = 0; i < size; i++)
    {
      if (random)
        addr = (uintptr_t *)(unsigned long)((rand() & (DDR_MEM_SIZE - 1) & ~0x3));

      data = *addr;
      printf("data @ address 0x%lx = 0x%lx \n\r", (unsigned long)addr, data);
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}
#endif
