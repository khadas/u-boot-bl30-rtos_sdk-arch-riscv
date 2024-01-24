/*
 * Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef ENTRY_S
#define ENTRY_S

#include "riscv_encoding.h"

#if __riscv_xlen == 64
#define SLL32 sllw
#define STORE sd
#define LOAD ld
#define LWU lwu
#define LOG_REGBYTES 3
#else
#define SLL32 sll
#define STORE sw
#define LOAD lw
#define LWU lw
#define LOG_REGBYTES 2
#endif
#define REGBYTES (1 << LOG_REGBYTES)

#if __riscv_flen == 64
#define FPSTORE fsd
#define FPLOAD fld
#define LOG_FPREGBYTES 3
#else
#define FPSTORE fsw
#define FPLOAD flw
#define LOG_FPREGBYTES 2
#endif

.align 2
.extern interrupt_register_nmi
.extern interrupt_register_exception

.macro wr_reg, address, value
    li  t0, \address
    li  t1, \value
    sw  t1, (t0)
.endm

###############################################
###############################################
#only save caller registers
.macro SAVE_CONTEXT
  STORE x1,  0*REGBYTES(sp)
  STORE x5,  1*REGBYTES(sp)
  STORE x6,  2*REGBYTES(sp)
  STORE x7,  3*REGBYTES(sp)
  STORE x10, 4*REGBYTES(sp)
  STORE x11, 5*REGBYTES(sp)
  STORE x12, 6*REGBYTES(sp)
  STORE x13, 7*REGBYTES(sp)
  STORE x14, 8*REGBYTES(sp)
  STORE x15, 9*REGBYTES(sp)
  STORE x16, 10*REGBYTES(sp)
  STORE x17, 11*REGBYTES(sp)
  STORE x28, 12*REGBYTES(sp)
  STORE x29, 13*REGBYTES(sp)
  STORE x30, 14*REGBYTES(sp)
  STORE x31, 15*REGBYTES(sp)
.endm
###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT
  LOAD x1,  0*REGBYTES(sp)
  LOAD x5,  1*REGBYTES(sp)
  LOAD x6,  2*REGBYTES(sp)
  LOAD x7,  3*REGBYTES(sp)
  LOAD x10, 4*REGBYTES(sp)
  LOAD x11, 5*REGBYTES(sp)
  LOAD x12, 6*REGBYTES(sp)
  LOAD x13, 7*REGBYTES(sp)
  LOAD x14, 8*REGBYTES(sp)
  LOAD x15, 9*REGBYTES(sp)
  LOAD x16, 10*REGBYTES(sp)
  LOAD x17, 11*REGBYTES(sp)
  LOAD x28, 12*REGBYTES(sp)
  LOAD x29, 13*REGBYTES(sp)
  LOAD x30, 14*REGBYTES(sp)
  LOAD x31, 15*REGBYTES(sp)
.endm
###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT_EXCEPT_X5
  LOAD x1,  0*REGBYTES(sp)
  LOAD x6,  2*REGBYTES(sp)
  LOAD x7,  3*REGBYTES(sp)
  LOAD x10, 4*REGBYTES(sp)
  LOAD x11, 5*REGBYTES(sp)
  LOAD x12, 6*REGBYTES(sp)
  LOAD x13, 7*REGBYTES(sp)
  LOAD x14, 8*REGBYTES(sp)
  LOAD x15, 9*REGBYTES(sp)
  LOAD x16, 10*REGBYTES(sp)
  LOAD x17, 11*REGBYTES(sp)
  LOAD x28, 12*REGBYTES(sp)
  LOAD x29, 13*REGBYTES(sp)
  LOAD x30, 14*REGBYTES(sp)
  LOAD x31, 15*REGBYTES(sp)
.endm
###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT_ONLY_X5
  LOAD x5,  1*REGBYTES(sp)
.endm
###############################################
###############################################
# Save the mepc and mstatus
#
.macro SAVE_MEPC_MSTATUS
  csrr x5, CSR_MEPC  
  STORE x5,  16*REGBYTES(sp)
  csrr x5, CSR_MSTATUS  
  STORE x5,  17*REGBYTES(sp)
  csrr x5, CSR_MSUBM
  STORE x5,  18*REGBYTES(sp)
.endm

###############################################
###############################################
# Restore the mepc and mstatus
#
.macro RESTORE_MEPC_MSTATUS
  LOAD x5,  16*REGBYTES(sp)
  csrw CSR_MEPC, x5  
  LOAD x5,  17*REGBYTES(sp)
  csrw CSR_MSTATUS, x5  
  LOAD x5,  18*REGBYTES(sp)
  csrw CSR_MSUBM, x5
.endm
###############################################
###############################################
# Disable Interrupt
#
.macro DISABLE_MIE
  csrc CSR_MSTATUS, MSTATUS_MIE
.endm
###############################################
###############################################
# Enable Interrupt
#
.macro ENABLE_MIE
  csrs CSR_MSTATUS, MSTATUS_MIE  
.endm
###############################################
###############################################

  .align 6
  .global exception_entry
exception_entry:
  addi sp, sp, -19*REGBYTES  // allocate stack

  // saving registers
  SAVE_CONTEXT
  // saving mepc/mstatus
  SAVE_MEPC_MSTATUS

  csrr a0, mcause
  mv a1, sp
  call interrupt_register_exception
  
  // restore mepc/mstatus registers
  RESTORE_MEPC_MSTATUS
  // restore registers
  RESTORE_CONTEXT
  
  addi sp, sp, 19*REGBYTES  // get back stack
  mret

###############################################
###############################################

  .align 2
  .global nmi_entry
nmi_entry:
  addi sp, sp, -19*REGBYTES  // allocate stack

  // saving registers
  SAVE_CONTEXT
  // saving mepc/mstatus
  SAVE_MEPC_MSTATUS
  
  csrr a0, mcause
  call interrupt_register_nmi
  
  // restore mepc/mstatus registers
  RESTORE_MEPC_MSTATUS
  // restore registers
  RESTORE_CONTEXT
  
  addi sp, sp, 19*REGBYTES  // get back stack
  mret

###############################################
###############################################

  .align 2
  .global int_entry
int_entry:
  addi sp, sp, -19*REGBYTES  // allocate stack

  // saving registers
  SAVE_CONTEXT
  // store mcause/mepc/msubm to stack
  csrrwi  x0, CSR_PUSHMCAUSE, 16
  csrrwi  x0, CSR_PUSHMEPC, 17
  csrrwi  x0, CSR_PUSHMSUBM, 18

service_loop:
  // Claim the CLIC to find its pending highest ID
  // if the ID is not 0, then automatically enable the mstatus.MIE, and jump to its vector-entry-label
  // and update the link register
  csrrw ra, CSR_JALMNXTI, ra

  RESTORE_CONTEXT_EXCEPT_X5

  #---- Critical section with interrupts disabled -----------------------
  DISABLE_MIE # Disable interrupts

  LOAD x5,  18*REGBYTES(sp)
  csrw CSR_MSUBM, x5
  LOAD x5,  17*REGBYTES(sp)
  csrw CSR_MEPC, x5
  LOAD x5,  16*REGBYTES(sp)
  csrw CSR_MCAUSE, x5

  RESTORE_CONTEXT_ONLY_X5

  // De-allocate the stack space
  addi sp, sp, 19*REGBYTES
  // Return to regular code
  mret
  
#endif
