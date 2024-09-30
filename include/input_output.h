#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "debug_mode.h"
#include "stack.h"

void print_code_error(CodeError code_error);

#ifndef NDEBUG
    void StackDump(const stack_t* stk, const char* file_name, int line_number);
#endif

#endif
