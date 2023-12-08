/* SPDX-License-Identifier: GPL-2.0-only */
#include <linux/linkage.h>
#include <asm/asm.h>

#define pDst a0
#define iValue a1
#define iNum a2

#define iVL a3
#define iTemp a4
#define pDstPtr a5

#define ELEM_LMUL_SETTING m8
#define vData v0

/* void *memset(void *, int, size_t) */
SYM_FUNC_START(__asm_memset_vector)

    mv pDstPtr, pDst

    vsetvli iVL, iNum, e8, ELEM_LMUL_SETTING, ta, ma
    vmv.v.x vData, iValue

loop:
    vse8.v vData, (pDstPtr)
    sub iNum, iNum, iVL
    add pDstPtr, pDstPtr, iVL
    vsetvli iVL, iNum, e8, ELEM_LMUL_SETTING, ta, ma
    bnez iNum, loop

    ret

SYM_FUNC_END(__asm_memset_vector)
