#!/bin/bash

# EXPORST_NAME means we can import from fib.js as follows:
# import { fib } from './fib.js'
# and fib will contain the module with webassembly

# EXPORTED_FUNCTIONS is the list of functions
# exported from C. Make sure you put them into extern "C" {} block in your c++ code. 
# Then simply prefix the functions you are going to export with an underscore, and add it to the list below

# Uncomment -s "FILESYSTEM=0" if you don't need to use fs (i.e. use it on web entirely)

# if you create more dependencies of fib.cc, simply add them to the end of the below command, like: -o ./fib.js fib.cc add.cc anotherdep.cc and_so_on.cc
emcc -O3 -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["cwrap", "ccall", "FS"]' -s -s ALLOW_MEMORY_GROWTH=1 -s SINGLE_FILE=1 -s MODULARIZE=1 -s WASM_BIGINT=1 -s 'EXPORTED_FUNCTIONS=["_read_elf_file", "_handle_instruction", "_allocate_CPU", "_get_cpu_ptr", "_free_cpu"]' -s "ENVIRONMENT='web'" -o ./main.js *.c


    # -s "FILESYSTEM=0"
    #
