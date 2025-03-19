#ifndef CPU_H
#define CPU_H

#define R 0b0110011
#define I 0b0010011
#define L 0b00000011
#define S 0b0100011
#define B 0b1100011
#define JAL 0b1101111
#define JALR 0b1100111
#define LUI 0b0110111
#define AUI 0b0010111

#define CSR 0b1110011
#define FENCE 0b0001111

#define MRET 0b0001000
#define SRET 0b0011000
#define WFI 0b0001000
#define SFENCE 0b0001001

#define ADDSUB 0x0
#define ADD 0x00
#define SUB 0x20
#define SLL 0x1
#define SLT 0x2
#define SLTU 0x3
#define XOR 0x4
#define SRL 0x5
#define SRA 0x20
#define OR 0x6
#define AND 0x7

#define BEQ 0x0
#define BNE 0x1
#define BLT 0x4
#define BGE 0x5
#define BLTU 0x6
#define BGEU 0x7

#define SB 0x0
#define SH 0x1
#define SW 0x2

#define LB 0x0
#define LH 0x1
#define LW 0x2
#define LBU 0x4
#define LHU 0x5

#define CSRRW 0x0
#define CSRRS 0x1
#define CSRRC 0x2
#define CSRRWI 0x3
#define CSRRSI 0x4
#define CSRRCI 0x5

#define rd(v) (v >> 7) & 0x1f
#define rs1(v) (v >> 15) & 0x1f
#define rs2(v) (v >> 20) & 0x1f

#include "bus.h"

enum Mode { USER = 0x00, SUPERVISOR = 0x01, MACHINE = 0x11 };

typedef struct CPU {
  uint64 x[32];
  uint64 pc;
  uint64 mepc;
  uint64 spec;
  uint32 csr[4096];
  Mode mode;
  Bus *bus;
} CPU;

void create_cpu(CPU *cpu);
void run_instruction(CPU *cpu, uint32 instr);
void reset_cpu(CPU *cpu);
void run_r_instructions(CPU *cpu, uint32 instr);
void run_i_instructions(CPU *cpu, uint32 instr);
void run_l_instructions(CPU *cpu, uint32 instr);
void run_s_instructions(CPU *cpu, uint32 instr);
void rin_b_instructions(CPU *cpu, uint32 instr);
void run_csr_instructions(CPU *cpu, uint32 instr);
void run_priviledge_mode(CPU *cpu, uint32 instr);
uint32 cpu_fetch_instruction(Bus *bus);
#endif
