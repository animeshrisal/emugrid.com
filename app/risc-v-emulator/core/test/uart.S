    .section .text
    .globl _start

_start:
    li a1, 'h'    
    li a2, 0x10000000 #Address of uart
    sb a1, 0(a2)

loop:
  addi x1, x1, 4
  bne x1, x2, loop
