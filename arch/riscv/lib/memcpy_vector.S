/* SPDX-License-Identifier: GPL-2.0-only */

#include <linux/linkage.h>
#include <asm/asm.h>

#define pDst a0
#define pSrc a1
#define iNum a2

#define iVL a3
#define pDstPtr a4

#define ELEM_LMUL_SETTING m8
#define vData v0


/* void *memcpy(void *, const void *, size_t) */
SYM_FUNC_START(__asm_memcpy_vector)
	mv pDstPtr, pDst
loop:
	vsetvli iVL, iNum, e8, ELEM_LMUL_SETTING, ta, ma
	vle8.v vData, (pSrc)
	sub iNum, iNum, iVL
	add pSrc, pSrc, iVL
	vse8.v vData, (pDstPtr)
	add pDstPtr, pDstPtr, iVL
	bnez iNum, loop
	ret
SYM_FUNC_END(__asm_memcpy_vector)
