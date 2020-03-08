/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2020 SiFive
 */

#ifndef __ASM_RISCV_VECTOR_H
#define __ASM_RISCV_VECTOR_H

#include <linux/types.h>

#ifdef CONFIG_RISCV_ISA_V

#include <asm/hwcap.h>
#include <asm/csr.h>
#include <asm/asm.h>

#define CSR_STR(x) __ASM_STR(x)

extern unsigned long riscv_vsize;

static __always_inline bool has_vector(void)
{
	return static_branch_likely(&riscv_isa_ext_keys[RISCV_ISA_EXT_KEY_VECTOR]);
}

static inline void __vstate_clean(struct pt_regs *regs)
{
	regs->status = (regs->status & ~(SR_VS)) | SR_VS_CLEAN;
}

static inline void vstate_off(struct pt_regs *regs)
{
	regs->status = (regs->status & ~SR_VS) | SR_VS_OFF;
}

static inline void vstate_on(struct pt_regs *regs)
{
	regs->status = (regs->status & ~(SR_VS)) | SR_VS_INITIAL;
}

static inline bool vstate_query(struct pt_regs *regs)
{
	return (regs->status & SR_VS) != 0;
}

static __always_inline void rvv_enable(void)
{
	csr_set(CSR_SSTATUS, SR_VS);
}

static __always_inline void rvv_disable(void)
{
	csr_clear(CSR_SSTATUS, SR_VS);
}

static __always_inline void __vstate_csr_save(struct __riscv_v_state *dest)
{
	asm volatile (
		"csrr	%0, " CSR_STR(CSR_VSTART) "\n\t"
		"csrr	%1, " CSR_STR(CSR_VTYPE) "\n\t"
		"csrr	%2, " CSR_STR(CSR_VL) "\n\t"
		"csrr	%3, " CSR_STR(CSR_VCSR) "\n\t"
		: "=r" (dest->vstart), "=r" (dest->vtype), "=r" (dest->vl),
		  "=r" (dest->vcsr) : :);
}

static __always_inline void __vstate_csr_restore(struct __riscv_v_state *src)
{
	asm volatile (
		"vsetvl	 x0, %2, %1\n\t"
		"csrw	" CSR_STR(CSR_VSTART) ", %0\n\t"
		"csrw	" CSR_STR(CSR_VCSR) ", %3\n\t"
		: : "r" (src->vstart), "r" (src->vtype), "r" (src->vl),
		    "r" (src->vcsr) :);
}

static inline void __vstate_save(struct __riscv_v_state *save_to, void *datap)
{
	rvv_enable();
	__vstate_csr_save(save_to);
	asm volatile (
		"vsetvli	t4, x0, e8, m8, ta, ma\n\t"
		"vse8.v		v0, (%0)\n\t"
		"add		%0, %0, t4\n\t"
		"vse8.v		v8, (%0)\n\t"
		"add		%0, %0, t4\n\t"
		"vse8.v		v16, (%0)\n\t"
		"add		%0, %0, t4\n\t"
		"vse8.v		v24, (%0)\n\t"
		: : "r" (datap) : "t4", "memory");
	rvv_disable();
}

static inline void __vstate_restore(struct __riscv_v_state *restore_from,
				    void *datap)
{
	rvv_enable();
	asm volatile (
		"vsetvli	t4, x0, e8, m8, ta, ma\n\t"
		"vle8.v		v0, (%0)\n\t"
		"add		%0, %0, t4\n\t"
		"vle8.v		v8, (%0)\n\t"
		"add		%0, %0, t4\n\t"
		"vle8.v		v16, (%0)\n\t"
		"add		%0, %0, t4\n\t"
		"vle8.v		v24, (%0)\n\t"
		: : "r" (datap) : "t4");
	__vstate_csr_restore(restore_from);
	rvv_disable();
}

#else /* ! CONFIG_RISCV_ISA_V  */

struct pt_regs;

static __always_inline bool has_vector(void) { return false; }
static inline bool vstate_query(struct pt_regs *regs) { return false; }
#define riscv_vsize (0)
#define vstate_off(regs)		do {} while (0)
#define vstate_on(regs)			do {} while (0)

#endif /* CONFIG_RISCV_ISA_V */

#endif /* ! __ASM_RISCV_VECTOR_H */
