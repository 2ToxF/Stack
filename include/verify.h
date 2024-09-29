#ifndef VERIFY_H
#define VERIFY_H

#include "stack.h"

#ifndef NDEBUG
#define STACK_VERIFY(stk)                                                        \
    do {                                                                         \
        CodeError temp_code_err = NO_ERROR;                                      \
        if ((temp_code_err = StackVerify(stk, __FILE__, __LINE__)) != NO_ERROR)  \
            return temp_code_err;                                                \
    } while(0)
#else
#define STACK_VERIFY(stk)
#endif

CodeError StackVerify(stack_t* stk, const char* file_name, int line_number);

#endif
