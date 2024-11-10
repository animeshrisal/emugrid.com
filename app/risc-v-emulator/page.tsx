'use client';

import { useEffect, useState } from 'react';
import Module from './core/main.js';
import { EmscriptenModule } from './core/index.js';

export default function Emulator() {

  const [wasm, setWasm] = useState<EmscriptenModule | undefined>(undefined);

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

      // Call the C function `_process_file` with the filename
      const test = wasm.ccall('read_elf_file', 'number', ['string'], ['/elf_file.o']);
      console.log(test);
    };

    // Read the file as an ArrayBuffer
    reader.readAsArrayBuffer(file);
  };

  if (!wasm) {
    return <div>Loading...</div>
  };

  return (
    <div>
      <h1>Risc V - Emulator</h1>
      <button onClick={() => wasm.ccall('handle_instruction', 'number', ['number'], [])}>Run next instruction</button>
      <input type="file" onChange={handleFileChange} />
    </div>
  )
}
