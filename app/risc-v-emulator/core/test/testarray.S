.data
array: .word 5, 10, 15, 20, 25
length: .word 5
sum: .word 0
  
.text
.globl _start

_start:
  la t0, array
  lw t1, length
  li t2, 0
  li t3, 0

loop:
  beq t2, t1, end
  slli t1, t2, 2
  add t5, t0, t4
  lw t6, 0(t5)
  add t3, t3, t6
  addi t2, t2 1
  j loop

end:
  sw t3, sum
  li a7, 10
  ecall
