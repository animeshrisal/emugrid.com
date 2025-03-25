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
  int func3 = (instr >> 12) & 0x7;
  int func7 = (instr >> 25) & 0x7f;
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
  int32 imm = (int32)(instr >> 20);
  if (imm & 0x800) {
    imm |= 0xFFFFF000;
  }

  printf("%d", func3);
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
    regs[rd] = load(cpu->bus, imm, 8);
    break;
  case LH:
    regs[rd] = load(cpu->bus, imm, 16);
    break;
  case LW:
    regs[rd] = load(cpu->bus, imm, 32);
    break;
  case LBU:
    regs[rd] = load(cpu->bus, imm, 32);
    break;
  case LHU:
    regs[rd] = load(cpu->bus, imm, 32);
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
    store(cpu->bus, addr, value, 8);
    break;
  case SH:
    store(cpu->bus, addr, value, 16);
    break;
  case SW:
    store(cpu->bus, addr, value, 32);
    break;
  default:
    printf("Illegal store instruction\n");
    break;
  }
}

void run_u_instructions(CPU *cpu, uint32 instr) {
  int opcode = instr & 0x7F;
  int rd = rd(instr);
  int32 imm = (uint64)(int64)(int32)(instr & 0xFFFFF000);
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

void run_ecall_instruction(CPU *cpu) { cpu->exception = CAUSE_USER_ECALL; }

void exec_jal(CPU *cpu, uint32 instr) {
  int32 imm = ((instr & 0x80000000) ? 0xFFF00000 : 0) |
              ((instr >> 21) & 0x3FF) | ((instr >> 20) & 0x1) << 10 |
              ((instr >> 12) & 0xFF) << 11;
  int rd = rd(instr);
  cpu->x[rd] = cpu->pc;
  cpu->pc = cpu->pc + (int64)imm - 4;
}

void exec_jalr(CPU *cpu, uint32 instr) {
  int rs1 = rs1(instr);
  int rd = rd(instr);
  int imm = ((int32)instr >> 20);
  uint64 pc = (cpu->x[rs1] + imm) & 0xffffffe;
  cpu->x[rd] = cpu->pc;
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
    break;
  case I:
    run_i_instructions(cpu, instr);
    break;
  case L:
    run_l_instructions(cpu, instr);
    break;
  case S:
    run_s_instructions(cpu, instr);
    break;
  case B:
    run_b_instructions(cpu, instr);
    break;
  case LUI:
  case AUI:
    run_u_instructions(cpu, instr);
    break;
  case CSR:
    run_csr_instructions(cpu, instr);
    break;
  case JAL:
  case JALR:
    run_j_instructions(cpu, instr);
  default:
    break;
  }
  cpu->pc += 4;
}

uint64 read_mstatus(CPU *cpu, uint8 shift) {
  return (load_csr(cpu, MSTATUS) >> shift) & 0x1f;
}

void write_mstatus(CPU *cpu, uint64 value, uint64 shift) {
  uint64 old_mstatus = load_csr(cpu, MSTATUS) | shift;
}

uint64 read_sstatus(CPU *cpu, uint8 shift) {
  return (load_csr(cpu, SSTATUS) >> shift) & 0x1f;
}

void write_sstatus(CPU *cpu, uint64 value, uint64 shift) {
  uint64 sstatus = load_csr(cpu, SSTATUS) | shift;
}

void run_mret_instruction(CPU *cpu) {
  cpu->pc = (uint64)(load_csr(cpu, MEPC));
  cpu->mode = read_mstatus(cpu, MSTATUS_MPP);
}

void run_sret_instruction(CPU *cpu) {
  cpu->pc = (uint64)(load_csr(cpu, SEPC));
  cpu->mode = read_sstatus(cpu, MSTATUS_SPP);
}

void take_trap(CPU *cpu) {
  int exception_pc = cpu->pc;
  int prev_mode = cpu->mode;
  int cause = cpu->exception;

  if (prev_mode == SUPERVISOR && ((load_csr(cpu, MEDELEG) >> cause) & 1) == 1) {
    cpu->pc = (uint64)(load_csr(cpu, STVEC) & !1);
    cpu->mode = SUPERVISOR;
    store_csr(cpu, SEPC, exception_pc & !1);
    store_csr(cpu, SCAUSE, cause);
    store_csr(cpu, STVAL, 0);

    switch (prev_mode) {

    case USER:
      write_sstatus(cpu, USER, MSTATUS_SPP_SHIFT);
      break;
    case SUPERVISOR:
      write_sstatus(cpu, SUPERVISOR, MSTATUS_SPP_SHIFT);
      break;
    default:
      printf("Previous privilege mode is invalid");
    }
  } else {
    cpu->mode = MACHINE;
    cpu->pc = (uint64)(load_csr(cpu, MTVEC));
    store_csr(cpu, MEPC, exception_pc);
    store_csr(cpu, MCAUSE, cause);
    store_csr(cpu, MTVAL, 0);

    switch (prev_mode) {
    case USER:
      write_sstatus(cpu, USER, MSTATUS_MPP_SHIFT);
      break;
    case SUPERVISOR:
      write_sstatus(cpu, SUPERVISOR, MSTATUS_MPP_SHIFT);
      break;
    case MACHINE:
      write_sstatus(cpu, MACHINE, MSTATUS_MPP_SHIFT);
      break;
    default:
      printf("Previous privilege mode is invalid");
    }
  }
}

void run_csr_instructions(CPU *cpu, uint32 instr) {

  int opcode = instr & 0x7f;
  int rd = rd(instr);
  int rs1 = rs1(instr);
  int func3 = (instr >> 12) & 0x7;
  int csr = (instr >> 20) & 0xFFF;
  uint32 rs1_value = cpu->x[rs1];
  uint32 csr_value = cpu->csr[csr];

  if (func3 == 0) {
    run_ecall_instruction(cpu);
  } else {
    switch (func3) {
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
    case MRET:
      run_mret_instruction(cpu);
    case SRET:
      run_sret_instruction(cpu);
    default:
      break;
    }
  }
}
