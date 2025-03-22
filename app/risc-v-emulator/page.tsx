'use client';

import { useEffect, useState, useRef } from 'react';
import Module from './core/main.js';
import { CPU, EmscriptenModule, UART } from './core/index.js';
import InstructionList from './components/InstructionList';
import styles from './page.module.scss';
import Uart from './components/Uart';

const RISC_V_REGISTER_OFFSET = 0;
const PC_OFFSET = RISC_V_REGISTER_OFFSET + 32 * 8;
const CSR_OFFSET = PC_OFFSET + 8;

const UART_OFFSET = 0;
const CLOCK_OFFSET = UART_OFFSET + 1 * 256;
const IS_INTERRUPTING_OFFSET = CLOCK_OFFSET + 8;

export default function Emulator() {
  const [wasm, setWasm] = useState<EmscriptenModule | undefined>(undefined);
  const [cpu, setCPU] = useState<CPU | undefined>(undefined);
  const [disassembleCode, setDisassembleCode] = useState<string[]>([]);
  const [currentInstruction, setCurrentInstruction] = useState(0);
  const runningRef = useRef<boolean>(false);
  const charRef = useRef<string>("");

  const writeUart = () => {
    if (wasm) {
      const uart_ptr = wasm.ccall<number, void[]>('get_uart_ptr', 'number', [], []);

      const upperCaseLetter = charRef.current.toUpperCase();
      if (upperCaseLetter >= 'A' && upperCaseLetter <= 'Z') {
        wasm.HEAP8[uart_ptr] = upperCaseLetter.charCodeAt(0) - 64;
        wasm.HEAP8[uart_ptr + IS_INTERRUPTING_OFFSET] = 1;
      } else {
      }

    }
  };

  useEffect(() => {
    const handleKeyPress = (event: KeyboardEvent) => {
      charRef.current = event.key
      writeUart();
    };


    window.addEventListener('keydown', handleKeyPress)

    return () => window.removeEventListener('keydown', handleKeyPress)
  }, [writeUart]);

  const runEmulatorLoop = () => {
    if (!runningRef.current) return;

    handleNextInstruction();
    requestAnimationFrame(runEmulatorLoop);
  }

  const handleStartExecution = () => {
    runningRef.current = true;
    runEmulatorLoop();
  }

  const handleStopExecution = () => {
    runningRef.current = false;
  }

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

  function readUart(ptr: number): UART | undefined {
    if (wasm) {
      for (let i = 0; i < 100; i++) {
        wasm.HEAPU8[(ptr) + i] = 1;
      }
    } else {
      return undefined;
    }
  }

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
      wasm.ccall<number, string>('read_elf_file', 'number', ['string'], ['/elf_file.o']);
      const listOfInstructions = wasm.ccall<string, void[]>('show_disassembled_code', 'string', [], []);

      const disassembleC = listOfInstructions.split('\n')
        .filter((instr: string) => instr.trim() !== '');

      setDisassembleCode(disassembleC)
    };

    reader.readAsArrayBuffer(file);
    handleNextInstruction();
  };

  if (!wasm) {
    return <div>Loading...</div>
  };

  const handleNextInstruction = () => {
    const pc = wasm.ccall<number, number>('main_loop', 'number', ['number'], [])
    const cpu_ptr = wasm.ccall<number, void[]>('get_cpu_ptr', 'number', [], []);
    const uart_ptr = wasm.ccall<number, void[]>('get_uart_ptr', 'number', [], []);
    setCurrentInstruction(pc);
    const _cpu = readCPU(cpu_ptr);
    const _uart = readUart(uart_ptr);
    setCPU(_cpu);
  }

  const Registers = () => {
    if (cpu) {
      const registerElements = [];
      for (let i = 0; i < cpu.riscv_register.length; i++) {
        registerElements.push(
          <div key={i}>{`x${i}: ${cpu.riscv_register[i].toString()}`}</div>
        );
      }

      return <div>{registerElements}</div>;
    } else {
      return (
        <div>Load an elf file to start running</div>
      )
    }
  }

  return (
    <div>
      <h1>Risc V - Emulator</h1>
      {cpu && <div>
        <button onClick={() => handleNextInstruction()}>Run next instruction</button>
        <button onClick={() => handleStopExecution()}>Stop instruction</button>
        <button onClick={() => handleStartExecution()}>Continue Execution</button>
      </div>
      }

      <input type="file" onChange={handleFileChange} />
      <div className={styles['emulator-container']} >
        <Registers />
        <Uart />

        <div id="inputBuffer" className={styles['hide']}>{charRef.current}</div>
        <InstructionList instructions={disassembleCode} current={currentInstruction} />
      </div>
    </div >
  )
}
