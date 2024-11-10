export interface Wasm {
  _fib(a: number): number;
}

export declare const WasmPromise: Promise<Wasm>;

export default WasmPromise;


export interface EmscriptenModule {
  FS: {
    writeFile: (filename: string, data: Uint8Array) => void;
  };
  ccall: (
    funcName: string,
    returnType: string | null,
    argTypes: string[],
    args: unknown[]
  ) => unknown;
}
