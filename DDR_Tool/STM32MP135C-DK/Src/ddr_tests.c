/**
  ******************************************************************************
  * @file    ddr_tests.c
  * @author  MCD Application Team
  * @brief   This file provides all the HAL DDR test functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2013-2023 STMicroelectronics.
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
#define DDR_BASE_ADDR                        0xC0000000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int get_addr(uint32_t addr_in, uint32_t *addr)
{
  if (addr_in != 0)
  {
    if (addr_in < DDR_BASE_ADDR)
    {
      printf("Address too low: 0x%lx\n\r", addr_in);
      return -1;
    }

    if ((addr_in & 0x3) != 0)
    {
      printf("Unaligned address: 0x%lx\n\r", addr_in);
      return -1;
    }

    *addr = addr_in;
  }
  else
  {
    *addr = DDR_BASE_ADDR;
  }

  return 0;
}

static void get_nb_loop(uint32_t loop_in, uint32_t *nb_loop,
                        uint32_t default_nb_loop)
{
  if (loop_in != 0)
  {
    if (loop_in == 0xFFFFFFFF)
    {
      printf("Warning: infinite loop requested\n\r");
    }

    *nb_loop = loop_in;
  }
  else
  {
    *nb_loop = default_nb_loop;
  }
}

static int get_buf_size(uint32_t size_in, uint32_t *size,
                        uint32_t default_size, uint32_t min_size)
{
  if (size_in != 0)
  {
    if ((size_in < min_size) || (size_in > DDR_MEM_SIZE))
    {
      printf("Invalid size: 0x%lx\n\r", size_in);
      printf("  (range = 0x%lx..0x%x)\n\r", min_size, DDR_MEM_SIZE);
      return -1;
    }

    if ((size_in & (min_size - 1)) != 0)
    {
      printf("Unaligned size: 0x%lx (min=%ld)\n\r", size_in, min_size);
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

static bool is_power_of_2(uint32_t n)
{
  return ((n != 0) && ((n & (n - 1)) == 0));
}

static void get_pattern(uint32_t pattern_in, uint32_t *pattern,
                        uint32_t default_pattern)
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
uint32_t DDR_Test_Databus(uint32_t addr_in)
{
  uint32_t pattern;
  uint32_t addr;

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  for (pattern = 1U; pattern != 0U; pattern <<= 1)
  {
    WRITE_REG(*(volatile uint32_t*)addr, pattern);

    if (READ_REG(*(volatile uint32_t*)addr) != pattern)
    {
      printf("  test_databus KO @ 0x%lx \n\r", addr);
      return 2;
    }
  }

  return 0;
}

static int test_databuswalk(uint8_t mode, uint32_t loop_in, uint32_t addr_in)
{
  int i;
  uint32_t addr;
  uint32_t data;
  uint32_t error = 0U;
  uint32_t loop = 0;
  uint32_t nb_loop;

  get_nb_loop(loop_in, &nb_loop, 100);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  while (error == 0U)
  {
    for (i = 0; i < 32; i++)
    {
      uint32_t pattern = mode ? (1 << i) : ~(1 << i);

      WRITE_REG(*(volatile uint32_t*)(addr + sizeof(uint32_t) * i), pattern);
    }

    for (i = 0; i < 32; i++)
    {
      uint32_t pattern = mode ? (1 << i) : ~(1 << i);

      data = READ_REG(*(volatile uint32_t*)(addr + sizeof(uint32_t) * i));
      if (pattern !=  data)
      {
        error |= 1 << i;
        printf("  %lx: error %lx expected %lx => error:%lx\n\r",
                addr + sizeof(uint32_t) * i, data, pattern, error);
      }
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }

    for (i = 0; i < 32; i++)
    {
      WRITE_REG(*(volatile uint32_t*)(addr + sizeof(uint32_t) * i), 0);
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
uint32_t DDR_Test_DatabusWalk0(uint32_t loop, uint32_t addr)
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
uint32_t DDR_Test_DatabusWalk1(uint32_t loop, uint32_t addr)
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
uint32_t DDR_Test_AddressBus(uint32_t size_in, uint32_t addr_in)
{
  uint32_t addr;
  uint32_t size;
  uint64_t addressmask;
  uint64_t offset;
  uint64_t testoffset = 0;
  uint32_t pattern     = 0xAAAAAAAA;
  uint32_t antipattern = 0x55555555;

  if (get_buf_size(size_in, &size, DDR_MEM_SIZE, 4) != 0)
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
  for (offset = sizeof(uint32_t); (offset & addressmask) != 0U; offset <<= 1)
  {
    WRITE_REG(*(volatile uint32_t*)(addr + (uint32_t)offset), pattern);
  }

  /* Check for address bits stuck high. */
  WRITE_REG(*(volatile uint32_t*)(addr + (uint32_t)testoffset), antipattern);

  for (offset = sizeof(uint32_t);
       ((offset & addressmask) != 0U) && (offset != DDR_MEM_SIZE);
       offset <<= 1)
  {
    if (READ_REG(*(volatile uint32_t*)(addr + (uint32_t)offset)) != pattern)
    {
      printf("  test_addrbus KO @ 0x%lx \n\r", addr + (uint32_t)offset);
      return 4;
    }
  }

  WRITE_REG(*(volatile uint32_t*)(addr + (uint32_t)testoffset), pattern);

  /* Check for address bits stuck low or shorted. */
  for (testoffset = sizeof(uint32_t);
       ((testoffset & addressmask) != 0U)  && (testoffset != DDR_MEM_SIZE);
       testoffset <<= 1)
  {
    WRITE_REG(*(volatile uint32_t*)(addr + (uint32_t)testoffset), antipattern);

    if (READ_REG(*(volatile uint32_t*)addr) != pattern)
    {
      printf("  test_addrbus KO @ 0x%lx \n\r", addr + (uint32_t)testoffset);
      return 5;
    }

    for (offset = sizeof(uint32_t);
         ((offset & addressmask) != 0U) && (offset != DDR_MEM_SIZE);
         offset <<= 1)
    {
      if ((READ_REG(*(volatile uint32_t*)(addr + (uint32_t)offset)) != pattern)
          && (offset != testoffset))
      {
        printf("  test_addrbus KO @ 0x%lx \n\r", addr + (uint32_t)offset);
        return 6;
      }
    }

    WRITE_REG(*(volatile uint32_t*)(addr + (uint32_t)testoffset), pattern);
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
uint32_t DDR_Test_MemDevice(uint32_t size_in, uint32_t addr_in)
{
  uint32_t addr;
  uint32_t size;
  uint64_t nb_words;
  uint32_t offset;
  uint32_t pattern;
  uint32_t antipattern;

  if (get_buf_size(size_in, &size, 4 * 1024, 4) != 0)
  {
    return 1;
  }

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  nb_words = size / sizeof(uint32_t);

  log_dbg("Fill with pattern\n\r");
  /* Fill memory with a known pattern. */
  for (pattern = 1, offset = 0; offset < nb_words;
       pattern++, offset += sizeof(uint32_t))
  {
    WRITE_REG(*(volatile uint32_t*)(addr + offset), pattern);
  }

  log_dbg("Check and invert pattern\n\r");
  /* Check each location and invert it for the second pass. */
  for (pattern = 1, offset = 0; offset < nb_words;
       pattern++, offset += sizeof(uint32_t))
  {
    if (READ_REG(*(volatile uint32_t*)(addr + offset)) != pattern)
    {
      printf("  test_memdevice KO @ 0x%lx \n\r", addr + offset);
      return 3;
    }

    antipattern = ~pattern;
    WRITE_REG(*(volatile uint32_t*)(addr + offset), antipattern);
  }

  log_dbg("Check inverted pattern\n\r");
  /* Check each location for the inverted pattern and zero it. */
  for (pattern = 1, offset = 0; offset < nb_words;
       pattern++, offset += sizeof(uint32_t))
  {
    antipattern = ~pattern;
    if (READ_REG(*(volatile uint32_t*)(addr + offset)) != antipattern)
    {
      printf("  test_memdevice KO @ 0x%lx \n\r", addr + offset);
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
uint32_t DDR_Test_SimultaneousSwitchingOutput(uint32_t size_in,
                                                  uint32_t addr_in)
{
  int i, j;
  uint32_t addr;
  uint32_t size;
  uint32_t remaining;
  uint32_t offset;
  uint32_t data = 0;

  if (get_buf_size(size_in, &size, 4 * 1024, 4) != 0)
  {
    return 1;
  }

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  log_dbg("running sso at 0x%lx length 0x%lx\n\r", addr, size);
  offset = addr;
  remaining = size;

  while (remaining) {
    for (i = 0; i < 32; i++) {
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

        WRITE_REG(*(volatile uint32_t*)offset, data);

        if (READ_REG(*(volatile uint32_t*)offset) != data)
        {
          printf("  test_sso KO @ 0x%lx \n\r", offset);
          return 3;
        }
      }
    }
    offset += sizeof(uint32_t);
    remaining -= sizeof(uint32_t);
  }

  return 0;
}

static void do_noise(uint32_t addr, uint32_t pattern, uint32_t *result)
{
  __asm volatile ("PUSH {r2-r10}         \n"
                  "MOV r0, %[addr]       \n"
                  "MOV r1, %[pattern]    \n"
                  "MOV r11, %[result]    \n"

                  "MVN r2, r1            \n"

                  "STR r1, [r0]          \n"
                  "LDR r3, [r0]          \n"
                  "STR r2, [r0]          \n"
                  "LDR r4, [r0]          \n"

                  "STR r1, [r0]          \n"
                  "LDR r5, [r0]          \n"
                  "STR r2, [r0]          \n"
                  "LDR r6, [r0]          \n"

                  "STR r1, [r0]          \n"
                  "LDR r7, [r0]          \n"
                  "STR r2, [r0]          \n"
                  "LDR r8, [r0]          \n"

                  "STR r1, [r0]          \n"
                  "LDR r9, [r0]          \n"
                  "STR r2, [r0]          \n"
                  "LDR r10, [r0]         \n"

                  "STMIA r11!, {r3-r10}  \n"

                  "POP {r2-r10}          \n"
                  ::[addr]    "r" (addr),
                    [pattern] "r" (pattern),
                    [result]  "r" (result)
                  : "r0", "r1", "r11");
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
uint32_t DDR_Test_Noise(uint32_t pattern_in, uint32_t addr_in)
{
  uint32_t pattern;
  uint32_t addr;
  uint32_t result[8] = {0,0,0,0,0,0,0,0};
  int i;

  get_pattern(pattern_in, &pattern, 0xFFFFFFFF);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  do_noise(addr, pattern, result);

  for (i = 0; i < 8;)
  {
    if (READ_REG(*(volatile uint32_t*)(&result[i++])) != pattern)
    {
      printf("  test_noise KO @ 0x%lx \n\r", result[i - 1]);
      return 2;
    }

    if (READ_REG(*(volatile uint32_t*)(&result[i++])) != ~pattern)
    {
      printf("  test_noise KO @ 0x%lx \n\r", result[i - 1]);
      return 3;
    }
  }

  return 0;
}

static void do_noiseburst(uint32_t addr, uint32_t pattern, size_t bufsize)
{
  __asm volatile ("PUSH {r2-r8}          \n"
                  "MOV r0, %[addr]       \n"
                  "MOV r1, %[pattern]    \n"
                  "MOV r9, %[bufsize]    \n"

                  "MVN r2, r1            \n"
                  "MOV r3, r1            \n"
                  "MOV r4, r2            \n"
                  "MOV r5, r1            \n"
                  "MOV r6, r2            \n"
                  "MOV r7, r1            \n"
                  "MOV r8, r2            \n"

                  "loop1:                \n"
                  "STMIA r0!, {r1-r8}    \n"
                  "STMIA r0!, {r1-r8}    \n"
                  "STMIA r0!, {r1-r8}    \n"
                  "STMIA r0!, {r1-r8}    \n"
                  "SUBS r9, r9, #128     \n"
                  "BGE loop1             \n"
                  "POP {r2-r8}           \n"
                  ::[addr]    "r" (addr),
                    [pattern] "r" (pattern),
                    [bufsize] "r" (bufsize)
                  : "r0", "r1", "r9");
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
uint32_t DDR_Test_NoiseBurst(uint32_t size_in, uint32_t pattern_in,
                                 uint32_t addr_in)
{
  uint32_t pattern;
  uint32_t addr;
  uint32_t offset;
  uint32_t bufsize;
  size_t remaining;
  size_t size;
  int i;

  if (get_buf_size(size_in, &bufsize, 4 * 1024, 128) != 0)
  {
    return 1;
  }

  get_pattern(pattern_in, &pattern, 0xFFFFFFFF);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  offset = addr;
  remaining = (size_t)bufsize;
  size = DDR_CHUNK_SIZE;

  while (remaining)
  {
    if (remaining < size)
    {
      size = remaining;
    }

    do_noiseburst(offset, pattern, size);
    remaining -= size;
    offset += size;
  }

  for (i = 0; i < bufsize;)
  {
    if (READ_REG(*(volatile uint32_t*)(addr + i)) != pattern)
    {
      printf("  test_noiseburst KO @ 0x%lx\n\r", addr + i);
      return 3;
    }

    i += sizeof(uint32_t);

    if (READ_REG(*(volatile uint32_t*)(addr + i)) != ~pattern)
    {
      printf("  test_noiseburst KO @ 0x%lx\n\r", addr + i);
      return 4;
    }

    i += sizeof(uint32_t);
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
uint32_t DDR_Test_Random(uint32_t size_in, uint32_t loop_in,
                                            uint32_t addr_in)
{
  uint32_t value = 0;
  uint32_t data;
  uint32_t offset;
  uint32_t addr;
  uint32_t error = 0U;
  uint32_t loop = 0;
  uint32_t nb_loop;
  uint32_t bufsize;
  unsigned int seed;

  if (get_buf_size(size_in, &bufsize, 4 * 1024, 8) != 0)
  {
    return 1;
  }

  get_nb_loop(loop_in, &nb_loop, 10);

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  bufsize /= 2;

  while (error == 0U)
  {
    seed = rand();
    srand(seed);

    for (offset = 0; offset < bufsize; offset += sizeof(uint32_t))
    {
      WRITE_REG(*(volatile uint32_t*)(addr + offset), rand());
    }

    memcpy((void *)(addr + bufsize), (void *)addr, bufsize);

    srand(seed);

    for (offset = 0; offset < (2 * bufsize); offset += sizeof(uint32_t))
    {
      if (offset == bufsize)
      {
        srand(seed);
      }

      value = rand();
      data = READ_REG(*(volatile uint32_t*)(addr + offset));
      if (data != value)
      {
        error++;
        printf("  loop %ld: error @ 0x%lx: 0x%lx expected 0x%lx\n\r",
               loop, offset, data, value);
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
static int test_loop(const uint32_t *pattern, uint32_t *address,
                     const uint32_t bufsize)
{
  int i;
  int j;
  uint32_t offset;
  uint32_t testsize;
  uint32_t remaining;

  offset = (uint32_t)address;
  remaining = bufsize;

  while (remaining)
  {
    testsize = bufsize > 0x1000000 ? 0x1000000 : bufsize;

    __asm volatile ("PUSH {r3-r10}          \n"
                    "MOV r0, %[pattern]     \n"
                    "MOV r1, %[offset]      \n"
                    "MOV r2, %[testsize]    \n"

                    "LDMIA r0!, {r3-r10}\n"

                    "loop2:\n"
                    "STMIA r1!, {r3-r10}\n"
                    "STMIA r1!, {r3-r10}\n"
                    "STMIA r1!, {r3-r10}\n"
                    "STMIA r1!, {r3-r10}\n"
                    "SUBS r2, r2, #128\n"
                    "BGE loop2\n"
                    "POP {r3-r10}\n"
                    ::[pattern]  "r" (pattern),
                      [offset]   "r" (offset),
                      [testsize] "r" (testsize)
                    : "r0", "r1", "r2");

    offset += testsize;
    remaining -= testsize;
  }

  for (i = 0; i < bufsize; i += DDR_PATTERN_SIZE * sizeof(uint32_t))
  {
    for (j = 0; j < DDR_PATTERN_SIZE; j++, address++)
    {
      if (READ_REG(*(volatile uint32_t*)address) != pattern[j])
      {
        printf("  test_freqpattern KO @ 0x%lx\n\r", *address);
        return 1;
      }
    }
  }

  return 0;
}

const uint32_t pattern_div1_x16[DDR_PATTERN_SIZE] = {
  0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF,
  0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF
};

const uint32_t pattern_div2_x16[DDR_PATTERN_SIZE] = {
  0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
  0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000
};

const uint32_t pattern_div4_x16[DDR_PATTERN_SIZE] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
  0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000
};

const uint32_t pattern_div4_x32[DDR_PATTERN_SIZE] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0x00000000, 0x00000000, 0x00000000, 0x00000000
};

const uint32_t pattern_mostly_zero_x16[DDR_PATTERN_SIZE] = {
  0x00000000, 0x00000000, 0x00000000, 0x0000FFFF,
  0x00000000, 0x00000000, 0x00000000, 0x00000000
};

const uint32_t pattern_mostly_zero_x32[DDR_PATTERN_SIZE] = {
  0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF,
  0x00000000, 0x00000000, 0x00000000, 0x00000000
};

const uint32_t pattern_mostly_one_x16[DDR_PATTERN_SIZE] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000FFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

const uint32_t pattern_mostly_one_x32[DDR_PATTERN_SIZE] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

#define DDR_NB_PATTERN                      5
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_HALF    DDRCTRL_MSTR_DATA_BUS_WIDTH_0
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_QUARTER DDRCTRL_MSTR_DATA_BUS_WIDTH_1

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
uint32_t DDR_Test_FrequencySelectivePattern(uint32_t size,
                                                uint32_t addr_in)
{
  const uint32_t * const patterns_x16[DDR_NB_PATTERN] = {
    pattern_div1_x16,
    pattern_div2_x16,
    pattern_div4_x16,
    pattern_mostly_zero_x16,
    pattern_mostly_one_x16,
  };
  const uint32_t * const patterns_x32[DDR_NB_PATTERN] = {
    pattern_div2_x16,
    pattern_div4_x16,
    pattern_div4_x32,
    pattern_mostly_zero_x32,
    pattern_mostly_one_x32
  };

  int i;
  int bus_width;
  int ret = 0;
  const uint32_t **patterns;
  uint32_t bufsize;
  uint32_t addr;

  if (get_buf_size(size, &bufsize, 4 * 1024, 128) != 0)
  {
    return 1;
  }

  if (get_addr(addr_in, &addr) != 0)
  {
    return 2;
  }

  switch (READ_REG(DDRCTRL->MSTR) & DDRCTRL_MSTR_DATA_BUS_WIDTH_Msk)
  {
    case DDRCTRL_MSTR_DATA_BUS_WIDTH_HALF:
    case DDRCTRL_MSTR_DATA_BUS_WIDTH_QUARTER:
      bus_width = 16;
      break;
    default:
      bus_width = 32;
      break;
  }

  patterns = (const uint32_t **)(bus_width == 16 ? patterns_x16 : patterns_x32);

  for (i = 0; i < DDR_NB_PATTERN; i++)
  {
    ret = test_loop(patterns[i], (uint32_t *)addr, bufsize);
    if (ret != 0)
    {
      printf("  test_freqpattern KO\n\r");
      return 3;
    }
  }

  return 0;
}

/* pattern test with size, loop for write pattern */
static int test_loop_size(const uint32_t *pattern, uint32_t size,
                          uint32_t *address, const uint32_t bufsize)
{
  int i, j;
  uint32_t *p = address;

  for (i = 0; i < bufsize; i += (size * sizeof(uint32_t)))
  {
    for (j = 0; j < size ; j++, p++)
    {
      *p = pattern[j];
    }
  }

  p = address;
  for (i = 0; i < bufsize; i += size * sizeof(uint32_t))
  {
    for (j = 0; j < size; j++, p++)
    {
      if (READ_REG(*(volatile uint32_t*)p) != pattern[j])
      {
        printf("  test KO @ 0x%ln\n\r", p);
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
uint32_t DDR_Test_BlockSequential(uint32_t size, uint32_t loop_in,
                                      uint32_t addr_in)
{
  uint32_t bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uint32_t addr;
  uint32_t value;
  int i;
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
      value = i | i << 8 | i << 16 | i << 24;
      ret = test_loop_size(&value, 1, (uint32_t *)addr, bufsize);
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
uint32_t DDR_Test_Checkerboard(uint32_t size, uint32_t loop_in,
                                   uint32_t addr_in)
{
  uint32_t bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uint32_t addr;
  uint32_t checkboard[2] = {0x55555555, 0xAAAAAAAA};
  int i;
  int ret;

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
      ret = test_loop_size(checkboard, 2, (uint32_t *)addr, bufsize);
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
uint32_t DDR_Test_BitSpread(uint32_t size, uint32_t loop_in,
                                uint32_t addr_in)
{
  uint32_t bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uint32_t addr;
  uint32_t bitspread[4];
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
    for (i = 1; i < 32; i++)
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

        ret = test_loop_size(bitspread, 4, (uint32_t *)addr, bufsize);
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
uint32_t DDR_Test_BitFlip(uint32_t size, uint32_t loop_in,
                              uint32_t addr_in)
{
  uint32_t bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uint32_t addr;
  uint32_t bitflip[4];
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
    for (i = 0; i < 32; i++)
    {
      bitflip[0] = 1 << i;
      bitflip[1] = bitflip[0];
      bitflip[2] = ~bitflip[0];
      bitflip[3] = bitflip[2];

      ret = test_loop_size(bitflip, 4, (uint32_t *)addr, bufsize);
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
uint32_t DDR_Test_WalkingZeroes(uint32_t size, uint32_t loop_in,
                                    uint32_t addr_in)
{
  uint32_t bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uint32_t addr;
  uint32_t value;
  int i;
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
    for (i = 0; i < 64; i++)
    {
      if (i < 32)
      {
        value = 1 << i;
      }
      else
      {
        value = 1 << (63 - i);
      }

      ret = test_loop_size(&value, 1, (uint32_t *)addr, bufsize);
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
uint32_t DDR_Test_WalkingOnes(uint32_t size, uint32_t loop_in,
                                  uint32_t addr_in)
{
  uint32_t bufsize;
  uint32_t nb_loop;
  uint32_t loop = 0;
  uint32_t addr;
  uint32_t value;
  int i;
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
    for (i = 0; i < 64; i++)
    {
      if (i < 32)
      {
        value = ~(1 << i);
      }
      else
      {
        value = ~(1 << (63 - i));
      }

      ret = test_loop_size(&value, 1, (uint32_t *)addr, bufsize);
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
uint32_t DDR_Test_Infinite_write(uint32_t pattern_in, uint32_t addr_in)
{
  uint32_t addr;
  uint32_t data;
  uint32_t loop = 0;
  uint32_t nb_loop = 0xFFFFFFFFU;
  int i, size = 1024 * 1024;
  bool random = false;
  volatile uint32_t go_loop = 1U;

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  get_pattern(pattern_in, &data, 0xA5A5AA55);

  if (addr == 0xC8888888)
  {
    printf("running random\n\r");
    random = true;
  }
  else
  {
    printf("running at 0x%lx with pattern 0x%lx\n\r", addr, data);
  }

  while (go_loop != 0U)
  {
    for (i = 0; i < size; i++)
    {
      if (random)
      {
        addr = (uint32_t)((rand() & (DDR_MEM_SIZE - 1) & ~0x3));
        data = rand();
      }

      WRITE_REG(*(volatile uint32_t*)addr, data);
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
uint32_t DDR_Test_Infinite_read(uint32_t pattern_in, uint32_t addr_in)
{
  uint32_t addr;
  uint32_t data;
  uint32_t loop = 0;
  uint32_t nb_loop = 0xFFFFFFFFU;
  int i, size = 1024 * 1024;
  bool random = false;
  volatile uint32_t go_loop = 1U;

  if (get_addr(addr_in, &addr) != 0)
  {
    return 1;
  }

  get_pattern(pattern_in, &data, 0xA5A5AA55);

  if (addr == 0xC8888888)
  {
    printf("running random\n\r");
    random = true;
  }
  else
  {
    printf("running at 0x%lx with pattern 0x%lx\n\r", addr, data);
    WRITE_REG(*(volatile uint32_t*)addr, data);
  }

  while (go_loop != 0U)
  {
    for (i = 0; i < size; i++)
    {
      if (random)
        addr = (uint32_t)((rand() & (DDR_MEM_SIZE - 1) & ~0x3));

      READ_REG(*(volatile uint32_t*)addr);
      printf("data @ address 0x%lx = 0x%lx \n\r", addr, data);
    }

    if (test_loop_end(&loop, nb_loop))
    {
      break;
    }
  }

  return 0;
}
#endif
