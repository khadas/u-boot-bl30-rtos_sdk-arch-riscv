/*
 * Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "exception_handler.h"
#include "riscv_encoding.h"
#include "interrupt_control_eclic.h"

uint32_t handle_exception(uint32_t mcause, uint32_t sp)
{
	uint32_t mstatus_mps_bits = ((read_csr(mstatus) & MSTATUS_MPS) >> MSTATUS_MPS_LSB);
	uint8_t exception_type = (mcause & 0x1f);

	printf("mstatus mps:0x%x,exception type:0x%x\n", mstatus_mps_bits, exception_type);

	return 0;
}

uint32_t handle_nmi(uint32_t mcause, uint32_t sp)
{
	return 0;
}

uint32_t interrupt_register_exception(uint32_t mcause, uint32_t sp)
{
	return handle_exception(mcause, sp);
}

uint32_t interrupt_register_nmi(uint32_t mcause, uint32_t sp)
{
  return handle_nmi(mcause, sp);
}

