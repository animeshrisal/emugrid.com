export interface Wasm {
  _fib(a: number): number;
}

export declare const WasmPromise: Promise<Wasm>;

export default WasmPromise;
