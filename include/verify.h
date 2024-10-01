#ifndef VERIFY_H
#define VERIFY_H

#include "debug_mode.h"
#include "stack.h"

#ifndef NDEBUG
#define STACK_VERIFY(stk)                                    \
    do {                                                     \
        CodeError temp_code_err = NO_ERROR;                  \
        if ((temp_code_err = StackVerify(stk)) != NO_ERROR)  \
        {                                                    \
            if (temp_code_err >= STACK_USES_MUCH_MEM_ERR)    \
                StackDump(stk, __FILE__, __LINE__);          \
            return temp_code_err;                            \
        }                                                    \
    } while(0)
#else
#define STACK_VERIFY(stk)                                    \
    do {                                                     \
        CodeError temp_code_err = NO_ERROR;                  \
        if ((temp_code_err = StackVerify(stk)) != NO_ERROR)  \
            return temp_code_err;                            \
    } while(0)
#endif

CodeError StackVerify(stack_t* stk);

#endif
