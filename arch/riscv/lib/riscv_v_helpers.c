// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2023 SiFive
 * Author: Andy Chiu <andy.chiu@sifive.com>
 */
#ifndef __NO_FORTIFY
# define __NO_FORTIFY
#endif
#include <linux/linkage.h>
#include <asm/asm.h>

#include <asm/string.h>
#include <asm/vector.h>
#include <asm/simd.h>

#ifdef CONFIG_MMU
#include <asm/asm-prototypes.h>
#endif

#ifdef CONFIG_MMU
size_t riscv_v_usercopy_threshold = CONFIG_RISCV_ISA_V_UCOPY_THRESHOLD;
int __asm_vector_usercopy(void *dst, void *src, size_t n);
int fallback_scalar_usercopy(void *dst, void *src, size_t n);
asmlinkage int enter_vector_usercopy(void *dst, void *src, size_t n)
{
	size_t remain, copied;

	/* skip has_vector() check because it has been done by the asm  */
	if (!may_use_simd())
		goto fallback;

	kernel_vector_begin();
	remain = __asm_vector_usercopy(dst, src, n);
	kernel_vector_end();

	if (remain) {
		copied = n - remain;
		dst += copied;
		src += copied;
		n = remain;
		goto fallback;
	}

	return remain;

fallback:
	return fallback_scalar_usercopy(dst, src, n);
}
#endif

#define V_OPT_TEMPLATE3(prefix, type_r, type_0, type_1)				\
extern type_r __asm_##prefix##_vector(type_0, type_1, size_t n);		\
type_r prefix(type_0 a0, type_1 a1, size_t n)					\
{										\
	type_r ret;								\
	if (has_vector() && may_use_simd() &&					\
	    n > riscv_v_##prefix##_threshold) {					\
		kernel_vector_begin();						\
		ret = __asm_##prefix##_vector(a0, a1, n);			\
		kernel_vector_end();						\
		return ret;							\
	}									\
	return __##prefix(a0, a1, n);						\
}

static size_t riscv_v_memset_threshold = CONFIG_RISCV_ISA_V_MEMSET_THRESHOLD;
V_OPT_TEMPLATE3(memset, void *, void*, int)
static size_t riscv_v_memcpy_threshold = CONFIG_RISCV_ISA_V_MEMCPY_THRESHOLD;
V_OPT_TEMPLATE3(memcpy, void *, void*, const void *)
static size_t riscv_v_memmove_threshold = CONFIG_RISCV_ISA_V_MEMMOVE_THRESHOLD;
V_OPT_TEMPLATE3(memmove, void *, void*, const void *)
