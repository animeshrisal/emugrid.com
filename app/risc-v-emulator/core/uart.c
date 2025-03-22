#include "uart.h"

bool uart_is_interrupting(Uart *uart) { return uart->is_interrupting; }

uint64 uart_read(Uart *uart, uint64 addr) {
  switch (addr) {
  case UART_RHR: {
    uart->buffer[UART_LSR - UART_BASE] &= !1;
    return uart->buffer[addr - UART_BASE];
    break;
  }
  }

  return 0;
}

void uart_write(Uart *uart, uint64 addr, uint64 value) {
  if (addr == UART_THR) {
    printf("%d", uart->buffer[addr - UART_BASE]);
  } else {
    uart->buffer[addr - UART_BASE] = (uint8)value;
  }
}
