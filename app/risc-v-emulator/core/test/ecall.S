    .section .text
    .globl _start

_start:
  csrw mstatus, t0
  la t0, print
  csrw mtvec, t0
  ecall
  addi x2, x0, 400
  j loop

print:
    li a2, 0x10000000     
    li a1, 'H'             

    sb a1, 0(a2)

    li a1, 'e'             
    sb a1, 0(a2)

    li a1, 'l'             
    sb a1, 0(a2)

    li a1, 'l'            
    sb a1, 0(a2)

    li a1, 'o'             
    sb a1, 0(a2)

    li a1, ' '             
    sb a1, 0(a2)

    li a1, 'W'             
    sb a1, 0(a2)

    li a1, 'o'             
    sb a1, 0(a2)

    li a1, 'r'             
    sb a1, 0(a2)

    li a1, 'l'            
    sb a1, 0(a2)

    li a1, 'd'             
    sb a1, 0(a2)

    li a1, 10              
    sb a1, 0(a2)

    mret

loop:
  addi x1, x1, 4
  bne x1, x2, loop
