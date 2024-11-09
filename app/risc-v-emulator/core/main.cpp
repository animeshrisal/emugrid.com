#ifdef __EMSCRIPTEN__

#include <emscripten/bind.h>

std::string hello() { return "Hello from C++!"; }

EMSCRIPTEN_BINDINGS(wasm_ar) { emscripten::function("hello", &hello); }
#endif // !EMSCRIPTEN
