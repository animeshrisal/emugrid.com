'use client';

import { useEffect, useState } from 'react';
import Module from './core/main.js';

export default function Emulator() {

  const [fib, setFib] = useState<((n: number) => number) | undefined>(undefined);
  const [test, setTest] = useState<((n: number) => number) | undefined>(undefined);

  useEffect(() => {
    Module().then((mod) => {
      setFib(() => mod.cwrap("fib", "number", ["number"]))
      setTest(() => mod.cwrap("test", "number", ["number"]))
    })
  }, [])

  if (!fib) {
    return <div>Loading...</div>
  }

  if (!test) {
    return <div>Loading...</div>
  }
  return (
    <div>
      <div>{fib(5)}</div>
      <div>{test(5)}</div>
    </div>
  )
}
