#include "cpu.h"
#include "types.h"
#include <stdint.h>
#include <stdio.h>

char *disassemble_instruction(uint32 instr) {
  char output[50];
  int opcode = instr & 0x7f;
  int func3 = (instr >> 12) & 0x7;
  int func7 = (instr >> 25) & 0x7f;
  int rd = rd(instr);
  int rs1 = rs1(instr);
  int rs2 = rs2(instr);
  int32 imm = (int32)(instr >> 20);

  if (imm & 0x800) {
    imm |= 0xFFFFF000;
  }

  switch (opcode) {
  case R:
    switch (func3) {
    case ADDSUB:
      if (func7 == ADD) {
        sprintf(output, "add x%d, x%d, x%d", rd, rs1, rs2);
      } else if (func7 == SUB) {
        sprintf(output, "sub x%d, x%d, x%d", rd, rs1, rs2);
      }
      break;
    case SLL:
      sprintf(output, "sll x%d, x%d, x%d", rd, rs1, rs2);
      break;
    case SLT:
      sprintf(output, "slt x%d, x%d, x%d", rd, rs1, rs2);
      break;
    case SLTU:
      sprintf(output, "sltu x%d, x%d, x%d", rd, rs1, rs2);
      break;
    case XOR:
      sprintf(output, "xor x%d, x%d, x%d", rd, rs1, rs2);
      break;
    case SRL:
      if (func7 == SRL) {
        sprintf(output, "srl x%d, x%d, x%d", rd, rs1, rs2);
      } else if (func7 == SRA) {
        sprintf(output, "sra x%d, x%d, x%d", rd, rs1, rs2);
      }
      break;
    case OR:
      sprintf(output, "or x%d, x%d, x%d", rd, rs1, rs2);
      break;
    case AND:
      sprintf(output, "and x%d, x%d, x%d", rd, rs1, rs2);
      break;
    default:
      sprintf(output, "Unknown R-type instruction");
      break;
    }
    break;

  case I:
    switch (func3) {
    case ADD:
      sprintf(output, "addi x%d, x%d, %d\n", rd, rs1, imm);
      break;
    case XOR:
      sprintf(output, "xori x%d, x%d, %d", rd, rs1, imm);
      break;
    case OR:
      sprintf(output, "ori x%d, x%d, %d", rd, rs1, imm);
      break;
    case AND:
      sprintf(output, "andi x%d, x%d, %d", rd, rs1, imm);
      break;
    case SLL:
      sprintf(output, "slli x%d, x%d, %d", rd, rs1, imm & 0x1F);
      break;
    case SRL:
      if ((imm >> 10) & 1) {
        sprintf(output, "srai x%d, x%d, %d", rd, rs1, imm & 0x1F);
      } else {
        sprintf(output, "srli x%d, x%d, %d", rd, rs1, imm & 0x1F);
      }
      break;
    case SLT:
      sprintf(output, "slti x%d, x%d, %d", rd, rs1, imm);
      break;
    case SLTU:
      sprintf(output, "sltiu x%d, x%d, %u", rd, rs1, imm);
      break;
    default:
      sprintf(output, "Unknown I-type instruction");
      break;
    }
    break;

  case L:
    switch (func3) {
    case LB:
      sprintf(output, "lb x%d, %d(x%d)", rd, imm, rs1);
      break;
    case LH:
      sprintf(output, "lh x%d, %d(x%d)", rd, imm, rs1);
      break;
    case LW:
      sprintf(output, "lw x%d, %d(x%d)", rd, imm, rs1);
      break;
    case LBU:
      sprintf(output, "lbu x%d, %d(x%d)", rd, imm, rs1);
      break;
    case LHU:
      sprintf(output, "lhu x%d, %d(x%d)", rd, imm, rs1);
      break;
    default:
      sprintf(output, "Unknown L-type instruction");
      break;
    }
    break;

  case S:
    switch (func3) {
    case SB:
      sprintf(output, "sb x%d, %d(x%d)", rs2, imm, rs1);
      break;
    case SH:
      sprintf(output, "sh x%d, %d(x%d)", rs2, imm, rs1);
      break;
    case SW:
      sprintf(output, "sw x%d, %d(x%d)", rs2, imm, rs1);
      break;
    default:
      sprintf(output, "Unknown S-type instruction");
      break;
    }
    break;

  case B:
    imm = ((int32)(instr & 0x80000000) >> 19) | ((instr & 0x80) << 4) |
          ((instr >> 20) & 0x7e0) | ((instr >> 7) & 0x1e);
    switch (func3) {
    case BEQ:
      sprintf(output, "beq x%d, x%d, %d", rs1, rs2, imm);
      break;
    case BNE:
      sprintf(output, "bne x%d, x%d, %d", rs1, rs2, imm);
      break;
    case BLT:
      sprintf(output, "blt x%d, x%d, %d", rs1, rs2, imm);
      break;
    case BGE:
      sprintf(output, "bge x%d, x%d, %d", rs1, rs2, imm);
      break;
    case BLTU:
      sprintf(output, "bltu x%d, x%d, %d", rs1, rs2, imm);
      break;
    case BGEU:
      sprintf(output, "bgeu x%d, x%d, %d", rs1, rs2, imm);
      break;
    default:
      sprintf(output, "Unknown B-type instruction");
      break;
    }
    break;

  case LUI:
    sprintf(output, "lui x%d, %d", rd, imm);
    break;

  case AUI:
    sprintf(output, "auipc x%d, %d", rd, imm);
    break;

  default:
    sprintf(output, "Unknown instruction");
    break;
  }

  return output;
}
