/*
******************************************************************************
**
**  File        : LinkerScriptaa64_el3_launcher_sysram.ld
**
**  Abstract    : Linker script for Aarch64 EL3 launcher in SYSRAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
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

INCLUDE ../LinkerScript_EL3_EL1_common.ld

/* Entry Point */
ENTRY(_ResetA35)

/* Select Memory in which code has to be placed */
/* and offset of code from start of this memory */
REGION_ALIAS("RAM", SYSRAM_EL3_LAUNCHER);

/* Code Memory */
__MEM_START__ = ORIGIN(RAM);
__MEM_SIZE__  = LENGTH(RAM);

/* put the EL3 stack at the end of the memory */
EL3_STACK_BASE = __MEM_START__ + __MEM_SIZE__;

SECTIONS
{
  /* Code starts with the vector table, the bootcode, then the
   * remainder of the code */
  . = __MEM_START__;
  .RESET . : {
        KEEP (* (RESET))
        *vectors.o(vectors)
        *bootcode.o(boot)
        *(.text) /* remainder of code */
        }>RAM

  /* Data */
        __data_start = .;
        .data  : { *(.data)  }>RAM
        .data1 : { *(.data1) }>RAM

  /* Notes section
   * This is not used so we discard it. Although not used it needs to be
   * explicitly mentioned in the linker script as some toolchains will place
   * the notes section at address 0 if it is not explicitly mentioned. */
  /DISCARD/ : { *(.note*) }

  /* Uninitialized data */
  /* 16-byte alignment constraints for fitting needs of toolset based on GCC 5.3.1 */
        . = ALIGN(16);
        __bss_start   = .;
        __bss_start__ = .;
        .bss   : { *(.bss)   }>RAM
        . = ALIGN(16);
        __bss_end   = .;
        __bss_end__ = .;
        . = ALIGN(4);
        __end   = .;
        __end__ = .;
        PROVIDE (end = .);
}

