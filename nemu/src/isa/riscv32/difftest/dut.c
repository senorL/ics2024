/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for (int i = 0; i < 32; i++) {
    if (ref_r->gpr[i] != cpu.gpr[i]) {
      printf("ref_gpr %d: 0x%08x\n", i, ref_r->gpr[i]);
      printf("cpu_gpr %d: 0x%08x\n", i, cpu.gpr[i]);
      return false;
    }
  }

  if (ref_r->pc != cpu.pc) {
    printf("pc: 0x%08x\n", pc);
    printf("ref_pc: 0x%08x\n", ref_r->pc);
    printf("cpu_pc: 0x%08x\n", cpu.pc);
    return false;
  }

  if (ref_r->sr[mstatus] != cpu.sr[mstatus]) {
    printf("ref_mstatus: 0x%08x\n", ref_r->sr[mstatus]);
    printf("cpu_mstatus: 0x%08x\n", cpu.sr[mstatus]);
    return false;
  }

  if (ref_r->sr[mcause] != cpu.sr[mcause]) {
    printf("ref_mcause: 0x%08x\n", ref_r->sr[mcause]);
    printf("cpu_mcause: 0x%08x\n", cpu.sr[mcause]);
    return false;
  }

  if (ref_r->sr[mepc] != cpu.sr[mepc]) {
    printf("ref_mepc: 0x%08x\n", ref_r->sr[mepc]);
    printf("cpu_mepc: 0x%08x\n", cpu.sr[mepc]);
    return false;
  }

  if (ref_r->sr[mtvec] != cpu.sr[mtvec]) {
    printf("ref_mtvec: 0x%08x\n", ref_r->sr[mtvec]);
    printf("cpu_mtvec: 0x%08x\n", cpu.sr[mtvec]);
    return false;
  }

  return true;
}
void isa_difftest_attach() {
}
