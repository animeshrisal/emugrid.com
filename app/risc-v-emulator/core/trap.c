#include "bus.h"
#include "cpu.h"

void take_trap(CPU *cpu, int cause) {
  int prev_mode = cpu->mode;
  int exception_pc = cpu->pc + 4;

  unsigned int mstatus = load_csr(cpu, MSTATUS);
  if (prev_mode == SUPERVISOR) {
    // to do
  } else {
    cpu->mode = MACHINE;
    cpu->pc = load_csr(cpu, MTVEC) & !1;

    store_csr(cpu, MEPC, exception_pc & !1);
    store_csr(cpu, MCAUSE, cause);
    store_csr(cpu, MTVAL, 0);
  }
}
