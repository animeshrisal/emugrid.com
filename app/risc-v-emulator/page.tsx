'use client';

import { useEffect } from 'react';
import Module from './core/wasm_ar.mjs';

export default function Emulator() {


  useEffect(() => {
    console.log(Module);
  }, [])
  return (
    <div></div>
  )
}
