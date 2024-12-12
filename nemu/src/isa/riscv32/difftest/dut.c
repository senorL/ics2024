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
  ref_r->sr[mstatus] = 0x1800;
  bool tmp = true;
  for (int i = 0; i < 32; i++) {
    if (ref_r->gpr[i] != cpu.gpr[i]){
      tmp = false;
      printf("ref_gpr %d: 0x%08x\n", i, ref_r->gpr[i]);
      break;
    }
  }
  if (ref_r->pc != cpu.pc) {
    tmp = false;
  }
  if (tmp == false) {
    printf("pc: 0x%08x\n", pc);
    printf("ref_pc: 0x%08x\n", ref_r->pc);
  }
  return tmp;
}

void isa_difftest_attach() {
}
