cmake_minimum_required(VERSION 3.12)
project(wasm_ar)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXECUTABLE_SUFFIX ".mjs")

add_executable(wasm_ar main.cpp)

set_target_properties(wasm_ar PROPERTIES LINK_FLAGS  "-lembind --emit-tsd interface.d.ts --bind -s MODULARIZE=1 -s -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s SINGLE_FILE=1 -s ENVIRONMENT='web' -s EXPORT_ES6=1")


