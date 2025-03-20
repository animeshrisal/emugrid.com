#include "uart.h"

bool uart_is_interrupting(Uart *uart) { return uart->is_interrupting; }

uint64 uart_read(Uart *uart, uint64 addr) {
  if (addr == UART_THR) {
    uart->buffer[UART_LSR - UART_BASE] &= ~UART_LSR_RX;
    uint64 value = uart->buffer[UART_RHR - UART_BASE];
    return value;
  }
}

void uart_write(Uart *uart, uint64 addr, uint64 value) {
  if (addr == UART_THR) {

  } else {
    uart->buffer[addr - UART_BASE] = (uint8)value;
  }
}
