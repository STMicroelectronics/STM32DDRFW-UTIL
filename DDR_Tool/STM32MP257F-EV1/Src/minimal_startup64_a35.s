/******************************************************************************
 * @file    minimal_startup64_a35.s
 * @author  Christophe ICORD (Intropack Team)
 * @date    May 2022
 * @brief   CMSIS Cortex-A35 Device Peripheral Access Layer
 *          Minimal start-up source file for A35 launched in Aarch64 mode at EL3
 *
 * LANGUAGE:  ASM ARMv8 Aarch64
 *
 * REFERENCE DOCUMENTS :
 * . [A35_TRM]
 *   "Arm Cortex-A35 Processor"
 *   "Technical Reference Manual / 100236_0100_00_en"
 *   Revision: r1p0 / Issue 0100-00 / 28-Feb-2019
 *   (file cortex_a35_trm_100236_0100_00_en.pdf)
 *
 * . [ARM_DAI_0527A] Application Note
 *   "Bare-metal Boot Code for ARMv8-a Processors"
 *   ARM DAI 0527A -- Version 1.0 -- 31/03/2017
 *   (file DAI0527A_baremetal_boot_code_for_ARMv8_A_processors.pdf)
 *
 * . [ARMv8_RM DDI0487B.a]
 *   "ARM Architecture Reference Manual - ARMv8, for ARMv8-A architecture profile"
 *   (file DDI0487B_a_armv8_arm.pdf)
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
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
**/
        /* EC: Exception Class Definition */
        .equ ESR_ELx_EC_UNKNOWN,     0x00   /* Unknown/Uncategorized */
        .equ ESR_ELx_EC_WFx,         0x01   /* WFI/WFE */
        .equ ESR_ELx_EC_CP15_32,     0x03   /* CP15 MCR/MRC */
        .equ ESR_ELx_EC_CP15_64,     0x04   /* CP15 MCRR/MRRC */
        .equ ESR_ELx_EC_CP14_MR,     0x05   /* CP14 MCR/MRC */
        .equ ESR_ELx_EC_CP14_LS,     0x06   /* CP14 LDC/STC */
        .equ ESR_ELx_EC_FP_ASIMD,    0x07   /* ASIMD */
        .equ ESR_ELx_EC_CP10_ID,     0x08   /* CP10 MRC/VMRS */
        .equ ESR_ELx_EC_CP14_64,     0x0C   /* CP14 MCRR/MRRC */
        .equ ESR_ELx_EC_ILL,         0x0E   /* PSTATE.IL */
        .equ ESR_ELx_EC_SVC32,       0x11   /* SVC (AArch32) */
        .equ ESR_ELx_EC_HVC32,       0x12   /* HVC (AArch32) */
        .equ ESR_ELx_EC_SMC32,       0x13   /* SMC (AArch32) */
        .equ ESR_ELx_EC_SVC64,       0x15   /* SVC (AArch64) */
        .equ ESR_ELx_EC_HVC64,       0x16   /* HVC (AArch64) */
        .equ ESR_ELx_EC_SMC64,       0x17   /* SMC (AArch64) */
        .equ ESR_ELx_EC_SYS64,       0x18   /* MSR/MRS (AArch64) */
        .equ ESR_ELx_EC_IMP_DEF,     0x1f   /* EL3 IMP DEF */
        .equ ESR_ELx_EC_IABT_LOW,    0x20   /* IABT (lower EL) */
        .equ ESR_ELx_EC_IABT_CUR,    0x21   /* IABT (current EL) */
        .equ ESR_ELx_EC_PC_ALIGN,    0x22   /* PC Alignment */
        .equ ESR_ELx_EC_DABT_LOW,    0x24   /* DABT (lower EL) */
        .equ ESR_ELx_EC_DABT_CUR,    0x25   /* DABT (current EL) */
        .equ ESR_ELx_EC_SP_ALIGN,    0x26   /* SP Alignment */
        .equ ESR_ELx_EC_FP_EXC32,    0x28   /* FP (AArch32) */
        .equ ESR_ELx_EC_FP_EXC64,    0x2C   /* FP (AArch64) */
        .equ ESR_ELx_EC_SERROR,      0x2F   /* SError */
        .equ ESR_ELx_EC_BREAKPT_LOW, 0x30   /* Breakpoint (lower EL) */
        .equ ESR_ELx_EC_BREAKPT_CUR, 0x31   /* Breakpoint (current EL) */
        .equ ESR_ELx_EC_SOFTSTP_LOW, 0x32   /* Software Step (lower EL) */
        .equ ESR_ELx_EC_SOFTSTP_CUR, 0x33   /* Software Step (current EL) */
        .equ ESR_ELx_EC_WATCHPT_LOW, 0x34   /* Watchpoint (lower EL) */
        .equ ESR_ELx_EC_WATCHPT_CUR, 0x35   /* Watchpoint (current EL) */
        .equ ESR_ELx_EC_BKPT32,      0x38   /* BKPT (AArch32) */
        .equ ESR_ELx_EC_VECTOR32,    0x3A   /* Vector catch (AArch32) */
        .equ ESR_ELx_EC_BRK64,       0x3C   /* BRK (AArch64) */

/*============================================================================*/
/* Vector table                                                               */
/*============================================================================*/

        // Weakly-import test-defined handlers.  The weak import means that if
        // a test does not define the handler then the default one will be
        // used.
        .weak el3_curr_el_sp0_sync_vector
        .weak el3_curr_el_sp0_irq_vector
        .weak el3_curr_el_sp0_fiq_vector
        .weak el3_curr_el_sp0_serror_vector
        .weak el3_curr_el_spx_sync_vector
        .weak el3_curr_el_spx_irq_vector
        .weak el3_curr_el_spx_fiq_vector
        .weak el3_curr_el_spx_serror_vector
        .weak el3_lower_el_aarch64_sync_vector
        .weak el3_lower_el_aarch64_irq_vector
        .weak el3_lower_el_aarch64_fiq_vector
        .weak el3_lower_el_aarch64_serror_vector
        .weak el3_lower_el_aarch32_sync_vector
        .weak el3_lower_el_aarch32_irq_vector
        .weak el3_lower_el_aarch32_fiq_vector
        .weak el3_lower_el_aarch32_serror_vector

        // Start of vectors
        .balign 0x800

        // Current EL with SP0
_el3_vector_table:
        .global _el3_vector_table
el3_curr_el_sp0_sync:
        // Branch to the weakly-imported test-defined handler.  If the test
        // does not define a handler then this instruction behaves like a NOP.
        b       el3_curr_el_sp0_sync_vector

        // Default code when a test does not define a replacement handler
        b       el3_curr_el_sp0_sync

        // Ensure next vector is aligned to the correct boundary
        .balign 0x80
el3_curr_el_sp0_irq:
        b       el3_curr_el_sp0_irq_vector
        b       el3_curr_el_sp0_irq

        .balign 0x80
el3_curr_el_sp0_fiq:
        b       el3_curr_el_sp0_fiq_vector
        b       el3_curr_el_sp0_fiq

        .balign 0x80
el3_curr_el_sp0_serror:
        b       el3_curr_el_sp0_serror_vector
        b       el3_curr_el_sp0_serror

        .balign 0x80
        // Current EL with SPx
el3_curr_el_spx_sync:
        // Simple exception handler (nested interrupts not allowed) :
        // see [ARM 100933_0100_en]
        // - stack all corruptible registers (x0-x15 & x29-x30)
        stp     x0,  x1,  [sp, #-16]!
        stp     x2,  x3,  [sp, #-16]!
        stp     x4,  x5,  [sp, #-16]!
        stp     x6,  x7,  [sp, #-16]!
        stp     x8,  x9,  [sp, #-16]!
        stp     x10, x11, [sp, #-16]!
        stp     x12, x13, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        stp     x29, x30, [sp, #-16]!
        // - call C IRQ handler
        bl      el3_curr_el_spx_sync_vector
        // - restore all corruptible registers (x0-x15 & x29-x30)
        ldp     x29, x30, [sp], #16
        ldp     x14, x15, [sp], #16
        ldp     x12, x13, [sp], #16
        ldp     x10, x11, [sp], #16
        ldp     x8,  x9,  [sp], #16
        ldp     x6,  x7,  [sp], #16
        ldp     x4,  x5,  [sp], #16
        ldp     x2,  x3,  [sp], #16
        ldp     x0,  x1,  [sp], #16
        eret

        .balign 0x80
el3_curr_el_spx_irq:
        // Simple exception handler (nested interrupts not allowed) :
        // see [ARM 100933_0100_en]
        // - stack all corruptible registers (x0-x15 & x29-x30)
        stp     x0,  x1,  [sp, #-16]!
        stp     x2,  x3,  [sp, #-16]!
        stp     x4,  x5,  [sp, #-16]!
        stp     x6,  x7,  [sp, #-16]!
        stp     x8,  x9,  [sp, #-16]!
        stp     x10, x11, [sp, #-16]!
        stp     x12, x13, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        stp     x29, x30, [sp, #-16]!
        // - call C IRQ handler
        bl      el3_curr_el_spx_irq_vector
        // - restore all corruptible registers (x0-x15 & x29-x30)
        ldp     x29, x30, [sp], #16
        ldp     x14, x15, [sp], #16
        ldp     x12, x13, [sp], #16
        ldp     x10, x11, [sp], #16
        ldp     x8,  x9,  [sp], #16
        ldp     x6,  x7,  [sp], #16
        ldp     x4,  x5,  [sp], #16
        ldp     x2,  x3,  [sp], #16
        ldp     x0,  x1,  [sp], #16
        eret

        .balign 0x80
el3_curr_el_spx_fiq:
        // Simple exception handler (nested interrupts not allowed) :
        // see [ARM 100933_0100_en]
        // - stack all corruptible registers (x0-x15 & x29-x30)
        stp     x0,  x1,  [sp, #-16]!
        stp     x2,  x3,  [sp, #-16]!
        stp     x4,  x5,  [sp, #-16]!
        stp     x6,  x7,  [sp, #-16]!
        stp     x8,  x9,  [sp, #-16]!
        stp     x10, x11, [sp, #-16]!
        stp     x12, x13, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        stp     x29, x30, [sp, #-16]!
        // - call C IRQ handler
        bl      el3_curr_el_spx_fiq_vector
        // - restore all corruptible registers (x0-x15 & x29-x30)
        ldp     x29, x30, [sp], #16
        ldp     x14, x15, [sp], #16
        ldp     x12, x13, [sp], #16
        ldp     x10, x11, [sp], #16
        ldp     x8,  x9,  [sp], #16
        ldp     x6,  x7,  [sp], #16
        ldp     x4,  x5,  [sp], #16
        ldp     x2,  x3,  [sp], #16
        ldp     x0,  x1,  [sp], #16
        eret

        .balign 0x80
el3_curr_el_spx_serror:
        // Simple exception handler (nested interrupts not allowed) :
        // see [ARM 100933_0100_en]
        // - stack all corruptible registers (x0-x15 & x29-x30)
        stp     x0,  x1,  [sp, #-16]!
        stp     x2,  x3,  [sp, #-16]!
        stp     x4,  x5,  [sp, #-16]!
        stp     x6,  x7,  [sp, #-16]!
        stp     x8,  x9,  [sp, #-16]!
        stp     x10, x11, [sp, #-16]!
        stp     x12, x13, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        stp     x29, x30, [sp, #-16]!
        // - call C IRQ handler
        bl      el3_curr_el_spx_serror_vector
        // - restore all corruptible registers (x0-x15 & x29-x30)
        ldp     x29, x30, [sp], #16
        ldp     x14, x15, [sp], #16
        ldp     x12, x13, [sp], #16
        ldp     x10, x11, [sp], #16
        ldp     x8,  x9,  [sp], #16
        ldp     x6,  x7,  [sp], #16
        ldp     x4,  x5,  [sp], #16
        ldp     x2,  x3,  [sp], #16
        ldp     x0,  x1,  [sp], #16
        eret

        .balign 0x80
        // Lower EL using AArch64
el3_lower_el_aarch64_sync:
        // Simple exception handler (nested interrupts not allowed) :
        // see [ARM 100933_0100_en]
        // - stack all corruptible registers :
        //   see [ARM DEN 0028] "Register use in AArch64 SMC and HVC calls"
        //   . x0-x3 not saved/restored because used as SMC parameters
        //   . x4-x17 + x29-x30
        stp     x4,  x5,  [sp, #-16]!
        stp     x6,  x7,  [sp, #-16]!
        stp     x8,  x9,  [sp, #-16]!
        stp     x10, x11, [sp, #-16]!
        stp     x12, x13, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        stp     x16, x17, [sp, #-16]!
        stp     x29, x30, [sp, #-16]!
        // - call C SMC handler 
        bl      el3_lower_el_aarch64_sync_vector
        ldp     x29, x30, [sp], #16
        ldp     x16, x17, [sp], #16
        ldp     x14, x15, [sp], #16
        ldp     x12, x13, [sp], #16
        ldp     x10, x11, [sp], #16
        ldp     x8,  x9,  [sp], #16
        ldp     x6,  x7,  [sp], #16
        ldp     x4,  x5,  [sp], #16
        eret

        .balign 0x80
el3_lower_el_aarch64_irq:
        b       el3_lower_el_aarch64_irq_vector
        b       el3_lower_el_aarch64_irq

        .balign 0x80
el3_lower_el_aarch64_fiq:
        b       el3_lower_el_aarch64_fiq_vector
        b       el3_lower_el_aarch64_fiq

        .balign 0x80
el3_lower_el_aarch64_serror:
        b       el3_lower_el_aarch64_serror_vector
        b       el3_lower_el_aarch64_serror

        .balign 0x80
        // Lower EL using AArch32
el3_lower_el_aarch32_sync:
        // Simple exception handler (nested interrupts not allowed) :
        // see [ARM 100933_0100_en]
        // - stack all corruptible registers :
        //   see [ARM DEN 0028] "Register use in AArch32 SMC and HVC calls"
        //   . x0-x3 not saved/restored because used as SMC parameters
        //   . x4-x15
        stp     x4,  x5,  [sp, #-16]!
        stp     x6,  x7,  [sp, #-16]!
        stp     x8,  x9,  [sp, #-16]!
        stp     x10, x11, [sp, #-16]!
        stp     x12, x13, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        // - call C SMC handler 
        bl      el3_lower_el_aarch32_sync_vector
        ldp     x14, x15, [sp], #16
        ldp     x12, x13, [sp], #16
        ldp     x10, x11, [sp], #16
        ldp     x8,  x9,  [sp], #16
        ldp     x6,  x7,  [sp], #16
        ldp     x4,  x5,  [sp], #16
        eret

        .balign 0x80
el3_lower_el_aarch32_irq:
        b       el3_lower_el_aarch32_irq_vector
        b       el3_lower_el_aarch32_irq

        .balign 0x80
el3_lower_el_aarch32_fiq:
        b       el3_lower_el_aarch32_fiq_vector
        b       el3_lower_el_aarch32_fiq

        .balign 0x80
el3_lower_el_aarch32_serror:
        b       el3_lower_el_aarch32_serror_vector
        b       el3_lower_el_aarch32_serror

        .balign 0x80
//        .extern main_a35_1
        .globl _Reset_EL3
        .type _Reset_EL3, @function
        .globl _ResetA35
        .set _ResetA35, _Reset_EL3
    .balign 8
_Reset_EL3:

/*============================================================================*/
/* Initialize registers                                                       */
/*============================================================================*/
        /* Initialise the register bank */
        mov     x0,  xzr
        mov     x1,  xzr
        mov     x2,  xzr
        mov     x3,  xzr
        mov     x4,  xzr
        mov     x5,  xzr
        mov     x6,  xzr
        mov     x7,  xzr
        mov     x8,  xzr
        mov     x9,  xzr
        mov     x10, xzr
        mov     x11, xzr
        mov     x12, xzr
        mov     x13, xzr
        mov     x14, xzr
        mov     x15, xzr
        mov     x16, xzr
        mov     x17, xzr
        mov     x18, xzr
        mov     x19, xzr
        mov     x20, xzr
        mov     x21, xzr
        mov     x22, xzr
        mov     x23, xzr
        mov     x24, xzr
        mov     x25, xzr
        mov     x26, xzr
        mov     x27, xzr
        mov     x28, xzr
        mov     x29, xzr
        mov     x30, xzr

        /* Zero the stack pointers, link registers and status registers */
        mov     sp,       x0
        msr     sp_el0,   x0
        msr     sp_el1,   x0
        msr     sp_el2,   x0
        msr     elr_el1,  x0
        msr     elr_el2,  x0
        msr     elr_el3,  x0
        msr     spsr_el1, x0
        msr     spsr_el2, x0
        msr     spsr_el3, x0

/*============================================================================*/
/* Disable MMU in case it was left enabled from an earlier run                */
/* This does not need to be done from a cold reset                            */
/*============================================================================*/
        mrs     x0, SCTLR_EL3
        bic     x0, x0, #0x1
        msr     SCTLR_EL3, x0

/*============================================================================*/
/* Set up Vector Base Address                                                 */
/*============================================================================*/
        ldr     x1, =_el3_vector_table
        msr     VBAR_EL3, x1

/*============================================================================*/
/* Initialize EL3 stacks according to core number according to EL1 stack base */
/*           stack_el3(A35_0) = EL3_STACK_BASE                                */
/*           stack_el3(A35_1) = EL3_STACK_BASE - 0x0400                       */
/* Note stack must be 8-byte aligned.                                         */
/*============================================================================*/
        /* Get value of Multiprocessor Affinity Register (EL1) */
        /* (see B2.37 in [A35_TRM])                            */
        mrs     x0, mpidr_el1
        /* Extract CPU ID bits */
        and     x0, x0, #0xFF
        /* Compute cpu_id * 0x400 */
        lsl     x0, x0, #0x0A
        /* Get stack base (shall be 8-byte aligned = 0x40-multiple) */
        ldr     x1, =EL3_STACK_BASE
        /* Compute stack_base - (cpu_id * 0x400) */
        sub     x0, x1, x0
        /* Set EL3 stack pointer (SP) */
        mov     sp, x0
        isb     sy

/*============================================================================*/
/* Enable NEON and Floating Point (see 5.4 at page 50 in [ARM_DAI_0527A])     */
/* "In AArch64, you do not need to enable access to the NEON
    and FP registers.
    However, access to the NEON and FP registers can still be trapped"        */
/*============================================================================*/
        /* - Check FP presence by reading ID_AA64PFR0_EL1 bits [19:16] "FP"  */
        /*   see [A35_TRM] B2.54 "AArch64 Processor Feature Register 0, EL1" */
        mrs     x0, ID_AA64PFR0_EL1
        ubfx    x0, x0, #16, #4
        /* - Skip FP initialization when FP not present */
        cbnz    x0, after_neon

        /* - Disable trapping of accessing in EL3 and EL2                   */
        /*   by clearing CPTR_EL3 bits 31 "TCPAC", 20 "TTA" and 10 "TFP"    */
        /*   see [A35_TRM] B2.33 "Architectural Feature Trap Register, EL3" */
        msr     CPTR_EL3, xzr
        /*   by letting CPTR_EL2 bits 31 "TCPAC" and 10 "TFP" set to 0b0    */
        /*   see [A35_TRM] B2.32 "Architectural Feature Trap Register, EL2" */

        /* - Disable access trapping in EL1 and EL0           */
        /*   by setting CPACR_EL1 bits [21:20] "FPEN" to 0b11 */
        /*   see [A35_TRM] B2.31 "Architectural Feature Access Control Register, EL1" */
        mov     x1, #(0x3 << 20)
        msr     CPACR_EL1, x1
        isb     sy

        /* - Initialize FPU registers */
        fmov    d0,  xzr
        fmov    d1,  xzr
        fmov    d2,  xzr
        fmov    d3,  xzr
        fmov    d4,  xzr
        fmov    d5,  xzr
        fmov    d6,  xzr
        fmov    d7,  xzr
        fmov    d8,  xzr
        fmov    d9,  xzr
        fmov    d10, xzr
        fmov    d11, xzr
        fmov    d12, xzr
        fmov    d13, xzr
        fmov    d14, xzr
        fmov    d15, xzr
        fmov    d16, xzr
        fmov    d17, xzr
        fmov    d18, xzr
        fmov    d19, xzr
        fmov    d20, xzr
        fmov    d21, xzr
        fmov    d22, xzr
        fmov    d23, xzr
        fmov    d24, xzr
        fmov    d25, xzr
        fmov    d26, xzr
        fmov    d27, xzr
        fmov    d28, xzr
        fmov    d29, xzr
        fmov    d30, xzr
        fmov    d31, xzr
after_neon:

/*============================================================================*/
/* EL3 MMU Configuration                                                      */
/*                                                                            */
/* Configure system to use extended v6 format page tables,                    */
/* with Access Flag Enable (AFE) bit disabled.                                */
/* Set translation table base                                                 */
/*============================================================================*/
        /* Configure Translation tables by setting properly                   */
        /* "Translation Control Register (EL3)" (see B2.96 in [A35_TRM]) :    */
        /* - bits   [5:0] "T0SZ" = 0b01_1111 = 31                             */
        /*          "Size offset of the memory region addressed by TTBR0_EL3" */
        /*          <=>   "The region size is 2^(64-T0SZ) = 2^33 bytes = 4GB" */
        /* - bits   [7:6] "RES0" = 0b00                                       */
        /* - bits   [9:8] "IRGN0" = 0b01 / 0b00                               */
        /*            <=> "Normal memory, Inner Write-Back Read-Allocate      */
        /*                 Write-Allocate Cacheable"                          */
        /*              / "Normal memory, Inner Non-cacheable"                */
        /* - bits [11:10] "ORGN0" = 0b01 / 0b00                               */
        /*            <=> "Normal memory, Outer Write-Back Read-Allocate      */
        /*                 Write-Allocate Cacheable"                          */
        /*              / "Normal memory, Inner Non-cacheable"                */
        /* - bits [13:12] "SH0" = 0b11 	                                      */
        /*            <=> "Inner Shareable"                                   */
        /* - bits [15:14] "TG0" = 0b00                                        */
        /*            <=> "Granule size for the TTBR0_EL3 is 4KB."            */
        /* - bits [18:16] "PS" = 0b001                                        */
        /*            <=> "Physical address size is 36 bits, 64GB"            */
        /* - bit     [19] "RES0" = 0b0                                        */
        /* - bit     [20] "TBI"  = 0b0                                        */
        /*            <=> "Top byte used in the address calculation"          */
        /* - bits [22:21] "RES0" = 0b00                                       */
        /* - bit     [23] "RES1" = 0b1                                        */
        /* - bits [30:24] "RES0" = 0b0000000                                  */
        /* - bit     [31] "RES1" = 0b1	                                      */
        /* ==> 0x8081_351F / 0x8081_301F */

.ifdef NON_CACHEABLE_TTB
        /* bits [9:8] "IRGN0" = bits [11:10] "ORGN0" = 0b00 */
        /* = "Inner and Outer Non-cacheable" */
        ldr     w1, =0x8081301F
.else
        /* bits [9:8] "IRGN0" = bits [11:10] "ORGN0" = 0b01 */
        /* <=> "Inner and Outer Cacheable" */
        ldr     w1, =0x8081351F
.endif
        msr     TCR_EL3, x1
        ldr     x1, mair_value
        msr     MAIR_EL3, x1

        adr     x0, mp2_el3_ttb0_base
        msr     ttbr0_el3, x0

/*============================================================================*/
/* SMP (Symmetric Multi-Processing) mode : TO BE MANAGED AT EL1 !             */
/* Clear bit 6 "SMPEN"/"Enable hardware management of data coherency"         */
/* in CPU Extended Control register (CPUECTLR_EL1)                            */
/*============================================================================*/
        mrs     x0, S3_1_C15_C2_1
        bic     x0, x0, #(1<<6)
        msr     S3_1_C15_C2_1, x0
        isb

/*============================================================================*/
/* Enable data & instruction caches and branch predictor and MMU at EL3       */
/* by setting SCTLR_EL3 bits 0 "M", 2 "C" and 12 "I"                          */
/* (see B2.92 "System Control Register, EL3" in [A35_TRM])                    */
/* Note : Branch prediction not controllable on A35 (bit 11 = res1)           */
/*============================================================================*/
        mrs     x0, SCTLR_EL3
        bic     x0, x0, #(0x1 << 3)    /* Disable Stack Strict Alignment */
        orr     x0, x0, #(0x1 << 0)    /* enable MMU before scatter loading */
        orr     x0, x0, #(0x1 << 2)    /* enable D Cache        */
        orr     x0, x0, #(0x1 << 12)   /* enable I Cache        */
        msr     SCTLR_EL3, x0

/*============================================================================*/
/* Finalize startup processing according to A35 core index                    */
/*============================================================================*/
        /* Get CPU index by reading MPIDR_EL1 bits [7:0] "Aff0"            */
        /* (see B2.84 "Multiprocessor Affinity Register, EL1" in [A35_TRM] */
        mrs     x0, MPIDR_EL1
        and     x0, x0, #0xFF
        /* Branch A35_1 core to its processing */
        cmp     x0, #1
        b.eq    servant_core1

        /* A35_0 core : Initialize data */
        stp     x19,x30,[sp,#-0x10]!
        ldr     x0, =__bss_start__
        ldr     x1, =__bss_end__
        sub     x2, x1, x0
        mov     x1, #0
        bl      memset
        /* Remove semi-hosting configuration : not needed */
//        bl      initialise_monitor_handles
        /* Remove __libc_init_array/__libc_fini_array : not needed (no C++ feature) */
//        ldr     x0, =__libc_fini_array
//        bl      atexit
//        bl      __libc_init_array
	    dsb     sy
	    isb

        /* A35_0 core : Branch to main function */
        bl      main
        b       exit

servant_core1:
        /* A35_1 core : Branch to its own main function */
//        b       main_a35_1
        b      servant_core1


/*============================================================================*/
/* Pagetables Initialization                                                  */
/*============================================================================*/
        .align 3
        .global  mp2_el3_ttb0_base
        .global  mair_value

        .equ XN,               (1<<22)
        .equ PXN,              (1<<21)
        .equ CONTIG,           (1<<20)

        .equ NGLOBAL,          (1<<11)
        .equ AF,               (1<<10) /* Access flag */
        .equ NON_SH,           (0<<8)
        .equ OUTER_SH,         (2<<8)
        .equ INNER_SH,         (3<<8)
        .equ AP2,              (1<<7) /* Read-only flag  */
        .equ AP1,              (1<<6) /* ELx access flag */
        .equ NS,               (1<<5)
        .equ IDX0,             (0<<2)
        .equ IDX1,             (1<<2)
        .equ IDX2,             (2<<2)
        .equ IDX3,             (3<<2)
        .equ IDX4,             (4<<2)
        .equ IDX5,             (5<<2)
        .equ IDX6,             (6<<2)
        .equ IDX7,             (7<<2)
        .equ BLOCK,            (1<<0)
        .equ TABLE,            (3<<0)
        .equ UNMAPPED,         (0<<0)

        .equ OUTER_WT,         (0<<6)
        .equ OUTER_WB,         (1<<6)
        .equ OUTER_NC,         (1<<6)
        .equ OUTER_NONTRANS,   (1<<7)
        .equ OUTER_WRALLOC,    (1<<4)
        .equ OUTER_RDALLOC,    (1<<5)

        /* Device Memory (see [ARMv8_RM DDI0487B.a] B2.7.2 page B2-112) : */
        /* - Device-nGnRnE memory aka (ARMv7) "Strongly Ordered Memory"   */
        /*   "nE" stands for "no Early Write Acknowledgement"             */
        .equ nGnRnE,           0x00
        /* - Device-nGnRE memory aka (ARMv7) "Device Memory"              */
        .equ nGnRE,            0x04
        /* - Device-nGRE memory */
        .equ nGRE,             0x08
        /* - Device-GRE memory */
        .equ GRE,              0x0C

        .equ INNER_WT,         (0<<2)
        .equ INNER_WB,         (1<<2)
        .equ INNER_NC,         (1<<2)
        .equ INNER_NONTRANS,   (1<<3)
        .equ INNER_WRALLOC,    (1<<0)
        .equ INNER_RDALLOC,    (1<<1)

        /* Normal Memory Inner & Outer cacheable (Write-Back Non transient Read & Write Allocate) */
        .equ IO_WBRWA, (OUTER_WB | OUTER_NONTRANS | OUTER_WRALLOC | OUTER_RDALLOC | INNER_WB | INNER_NONTRANS | INNER_WRALLOC | INNER_RDALLOC)
        /* Normal Memory Inner & Outer Non-cacheable */
        .equ IO_NC,    (INNER_NC | OUTER_NC)

        /* Memory attributes for MAIR */
        .equ ATTR0,            nGnRnE
        .equ ATTR1,            nGnRE
        .equ ATTR2,            IO_WBRWA
        .equ ATTR3,            IO_NC
        .equ ATTR4,            0
        .equ ATTR5,            0
        .equ ATTR6,            0
        .equ ATTR7,            0

        /* Secure Memory (by default on EL3) */
        .equ SO_MEMORY,           (IDX0 | AP1 | AF)
        .equ DEVICE,              (IDX1 | AP1 | AF)
        .equ MEMORY,              (IDX2 | AP1 | AF)
        .equ NC_MEMORY,           (IDX3 | AP1 | AF)

        /* Secure Memory (for ELx < EL3) */
        .equ SO_ELX_MEMORY,       (IDX0 | AF)
        .equ ELX_DEVICE,          (IDX1 | AF)
        .equ ELX_MEMORY,          (IDX2 | AF)
        .equ NC_ELX_MEMORY,       (IDX3 | AF)

        /* Non-secure Memory (for ELx < EL3) */
        .equ NS_SO_MEMORY,        (SO_ELX_MEMORY | NS)
        .equ NS_DEVICE,           (ELX_DEVICE    | NS)
        .equ NS_MEMORY,           (ELX_MEMORY    | NS)
        .equ NS_NC_MEMORY,        (NC_ELX_MEMORY | NS)

        /* Memory shareability :                                    */
        /* (see [ARMv8_RM DDI0487B.a] D4.5.1 pages D4-2090/D4-2091) */
        /* - for cacheable memory : set to Inner Shareable          */
        .equ SHARED_MEMORY,        (MEMORY    | INNER_SH)
        .equ SHARED_NS_MEMORY,     (NS_MEMORY | INNER_SH)
        .equ SHARED_ELX_MEMORY,    (ELX_MEMORY | INNER_SH)
        /* - for non-cacheable memory : set to Outer Shareable      */
        /*   (see "Note" at page D4-2091 in [ARMv8_RM DDI0487B.a])  */
        .equ SHARED_SO_MEMORY,     (SO_MEMORY  | OUTER_SH)
        .equ SHARED_DEVICE,        (DEVICE     | OUTER_SH)
        .equ SHARED_NC_MEMORY,     (NC_MEMORY  | OUTER_SH)

        .equ SHARED_SO_ELX_MEMORY, (SO_ELX_MEMORY | OUTER_SH)
        .equ SHARED_ELX_DEVICE,    (ELX_DEVICE    | OUTER_SH)
        .equ SHARED_NC_ELX_MEMORY, (NC_ELX_MEMORY | OUTER_SH)

        .equ SHARED_NS_SO_MEMORY,  (NS_SO_MEMORY | OUTER_SH)
        .equ SHARED_NS_DEVICE,     (NS_DEVICE    | OUTER_SH)
        .equ SHARED_NS_NC_MEMORY,  (NS_NC_MEMORY | OUTER_SH)

        /* Put 64-bit value with correct endianness */
        .macro PUT_64B high, low
        .quad (\high << 32) + \low
        .endm

        .align 3
mair_value:
        PUT_64B ((ATTR7<<24) | (ATTR6<<16) | (ATTR5<<8) | (ATTR4<<0), (ATTR3<<24) | (ATTR2<<16) | (ATTR1<<8) | (ATTR0<<0))

        /* Create an entry pointing to a next-level table */
        .macro TABLE_ENTRY PA, ATTR
        PUT_64B   \ATTR, ((\PA) + TABLE)
        .endm

        /* Create an entry for a 1GB block */
        .macro BLOCK_1GB PA, ATTR_HI, ATTR_LO
        PUT_64B   \ATTR_HI, ((\PA)  &  0x1C0000000)  |  \ATTR_LO  |  BLOCK
        .endm

        /* Create an entry for a 2MB block */
        .macro BLOCK_2MB PA, ATTR_HI, ATTR_LO
        PUT_64B   \ATTR_HI, ((\PA)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        .endm

        /* Create an entry for a 32MB block (using multiple 2MB blocks) */
        .macro BLOCK_32MB PA, ATTR_HI, ATTR_LO
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0000000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0200000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0400000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0600000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0800000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0A00000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0C00000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x0E00000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1000000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1200000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1400000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1600000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1800000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1A00000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1C00000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        PUT_64B   \ATTR_HI  |  CONTIG, ((\PA  |  0x1E00000)  &  0xFFE00000)  |  \ATTR_LO  |  BLOCK
        .endm

        /* Create an entry for a 1GB invalid block */
        .macro INVALID_1GB PA
        PUT_64B   0, ((\PA)  &  0xC0000000)
        .endm

        /* Create an entry for a 2MB invalid block */
        .macro INVALID_2MB PA
        PUT_64B   0, ((\PA)  &  0xFFE00000)
        .endm

        /* Create invalid entries for 32MB area */
        .macro INVALID_32MB PA
        PUT_64B   0, ((\PA  |  0x0000000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x0200000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x0400000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x0600000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x0800000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x0A00000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x0C00000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x0E00000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1000000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1200000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1400000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1600000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1800000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1A00000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1C00000)  &  0xFFE00000)
        PUT_64B   0, ((\PA  |  0x1E00000)  &  0xFFE00000)
        .endm

/*------------------------------------------------------*/
/* Panther (MP2) EL3 First-level table : minimal config */
/*                                                      */
/*   Entry 0: -> Block 0x00000000 - 0x3FFFFFFF          */
/*               (Whole range as non-cacheable Memory)  */
/*   Entry 1: -> Block 0x40000000 - 0x7FFFFFFF          */
/*               Shared Strongly Ordered Memory         */
/*   Entry 2: -> Block 0x80000000 - 0xBFFFFFFF          */
/*               (Whole range as non-cacheable Memory)  */
/*   Entry 3: -> Block 0xC0000000 - 0xFFFFFFFF          */
/*               (Whole range as non-cacheable Memory)  */
/*   Entry 4: -> Block 0x100000000 - 0x13FFFFFFF        */
/*               (Whole range as non-cacheable Memory)  */
/*   Entry 5: -> Block 0x140000000 - 0x17FFFFFFF        */
/*               (Whole range as non-cacheable Memory)  */
/*------------------------------------------------------*/
        /* Table base address aligned to 32-byte */
        /* according to TCR_EL3.T0SZ value       */
        .balign 0x20
mp2_el3_ttb0_base:
        BLOCK_1GB   0x00000000, 0, SHARED_NC_MEMORY
        BLOCK_1GB   0x40000000, 0, SHARED_SO_MEMORY
        BLOCK_1GB   0x80000000, 0, SHARED_NC_MEMORY
        BLOCK_1GB   0xC0000000, 0, SHARED_NC_MEMORY
        BLOCK_1GB   0x100000000, 0, SHARED_NC_MEMORY
        BLOCK_1GB   0x140000000, 0, SHARED_NC_MEMORY
