/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2020 SiFive
 */

#ifndef __ASM_RISCV_VECTOR_H
#define __ASM_RISCV_VECTOR_H

#ifdef CONFIG_RISCV_ISA_V

#include <linux/types.h>
#include <asm/csr.h>

#define CSR_STR_VAL(x)	#x
#define CSR_STR(x)	CSR_STR_VAL(x)

static __always_inline void rvv_enable(void)
{
	asm volatile (
		"csrs	" CSR_STR(CSR_STATUS) ", %0\n\t"
		: : "r" (SR_VS) :);
}

static __always_inline void rvv_disable(void)
{
	asm volatile (
		"csrc	" CSR_STR(CSR_STATUS) ", %0\n\t"
		: : "r" (SR_VS) :);
}

#endif /* CONFIG_RISCV_ISA_V  */

#endif /* ! __ASM_RISCV_VECTOR_H */
