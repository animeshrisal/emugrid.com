#include "uart.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

bool uart_is_interrupting(Uart *uart) { return uart->is_interrupting; }

uint64 uart_load(Uart *uart, uint64 addr) {
  switch (addr) {
  case UART_RHR: {
    uart->buffer[UART_LSR - UART_BASE] &= !1;
    return uart->buffer[addr - UART_BASE];
    break;
  }
  }

  return 0;
}

void uart_store(Uart *uart, uint64 addr, uint64 value) {
  if (addr == UART_THR) {
    printf("%d\n", uart->buffer[addr - UART_BASE]);

#ifdef __EMSCRIPTEN__
    EM_ASM(
        {
          let outputElement = document.getElementById('output');
          outputElement.textContent += String.fromCharCode(Number($0));
        },
        value);
#endif
  } else {
    uart->buffer[addr - UART_BASE] = (uint8)value;
  }
}
