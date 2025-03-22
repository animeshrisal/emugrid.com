#include "types.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define UART_BASE 0x10000000
#define UART_SIZE 0x100
#define UART_IRQ 10
#define UART_RHR (UART_BASE + 0)
#define UART_THR (UART_BASE + 0)
#define UART_ISR (UART_BASE + 0)
#define UART_LCR (UART_BASE + 3)
#define UART_LSR (UART_BASE + 5)
#define UART_LSR_RX 0x01;
#define UART_LSR_TX 0x20;

typedef struct {
  uint8 buffer[UART_SIZE];
  uint64 clock;
  uint8 is_interrupting;
} Uart;

bool uart_is_interrupting(Uart *uart);
uint64 uart_load(Uart *uart, uint64 addr);
void uart_store(Uart *uart, uint64 addr, uint64 value);
