/*!
    \file
    File with functions and macro for checking wheter stack is working without problems or not
*/

#ifndef VERIFY_H
#define VERIFY_H

#include "debug_mode.h"
#include "stack.h"

#ifndef NDEBUG
/// @brief Macro for verifying stack
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
/// @brief Macro for verifying stack
#define STACK_VERIFY(stk)                                    \
    do {                                                     \
        CodeError temp_code_err = NO_ERROR;                  \
        if ((temp_code_err = StackVerify(stk)) != NO_ERROR)  \
            return temp_code_err;                            \
    } while(0)
#endif

/*!
    Verifies stack's sructure and data (elements)
    \param[in]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
*/
CodeError StackVerify(stack_t* stk);

#endif
