#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "stack.h"

void print_code_error(CodeError code_error);
void StackDump(const stack_t* stk, const char* file_name, int line_number);

#endif
