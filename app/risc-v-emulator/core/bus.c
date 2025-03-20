#include "bus.h"

uint64 load(Bus *bus, uint64 address) {
  if UART_BASE
    <= address &&address < UART_BASE + UART_SIZE
}

uint8 bus_read8(Bus *bus, uint64 address) {
  return (uint8)bus->memory[address];
}

uint16 bus_read16(Bus *bus, uint64 address) {
  return (uint16)bus->memory[address] | (uint16)(bus->memory[address + 1] << 8);
};

uint32 bus_read32(Bus *bus, uint64 address) {
  return (uint32)bus->memory[address] |
         (uint32)(bus->memory[address + 1] << 8) |
         (uint32)(bus->memory[address + 2] << 16) |
         (uint32)(bus->memory[address + 3] << 24);
}

uint64 bus_read64(Bus *bus, uint64 address) {
  return (uint64)bus->memory[address] |
         (uint64)(bus->memory[address + 1] << 8) |
         (uint64)(bus->memory[address + 2] << 16) |
         (uint64)(bus->memory[address + 3] << 24) |
         (uint64)(bus->memory[address + 4] << 32) |
         (uint64)(bus->memory[address + 5] << 40) |
         (uint64)(bus->memory[address + 6] << 48) |
         (uint64)(bus->memory[address + 7] << 56);
}

void bus_write8(Bus *bus, uint64 address, uint8 value) {
  bus->memory[address] = value;
}

void bus_write16(Bus *bus, uint64 address, uint16 value) {
  bus->memory[address] = (uint8)(value & 0xFF);
  bus->memory[address + 1] = (uint8)((value >> 8) & 0xFF);
}

void bus_write32(Bus *bus, uint64 address, uint32 value) {
  bus->memory[address] = (uint8)(value & 0xFF);
  bus->memory[address + 1] = (uint8)((value >> 8) & 0xFF);
  bus->memory[address + 2] = (uint8)((value >> 16) & 0xFF);
  bus->memory[address + 3] = (uint8)((value >> 24) & 0xFF);
}

void bus_write64(Bus *bus, uint64 address, uint64 value) {
  bus->memory[address] = (uint8)(value & 0xFF);
  bus->memory[address + 1] = (uint8)((value >> 8) & 0xFF);
  bus->memory[address + 2] = (uint8)((value >> 16) & 0xFF);
  bus->memory[address + 3] = (uint8)((value >> 24) & 0xFF);
  bus->memory[address + 4] = (uint8)((value >> 32) & 0xFF);
  bus->memory[address + 5] = (uint8)((value >> 40) & 0xFF);
  bus->memory[address + 6] = (uint8)((value >> 48) & 0xFF);
  bus->memory[address + 7] = (uint8)((value >> 56) & 0xFF);
}
