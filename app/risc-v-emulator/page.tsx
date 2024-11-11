'use client';

import { useEffect, useState } from 'react';
import Module from './core/main.js';
import { CPU, EmscriptenModule } from './core/index.js';

const RISC_V_REGISTER_OFFSET = 0;
const PC_OFFSET = RISC_V_REGISTER_OFFSET + 32 * 8;
const CSR_OFFSET = PC_OFFSET + 8;


export default function Emulator() {

  function readCPU(ptr: number): CPU | undefined {
    if (wasm) {
      const riscv_register = new BigInt64Array(32);
      for (let i = 0; i < 32; i++) {
        riscv_register[i] = wasm.HEAPU64[(ptr + RISC_V_REGISTER_OFFSET) / 8 + i];
      }

      const pc = wasm.HEAPU64[(ptr + PC_OFFSET) / 8];

      const csr = new Uint32Array(4096);
      for (let i = 0; i < 4096; i++) {
        csr[i] = wasm.HEAPU32[(ptr + CSR_OFFSET) / 4 + i];
      }

      // Assuming `bus` is not directly managed in JS for now
      const bus = null;

      return {
        riscv_register,
        pc,
        csr,
        bus
      };
    } else {
      return undefined;
    }
  }

  const [wasm, setWasm] = useState<EmscriptenModule | undefined>(undefined);
  const [cpu, setCPU] = useState<CPU | undefined>(undefined);

  useEffect(() => {
    Module().then((mod) => {
      setWasm(mod);
    })
  }, [])

  const handleFileChange = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file || !wasm) return;

    const reader = new FileReader();

    reader.onload = (e: ProgressEvent<FileReader>) => {
      // Create a Uint8Array from the file's ArrayBuffer
      const data = new Uint8Array(e.target?.result as ArrayBuffer);

      // Write the file to Emscripten's virtual filesystem
      wasm.FS.writeFile('/elf_file.o', data);
      wasm.ccall<void, void>('allocate_CPU', "NULL", [], []);
      // Call the C function `_process_file` with the filename
      const test = wasm.ccall<number, string>('read_elf_file', 'number', ['string'], ['/elf_file.o']);
      console.log(test);
    };

    // Read the file as an ArrayBuffer
    reader.readAsArrayBuffer(file);
  };

  if (!wasm) {
    return <div>Loading...</div>
  };

  const handleNextInstruction = () => {
    wasm.ccall('handle_instruction', 'number', ['number'], [])
    const cpu_ptr = wasm.ccall<number, void[]>('get_cpu_ptr', 'number', [], []);
    const cpu = readCPU(cpu_ptr);
    setCPU(cpu);
  }

  const Registers = () => {
    if (cpu) {
      return (
        <div>
          {cpu.riscv_register[0]}
          {cpu.riscv_register[1]}
          {cpu.riscv_register[2]}
          {cpu.riscv_register[3]}
          {cpu.riscv_register[4]}

        </div>
      )
    } else {
      return (
        <div>Empty</div>
      )
    }
  }

  return (
    <div>
      <h1>Risc V - Emulator</h1>
      <button onClick={() => handleNextInstruction()}>Run next instruction</button>
      <input type="file" onChange={handleFileChange} />
      <Registers />
    </div>
  )
}
