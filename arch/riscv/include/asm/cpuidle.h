/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_CPUIDLE_H
#define __ASM_CPUIDLE_H

#include <asm/proc-fns.h>

#ifdef CONFIG_CPU_IDLE
extern int arm_cpuidle_init(unsigned int cpu);
extern int arm_cpuidle_suspend(int index);

static int cpu_suspend(u32 state, int (*entry_point)(unsigned long state))
{
	return 0;
}
static void cpu_resume(void) {
}

void *arm_pm_restart;
static inline int get_logical_index(unsigned int cpu)
{
	return 0;
}

#define MPIDR_HWID_BITMASK	0
#else
static inline int arm_cpuidle_init(unsigned int cpu)
{
	return -EOPNOTSUPP;
}

static inline int arm_cpuidle_suspend(int index)
{
	return -EOPNOTSUPP;
}
#endif
#endif
