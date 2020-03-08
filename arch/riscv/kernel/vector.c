// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2023 SiFive
 * Author: Andy Chiu <andy.chiu@sifive.com>
 */
#include <linux/export.h>

#include <asm/vector.h>
#include <asm/csr.h>

unsigned long riscv_v_vsize __read_mostly;
EXPORT_SYMBOL_GPL(riscv_v_vsize);

void riscv_v_setup_vsize(void)
{
	/* There are 32 vector registers with vlenb length. */
	riscv_v_enable();
	riscv_v_vsize = csr_read(CSR_VLENB) * 32;
	riscv_v_disable();
}

