#include "cpu.h"
#include "bus.h"
#include "types.h"
#include <stdio.h>

uint64 mtval;
uint64 stval;
uint64 mepc;
uint64 sepc;
uint64 mscratch;
uint64 sscratch;
uint64 mcause;
uint64 scause;
uint64 mtvec;
uint64 stvec;

void create_cpu(CPU *cpu) {
  for (int i = 0; i < 32; i++) {
    cpu->x[i] = 0;
  }
  cpu->pc = DRAM_BASE;
  for (int i = 0; i < 4096; i++) {
    cpu->csr[i] = 0;
  }
};

void reset_cpu(CPU *cpu) { create_cpu(cpu); }

void run_r_instructions(CPU *cpu, uint32 instr) {
  int func3 = (instr >> 12) & 0x7;  // func3 (3 bits)
  int func7 = (instr >> 25) & 0x7f; // func7 (7 bits)
  int rd = rd(instr);
  int rs1 = rs1(instr);
  int rs2 = rs2(instr);
  uint64 *regs = cpu->x;
  switch (func3) {
  case ADDSUB:
    switch (func7) {
    case ADD:
      regs[rd] = regs[rs1] + regs[rs2];
      break;
    case SUB:
      regs[rd] = regs[rs1] - regs[rs2];
      break;
    }
    break;

  case SLL:
    regs[rd] = regs[rs1] << regs[rs2];
    break;

  case SLT:

    regs[rd] = (regs[rs1] < regs[rs2]) ? 1 : 0;
    break;

  case SLTU:
    regs[rd] = (regs[rs1] < regs[rs2]) ? 1 : 0;
    break;

  case XOR:
    regs[rd] = regs[rs1] ^ regs[rs2];
    break;
  case SRL:
    if (func7 == SRL) {
      regs[rd] = regs[rs1] >> regs[rs2];
    } else if (func7 == SRA) {
      regs[rd] = (int)regs[rs1] >> regs[rs2];
    }
    break;

  case OR:
    regs[rd] = regs[rs1] | regs[rs2];
    break;

  case AND:
    regs[rd] = regs[rs1] & regs[rs2];
    break;

  default:
    break;
  }
}

void run_i_instructions(CPU *cpu, uint32 instr) {
  int func3 = (instr >> 12) & 0x7;
  int rd = rd(instr);
  int rs1 = rs1(instr);

  //
  int32 imm = (int32)(instr >> 20);
  if (imm & 0x800) {
    imm |= 0xFFFFF000;
  }

  uint64 *regs = cpu->x;
  switch (func3) {
  case ADD:
    regs[rd] = regs[rs1] + imm;
    break;
  case XOR:
    regs[rd] = regs[rs1] ^ imm;
    break;
  case OR:
    regs[rd] = regs[rs1] | (imm & 0x1F);
    break;
  case AND:
    regs[rd] = regs[rs1] & imm;
    break;
  case SLL:
    regs[rd] = regs[rs1] << (imm & 0x1F);
    break;
  case SRL:
    if ((imm >> 10) & 1) {
      regs[rd] = (int32)regs[rs1] >> (imm & 0x1F);
    } else {
      regs[rd] = (uint32)regs[rs1] >> (imm & 0x1F);
    };
    break;

  case SRA:

    break;

  case SLT:
    regs[rd] = regs[rs1] < imm ? 1 : 0;
    break;
  case SLTU:
    regs[rd] = regs[rs1] < (uint32)imm ? 1 : 0;
    break;
  default:
    break;
  }
}

void run_b_instructions(CPU *cpu, uint32 instr) {
  int rs1 = rs1(instr);
  int rs2 = rs2(instr);
  int32 imm = ((int32)(instr & 0x80000000) >> 19) | ((instr & 0x80) << 4) |
              ((instr >> 20) & 0x7e0) | ((instr >> 7) & 0x1e);

  int func3 = (instr >> 12) & 0x7; // func3 (3 bit
  //
  switch (func3) {
  case BEQ:
    if (rs1 == rs2) {
      cpu->pc += imm;
    };

    break;
  case BNE:
    if (rs1 != rs2) {
      cpu->pc += imm;
    }
    break;
  case BLT:
    if (rs1 < rs2) {
      cpu->pc += imm;
    }

    break;
  case BGE:
    if (rs1 >= rs2) {
      cpu->pc += imm;
    }

    break;
  case BLTU:
    if (rs1 < rs2) {
      cpu->pc += imm;
    }

    break;
  case BGEU:
    if (rs1 >= rs2) {
      cpu->pc += imm;
    }

    break;
  default:
    break;
  }

  cpu->pc -= 0x4;
}

void run_l_instructions(CPU *cpu, uint32 instr) {
  int func3 = (instr >> 12) & 0x7;
  int rd = rd(instr);
  int rs1 = rs1(instr);
  int32 imm = (int32)(instr >> 20);

  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Sign-extend immediate
  }

  uint64 addr = cpu->x[rs1] + imm;
  uint64 *regs = cpu->x;

  switch (func3) {
  case LB:
    regs[rd] = (int8)bus_read8(cpu->bus, addr); // Load Byte, sign-extended
    break;
  case LH:
    regs[rd] = (int16)bus_read16(cpu->bus, addr); // Load Half, sign-extended
    break;
  case LW:
    regs[rd] = (int32)bus_read32(cpu->bus, addr); // Load Word, sign-extended
    break;
  case LBU:
    regs[rd] = bus_read8(cpu->bus, addr); // Load Byte, zero-extended
    break;
  case LHU:
    regs[rd] = bus_read16(cpu->bus, addr); // Load Half, zero-extended
    break;
  default:
    printf("Illegal load instruction\n");
    break;
  }
}

void run_s_instructions(CPU *cpu, uint32 instr) {
  int func3 = (instr >> 12) & 0x7;
  int rs1 = rs1(instr);
  int rs2 = rs2(instr);

  int imm1 = (instr >> 7) & 0x1F;  // imm[4:0]
  int imm2 = (instr >> 25) & 0x7F; // imm[11:5]
  int32 imm = (imm2 << 5) | imm1;  // Concatenate for full 12-bit immediate

  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Sign-extend immediate
  }

  uint64 addr = cpu->x[rs1] + imm;
  uint64 value = cpu->x[rs2];

  switch (func3) {
  case SB:
    bus_write8(cpu->bus, addr, (uint8)value); // Store Byte
    break;
  case SH:
    bus_write16(cpu->bus, addr, (uint16)value); // Store Half
    break;
  case SW:
    bus_write32(cpu->bus, addr, (uint32)value); // Store Word
    break;
  default:
    printf("Illegal store instruction\n");
    break;
  }
}

void run_u_instructions(CPU *cpu, uint32 instr) {
  int opcode = instr & 0x7F;
  int rd = rd(instr);
  int32 imm = instr & 0xFFFFF000; // Upper 20 bits for U-type immediate

  uint64 *regs = cpu->x;

  switch (opcode) {
  case LUI:
    regs[rd] = imm; // Load Upper Immediate
    break;
  case AUI:
    regs[rd] = cpu->pc + imm; // AUIPC: Add Upper Immediate to PC
    break;
  default:
    printf("Illegal U-type instruction\n");
    break;
  }
}

uint64 load_csr(CPU *cpu, uint32 address) { return cpu->csr[address]; }

void store_csr(CPU *cpu, uint32 address, uint64 value) {
  cpu->csr[address] = value;
}

void run_privileged_mode(CPU *cpu, uint32 instr) {
  int opcode = instr & 0x7f;
  int priv = (instr >> 25) & 0x7f; // Correct masking and shifting

  switch (priv) {
  case MRET:
    cpu->pc = cpu->mepc;

    unsigned int mstatus = load_csr(cpu, MSTATUS);

    if ((mstatus >> MPIE_BIT) & 1) {
      mstatus |= (1 << MIE_BIT);
    } else {
      mstatus &= ~(1 << MIE_BIT);
    }

    mstatus |= (1 << MPIE_BIT);

    mstatus &= ~(0b11 << 11);

    store_csr(cpu, MSTATUS, mstatus);
    cpu->mode = (mstatus >> 11) & 0b11;
    break;

  case SRET:
    cpu->pc = cpu->sepc;
    unsigned int sstatus = load_csr(cpu, (uint8)SSTATUS);

    if ((sstatus >> SPIE_BIT) & 1) {
      sstatus |= (1 << SIE_BIT);
    } else {
      sstatus &= ~(1 << SIE_BIT);
    }

    sstatus |= (1 << SPIE_BIT);

    sstatus &= ~(1 << 8);

    store_csr(cpu, SSTATUS, sstatus);
    cpu->mode = (sstatus >> 8) & 1 ? SUPERVISOR : USER;
    break;

  default:
    printf("Illegal privilege instruction\n");
    break;
  }
}

void run_csr_instructions(CPU *cpu, uint32 instr) {

  int opcode = instr & 0x7f;
  int rd = rd(instr);
  int rs1 = rs1(instr);
  int csr = (instr >> 20) & 0xFFF;
  uint32 rs1_value = cpu->x[rs1];
  uint32 csr_value = cpu->csr[csr];

  switch (opcode) {
  case CSRRW:
    cpu->x[rd] = csr_value;
    cpu->csr[csr] = rs1_value;
    break;

  case CSRRS:
    cpu->x[rd] = csr_value;
    cpu->csr[csr] |= rs1_value;
    break;
  case CSRRC:
    cpu->x[rd] = csr_value;
    cpu->csr[csr] &= ~rs1_value;
    break;
  case CSRRWI:
    cpu->x[rd] = csr_value;
    cpu->csr[csr] = rs1 & 0x1F;
    break;
  case CSRRSI:
    cpu->x[rd] = csr_value;
    cpu->csr[csr] |= rs1 & 0x1F;
    break;
  case CSRRCI:
    cpu->x[rd] = csr_value;
    cpu->csr[csr] &= ~(rs1 & 0x1F);
    break;

  default:
    break;
  }
}

void exec_jal(CPU *cpu, uint32 instr) {
  int32 imm =
      ((instr & 0x80000000) ? 0xFFF00000 : 0) | // imm[20] (sign extension)
      ((instr >> 21) & 0x3FF) |                 // imm[10:1]
      ((instr >> 20) & 0x1) << 10 |             // imm[11]
      ((instr >> 12) & 0xFF) << 11;             // imm[19:12]

  int rd = rd(instr);
  cpu->x[rd] = cpu->pc;
  cpu->pc = cpu->pc + (int64)imm - 4;
}

void exec_jalr(CPU *cpu, uint32 instr) {
  int rs1 = rs1(instr);
  int rd = rd(instr);
  int imm = ((int32)instr >> 20);
  uint64 pc = (cpu->x[rs1] + imm) & 0xffffffe;
  cpu->x[rd] = cpu->pc + 4;
  cpu->pc = pc;
}

void run_j_instructions(CPU *cpu, uint32 instr) {
  int rd = rd(instr);

  int opcode = instr & 0x7f;
  int rs1 = rs1(instr);
  switch (opcode) {
  case JAL:
    exec_jal(cpu, instr);
    break;

  case JALR:
    exec_jalr(cpu, instr);
    break;
  }
}

void run_instruction(CPU *cpu, uint32 instr) {
  int opcode = instr & 0x7f;
  switch (opcode) {
  case R:
    run_r_instructions(cpu, instr);
    cpu->pc += 4;
    break;
  case I:
    run_i_instructions(cpu, instr);
    cpu->pc += 4;
    break;
  case L:
    run_l_instructions(cpu, instr);
    cpu->pc += 4;
    break;
  case S:
    run_s_instructions(cpu, instr);
    cpu->pc += 4;
    break;
  case B:
    run_b_instructions(cpu, instr);
    cpu->pc += 4;
    break;
  case LUI:
  case AUI:
    run_u_instructions(cpu, instr);
    cpu->pc += 4;
    break;
  case CSR:
    run_csr_instructions(cpu, instr);
    cpu->pc += 4;
    break;
  case JAL:
  case JALR:
    run_j_instructions(cpu, instr);
  default:
    break;
  }
}

bool check_pending_interrupts(CPU *cpu) {

  int irq;
  switch (cpu->mode) {
  case SUPERVISOR:
    if ((load_csr(cpu, MSTATUS) >> 3) & 1 == 0) {
      return false;
    }
    break;
  case MACHINE:
    if ((load_csr(cpu, MSTATUS) >> 1) & 1 == 0) {
      return false;
    }
    break;
  default:
    printf("Illegal instruction!");
    break;
  }

  if (cpu->bus->uart->is_interrupting) {
    irq = UART_IRQ;
  }
}

void take_interrupt_trap(CPU *cpu) {
  int exception_pc = cpu->pc;
  int prev_mode = cpu->mode;
  int cause;
}

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

    if ((mstatus >> MPIE_BIT) & 1) {
      mstatus |= (1 << MIE_BIT);
    } else {
      mstatus &= ~(1 << MIE_BIT);
    }

    store_csr(cpu, MSTATUS, mstatus & !(1 << 3));
    store_csr(cpu, MSTATUS, mstatus & !(0b11 << 11));
  }
}

void run_ecall() {}

void take_interrupt_traps(CPU *cpu) {}
