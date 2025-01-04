#include "cpu.h"
#include "bus.h"
#include "types.h"
#include <stdio.h>

void create_cpu(CPU *cpu) {
  for (int i = 0; i < 32; i++) {
    cpu->riscv_register[i] = 0;
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
  uint64 *regs = cpu->riscv_register;
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

    regs[rd] = (regs[rs1] < regs[rs2]) ? 1 : 0; // Set Less Than
    break;

  case SLTU:
    regs[rd] = (regs[rs1] < regs[rs2]) ? 1 : 0; // Set Less Than Unsigned
    break;

  case XOR:
    regs[rd] = regs[rs1] ^ regs[rs2];
    break;
  case SRL:
    if (func7 == SRL) {
      regs[rd] = regs[rs1] >> regs[rs2]; // Shift Right Logical
    } else if (func7 == SRA) {
      regs[rd] = (int)regs[rs1] >> regs[rs2]; // Shift Right Arithmetic
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
  int func3 = (instr >> 12) & 0x7; // func3 (3 bits)

  int rd = rd(instr);
  int rs1 = rs1(instr);

  //
  int32 imm = (int32)(instr >> 20);
  if (imm & 0x800) {
    imm |= 0xFFFFF000;
  }

  uint64 *regs = cpu->riscv_register;
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
    if ((imm >> 10) & 1) { // SRAI: Shift right arithmetic immediate
      regs[rd] = (int32)regs[rs1] >> (imm & 0x1F); // Sign-extend shift
    } else { // SRLI: Shift right logical immediate
      regs[rd] = (uint32)regs[rs1] >> (imm & 0x1F); // Zero-extend shift
    }
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
  int32 imm = (int32)(instr >> 20); // Extract imm[11:0] (12 bits)
  if (imm & 0x800) {                // If the sign bit (bit 11) is set,
    imm |= 0xFFFFF000;              // sign-extend to 32 bits
  }

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

  uint64 addr = cpu->riscv_register[rs1] + imm;
  uint64 *regs = cpu->riscv_register;

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

  uint64 addr = cpu->riscv_register[rs1] + imm;
  uint64 value = cpu->riscv_register[rs2];

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

  uint64 *regs = cpu->riscv_register;

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
  default:
    printf("Illegal instruction!!!\n");
    break;
  }
}
