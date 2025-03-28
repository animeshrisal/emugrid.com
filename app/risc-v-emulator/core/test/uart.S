    .section .text
    .globl _start

_start:
    li a2, 0x10000000 #Address of uart
    li a1, 'H'             # Character 'H'
    sb a1, 0(a2)

    li a1, 'e'             # Character 'e'
    sb a1, 0(a2)

    li a1, 'l'             # Character 'l'
    sb a1, 0(a2)

    li a1, 'l'             # Character 'l'
    sb a1, 0(a2)

    li a1, 'o'             # Character 'o'
    sb a1, 0(a2)

    li a1, ' '             # Space
    sb a1, 0(a2)

    li a1, 'W'             # Character 'W'
    sb a1, 0(a2)

    li a1, 'o'             # Character 'o'
    sb a1, 0(a2)

    li a1, 'r'             # Character 'r'
    sb a1, 0(a2)

    li a1, 'l'             # Character 'l'
    sb a1, 0(a2)

    li a1, 'd'             # Character 'd'
    sb a1, 0(a2)

    # Optional: Add a newline character
    li a1, 10              # Newline (ASCII 10)
    sb a1, 0(a2)

loop:
  addi x1, x1, 4
  bne x1, x2, loop
