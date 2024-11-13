export interface Wasm {
  _fib(a: number): number;
}

export interface CPU {
  riscv_register: BigInt64Array; // 64-bit integer registers
  pc: bigint;                    // 64-bit integer for program counter
  csr: Uint32Array;              // 32-bit integer CSR array
  bus: number | null;            // Pointer to Bus (null if not used)
}

export interface registers {

}

export interface RiscVRegisters {
  x0: number;
  x1: number;
  x2: number;
  x3: number;
  x4: number;
  x5: number;
  x6: number;
  x7: number;
  x8: number;
  x9: number;
  x10: number;
  x11: number;
  x12: number;
  x13: number;
  x14: number;
  x15: number;
  x16: number;
  x17: number;
  x18: number;
  x19: number;
  x20: number;
  x21: number;
  x22: number;
  x23: number;
  x24: number;
  x25: number;
  x26: number;
  x27: number;
  x28: number;
  x29: number;
  x30: number;
  x31: number;
}


export declare const WasmPromise: Promise<Wasm>;

export default WasmPromise;


export interface EmscriptenModule {
  _allocate_cpu(): void;
  _get_struct_ptr(): number;
  _free_struct(): void;
  HEAP8: Int8Array;
  HEAP16: Int16Array;
  HEAP32: Int32Array;
  HEAPU8: Uint8Array;
  HEAPU16: Uint16Array;
  HEAPU32: Uint32Array;
  HEAPF32: Float32Array;
  HEAPF64: Float64Array;
  HEAP64: BigInt64Array;
  HEAPU64: BigUint64Array;
  FS: {
    writeFile: (filename: string, data: Uint8Array) => void;
  };
  ccall: <ReturnType = unknown, ArgTypes = unknown>(
    funcName: string,
    returnType: string | null,
    argTypes: string[],
    args: ArgTypes[]
  ) => ReturnType;
}
