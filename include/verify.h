#ifndef VERIFY_H
#define VERIFY_H

#include "input_output.h"
#include "stack.h"

#define STACK_VERIFY(stk)                    \
    if (StackVerify(stk) != NO_ERROR)        \
    {                                        \
        StackDump(stk, __FILE__, __LINE__);  \
        return StackVerify(stk);             \
    }

CodeError StackVerify(const stack_t* stk);

#endif
