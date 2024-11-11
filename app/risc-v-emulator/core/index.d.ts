export interface Wasm {
  _fib(a: number): number;
}

export interface CPU {
  riscv_register: BigInt64Array; // 64-bit integer registers
  pc: bigint;                    // 64-bit integer for program counter
  csr: Uint32Array;              // 32-bit integer CSR array
  bus: number | null;            // Pointer to Bus (null if not used)
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
