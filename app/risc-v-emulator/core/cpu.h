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
#define CAUSE_HYPERVISOR_ECALL 0xa
#define CAUSE_MACHINE_ECALL 0xb
#define CAUSE_FETCH_PAGE_FAULT 0xc
#define CAUSE_LOAD_PAGE_FAULT 0xd
#define CAUSE_STORE_PAGE_FAULT 0xf
#define CAUSE_INTERRUPT ((uint32)1 << 31)

/* misa CSR */
#define MCPUID_SUPER (1 << ('S' - 'A'))
#define MCPUID_USER (1 << ('U' - 'A'))
#define MCPUID_I (1 << ('I' - 'A'))
#define MCPUID_M (1 << ('M' - 'A'))
#define MCPUID_A (1 << ('A' - 'A'))
#define MCPUID_F (1 << ('F' - 'A'))
#define MCPUID_D (1 << ('D' - 'A'))
#define MCPUID_Q (1 << ('Q' - 'A'))
#define MCPUID_C (1 << ('C' - 'A'))

#define MIP_USIP (1 << 0)
#define MIP_SSIP (1 << 1)
#define MIP_HSIP (1 << 2)
#define MIP_MSIP (1 << 3)
#define MIP_UTIP (1 << 4)
#define MIP_STIP (1 << 5)
#define MIP_HTIP (1 << 6)
#define MIP_MTIP (1 << 7)
#define MIP_UEIP (1 << 8)
#define MIP_SEIP (1 << 9)
#define MIP_HEIP (1 << 10)
#define MIP_MEIP (1 << 11)

/* mstatus CSR */

#define MSTATUS_SPIE_SHIFT 5
#define MSTATUS_MPIE_SHIFT 7
#define MSTATUS_SPP_SHIFT 8
#define MSTATUS_MPP_SHIFT 11
#define MSTATUS_FS_SHIFT 13
#define MSTATUS_UXL_SHIFT 32
#define MSTATUS_SXL_SHIFT 34

#define MSTATUS_UIE (1 << 0)
#define MSTATUS_SIE (1 << 1)
#define MSTATUS_HIE (1 << 2)
#define MSTATUS_MIE (1 << 3)
#define MSTATUS_UPIE (1 << 4)
#define MSTATUS_SPIE (1 << MSTATUS_SPIE_SHIFT)
#define MSTATUS_HPIE (1 << 6)
#define MSTATUS_MPIE (1 << MSTATUS_MPIE_SHIFT)
#define MSTATUS_SPP (1 << MSTATUS_SPP_SHIFT)
#define MSTATUS_HPP (3 << 9)
#define MSTATUS_MPP (3 << MSTATUS_MPP_SHIFT)
#define MSTATUS_FS (3 << MSTATUS_FS_SHIFT)
#define MSTATUS_XS (3 << 15)
#define MSTATUS_MPRV (1 << 17)
#define MSTATUS_SUM (1 << 18)
#define MSTATUS_MXR (1 << 19)
#define MSTATUS_UXL_MASK ((uint64)3 << MSTATUS_UXL_SHIFT)
#define MSTATUS_SXL_MASK ((uint64)3 << MSTATUS_SXL_SHIFT)

#define SSTATUS_MASK0                                                          \
  (MSTATUS_UIE | MSTATUS_SIE | MSTATUS_UPIE | MSTATUS_SPIE | MSTATUS_SPP |     \
   MSTATUS_FS | MSTATUS_XS | MSTATUS_SUM | MSTATUS_MXR)
#define SSTATUS_MASK SSTATUS_MASK0

#define MSTATUS_MASK                                                           \
  (MSTATUS_UIE | MSTATUS_SIE | MSTATUS_MIE | MSTATUS_UPIE | MSTATUS_SPIE |     \
   MSTATUS_MPIE | MSTATUS_SPP | MSTATUS_MPP | MSTATUS_FS | MSTATUS_MPRV |      \
   MSTATUS_SUM | MSTATUS_MXR)

/* cycle and insn counters */
#define COUNTEREN_MASK ((1 << 0) | (1 << 2))

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

#define XLEN 64
#define MTIME_ADDR 0x40000000
#define MTIMECMP_ADDR 0x40000008
#define UART_TX_ADDR 0x40002000

#include "bus.h"

enum Mode { USER = 0x00, SUPERVISOR = 0x01, MACHINE = 0x11 };

typedef struct CPU {
  uint64 x[32];
  uint64 pc;
  uint64 mepc;
  uint64 spec;
  uint32 csr[4096];
  enum Mode mode;
  Bus *bus;
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
uint32 cpu_fetch_instruction(Bus *bus);
#endif
