import React from 'react';
import styles from './InstructionList.module.scss';

export interface InsructionList {
  instructions: string[],
  current: number
}

export default function InstructionList({ instructions, current }: InstructionList) {
  return (
    <div className={styles['instruction-container']}>
      <div className={styles['header']}>Code</div>
      <ul>{
        instructions.map((code: string, index: number) => <li key={index}><span className={current == index ? styles['active-code'] : styles['inactive-code']}>&gt;</span><span>{code}</span></li>)
      }</ul>
    </div>
  )
}
