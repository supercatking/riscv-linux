/* SPDX-License-Identifier: GPL-2.0-only */
#include <linux/linkage.h>
#include <asm/asm.h>

#define pDst a0
#define pSrc a1
#define iNum a2

#define iVL a3
#define pDstPtr a4
#define pSrcBackwardPtr a5
#define pDstBackwardPtr a6

#define ELEM_LMUL_SETTING m8
#define vData v0

SYM_FUNC_START(__asm_memmove_vector)

    mv pDstPtr, pDst

    bgeu pSrc, pDst, forward_copy_loop
    add pSrcBackwardPtr, pSrc, iNum
    add pDstBackwardPtr, pDst, iNum
    bltu pDst, pSrcBackwardPtr, backward_copy_loop

forward_copy_loop:
    vsetvli iVL, iNum, e8, ELEM_LMUL_SETTING, ta, ma

    vle8.v vData, (pSrc)
    sub iNum, iNum, iVL
    add pSrc, pSrc, iVL
    vse8.v vData, (pDstPtr)
    add pDstPtr, pDstPtr, iVL

    bnez iNum, forward_copy_loop
    ret

backward_copy_loop:
    vsetvli iVL, iNum, e8, ELEM_LMUL_SETTING, ta, ma

    sub pSrcBackwardPtr, pSrcBackwardPtr, iVL
    vle8.v vData, (pSrcBackwardPtr)
    sub iNum, iNum, iVL
    sub pDstBackwardPtr, pDstBackwardPtr, iVL
    vse8.v vData, (pDstBackwardPtr)
    bnez iNum, backward_copy_loop
    ret

SYM_FUNC_END(__asm_memmove_vector)
