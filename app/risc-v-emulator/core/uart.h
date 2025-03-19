#include "types.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define UART_BASE 0x10000000
#define UART_SIZE 0x100
#define UART_IRQ 10
#define UART_RHR (UART_BASE + 0)
#define UART_THR (UART_BASE + 0)
#define UART_ISR (UART_BASE + 2)
#define UART_LSR (UART_BASE + 5)
#define BYTE 1

typedef struct {
  uint8 uart[UART_SIZE];
  uint64 clock;
  bool not_null;
} Uart;
