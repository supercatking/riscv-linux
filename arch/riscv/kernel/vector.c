#include <linux/sched/signal.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#include <asm/thread_info.h>
#include <asm/processor.h>
#include <asm/parse_asm.h>
#include <asm/vector.h>
#include <asm/ptrace.h>
#include <asm/bug.h>

static bool insn_is_vector(u32 insn_buf)
{
	u32 opcode = insn_buf & __INSN_OPCODE_MASK;
	/*
	 * All V-related instructions, including CSR operations are 4-Byte. So,
	 * do not handle if the instruction length is not 4-Byte.
	 */
	if (unlikely(GET_INSN_LENGTH(insn_buf) != 4))
		return false;
	if (opcode == OPCODE_VECTOR) {
		return true;
	} else if (opcode == OPCODE_LOADFP || opcode == OPCODE_STOREFP) {
		u32 width = EXTRACT_LOAD_STORE_FP_WIDTH(insn_buf);
		if (width == LSFP_WIDTH_RVV_8 || width == LSFP_WIDTH_RVV_16 ||
		    width == LSFP_WIDTH_RVV_32 || width == LSFP_WIDTH_RVV_64 )
			return true;
	} else if (opcode == OPCODE_SYSTEM) {
		u32 csr = EXTRACT_SYSTEM_CSR(insn_buf);
		if ((csr >= CSR_VSTART && csr <= CSR_VCSR) ||
		    (csr >= CSR_VL && csr <= CSR_VLENB))
			return true;
	}
	return false;
}

int rvv_thread_zalloc(void)
{
	void *datap;

	datap = kzalloc(riscv_vsize, GFP_KERNEL);
	if (!datap)
		return -ENOMEM;
	current->thread.vstate.datap = datap;
	memset(&current->thread.vstate, 0, offsetof(struct __riscv_v_state,
						    datap));
	return 0;
}

bool rvv_first_use_handler(struct pt_regs *regs)
{
	__user u32 *epc = (u32 *) regs->epc;
	u32 tval = (u32) regs->badaddr;

	/* If V has been enabled then it is not the first-use trap */
	if (vstate_query(regs))
		return false;
	/* Get the instruction */
	if (!tval) {
		if (__get_user(tval, epc))
			return false;
	}
	/* Filter out non-V instructions */
	if (!insn_is_vector(tval))
		return false;
	/* Sanity check. datap should be null by the time of the first-use trap */
	WARN_ON(current->thread.vstate.datap);
	/*
	 * Now we sure that this is a V instruction. And it executes in the
	 * context where VS has been off. So, try to allocate the user's V
	 * context and resume execution.
	 */
	if (rvv_thread_zalloc()) {
		force_sig(SIGKILL);
		return true;
	}
	vstate_on(regs);
	return true;
}

