riscv64-unknown-elf-as uart.S -o uart.o
riscv64-unknown-elf-as ecall.S -o ecall.o

riscv64-unknown-elf-ld -T ecall.ld -o ecall ecall.o
riscv64-unknown-elf-ld -T uart.ld -o uart uart.o

