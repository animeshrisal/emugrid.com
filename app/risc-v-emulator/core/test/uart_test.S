    .globl _start
    .text
_start:
    # Initialize UART base address
    li a0, 0x10000000   # UART base address (this might change based on your setup)

    # Write 'A' character to UART
    li a1, 0x41          # Load ASCII value of 'A' into a1 (0x41 is 'A')
write_loop:
    # Wait until UART is ready to transmit (check the TXFIFO empty flag)
    lw t0, 0(a0)         # Load UART status register
    andi t0, t0, 0x20    # Check if the TXFIFO is empty (TXFIFO empty bit is at position 5)
    beqz t0, write_loop  # If it's not empty, wait

    # Send the character (write it to the TXFIFO)
    sw a1, 0(a0)         # Store the character to the UART data register

    # Exit (we don't want an infinite loop for a simple example)
    li a7, 93            # ECALL for exit (exit code)
    ecall
