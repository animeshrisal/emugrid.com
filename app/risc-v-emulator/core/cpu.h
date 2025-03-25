#ifndef CPU_H
#define CPU_H

#define CAUSE_MISALIGNED_FETCH 0x0
#define CAUSE_FAULT_FETCH 0x1
#define CAUSE_ILLEGAL_INSTRUCTION 0x2
#define CAUSE_BREAKPOINT 0x3
#define CAUSE_MISALIGNED_LOAD 0x4
#define CAUSE_FAULT_LOAD 0x5
#define CAUSE_MISALIGNED_STORE 0x6
#define CAUSE_FAULT_STORE 0x7
#define CAUSE_USER_ECALL 0x8
#define CAUSE_SUPERVISOR_ECALL 0x9
#define CAUSE_HYPERVISOR_ECALL 0xA
#define CAUSE_MACHINE_ECALL 0xB
#define CAUSE_FETCH_PAGE_FAULT 0xC
#define CAUSE_LOAD_PAGE_FAULT 0xD
#define CAUSE_STORE_PAGE_FAULT 0xF
#define CAUSE_INTERRUPT ((uint32)1 << 31)

#define MSTATUS_SPP (3 << 11)
#define MSTATUS_MPP (1 << 8)

/* Instruction Types */
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
#define ECALL 0b1110011
#define FENCE 0b0001111

/* Operation Codes */
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

/* Branch Conditions */
#define BEQ 0x0
#define BNE 0x1
#define BLT 0x4
#define BGE 0x5
#define BLTU 0x6
#define BGEU 0x7

/* Store Operations */
#define SB 0x0
#define SH 0x1
#define SW 0x2

/* Load Operations */
#define LB 0x0
#define LH 0x1
#define LW 0x2
#define LBU 0x4
#define LHU 0x5

/* CSR Instructions */
#define CSRRW 0x0
#define CSRRS 0x1
#define CSRRC 0x2
#define CSRRWI 0x3
#define CSRRSI 0x4
#define CSRRCI 0x5

/* Registers */
#define rd(v) ((v >> 7) & 0x1f)
#define rs1(v) ((v >> 15) & 0x1f)
#define rs2(v) ((v >> 20) & 0x1f)

/* Constants */
#define XLEN 64
#define MTIME_ADDR 0x40000000
#define MTIMECMP_ADDR 0x40000008
#define UART_TX_ADDR 0x40002000

/* CSR Addresses */
#define MHARTID 0xf14
#define MSTATUS 0x300
#define MEDELEG 0x302
#define MIDELEG 0x303
#define MIE 0x304
#define MTVEC 0x305
#define MCOUNTEREN 0x306
#define MSCRATCH 0x340
#define MEPC 0x341
#define MCAUSE 0x342
#define MTVAL 0x343
#define MIP 0x344
#define SSTATUS 0x100
#define SIE 0x104
#define STVEC 0x105
#define SSCRATCH 0x140
#define SEPC 0x141
#define SCAUSE 0x142
#define STVAL 0x143
#define SIP 0x144
#define SATP 0x180

#define MRET 0b0011000
#define SRET 0b0001000

#include "bus.h"

enum Mode { USER = 0x00, SUPERVISOR = 0x01, MACHINE = 0x11 };

typedef struct CPU {
  uint64 x[32];
  uint64 pc;
  uint32 csr[4096];
  uint64 mepc;
  uint64 sepc;
  enum Mode mode;
  Bus *bus;
  uint64 exception;
} CPU;

void create_cpu(CPU *cpu);
void run_instruction(CPU *cpu, uint32 instr);
void reset_cpu(CPU *cpu);
void run_r_instructions(CPU *cpu, uint32 instr);
void run_i_instructions(CPU *cpu, uint32 instr);
void run_l_instructions(CPU *cpu, uint32 instr);
void run_s_instructions(CPU *cpu, uint32 instr);
void run_b_instructions(CPU *cpu, uint32 instr);
void run_j_instructions(CPU *cpu, uint32 instr);
void run_csr_instructions(CPU *cpu, uint32 instr);
void run_priviledge_mode(CPU *cpu, uint32 instr);
void run_ecall_instructions(CPU *cpiu, uint32 instr);
uint32 cpu_fetch_instruction(Bus *bus);
uint64 load_csr(CPU *cpu, uint32 address);
void store_csr(CPU *cpu, uint32 address, uint64 value);
void take_trap(CPU *cpu);
#endif
