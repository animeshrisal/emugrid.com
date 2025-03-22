#ifndef BUS_H
#define BUS_H

#include "types.h"
#include "uart.h"

#define DRAM_SIZE 1 * 1024 * 1204 // 1 GB RAM
#define DRAM_BASE 0x80000000

typedef struct Bus {
  uint64 memory[DRAM_SIZE];
  Uart *uart;
} Bus;

uint64 load(Bus *bus, uint64 address, uint8 size);
uint64 load_bus(Bus *bus, uint64 address, uint8 size);
uint8 bus_read8(Bus *bus, uint64 address);
uint16 bus_read16(Bus *bus, uint64 address);
uint32 bus_read32(Bus *bus, uint64 address);
uint64 bus_read64(Bus *bus, uint64 address);

void store(Bus *bus, uint64 address, uint64 value, uint8 size);
void store_bus(Bus *bus, uint64 address, uint64 value, uint8 size);
void bus_write8(Bus *bus, uint64 addr, uint8 value);
void bus_write16(Bus *bus, uint64 addr, uint16 value);
void bus_write32(Bus *bus, uint64 addr, uint32 value);
void bus_write64(Bus *bus, uint64 addr, uint64 value);
#endif
