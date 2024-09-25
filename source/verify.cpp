#include <stddef.h>

#include "stack.h"
#include "verify.h"

CodeError StackVerify(const stack_t* stk)
{
    if (stk == 0)
        return NULL_STK_STRUCT_PTR_ERR;
    if (stk->data == 0)
        return NULL_STK_DATA_PTR_ERR;
    if (stk->capacity < 0)
        return NEG_STK_CAPACITY_ERR;
    if (stk->index < 0)
        return STACK_ANTIOVERFLOW_ERR;
    if (stk->index >= stk->capacity)
        return STACK_OVERFLOW_ERR;
    // if (stk->index < stk->capacity / RESIZE_COEF_DOWN && (size_t) stk->capacity > DEFAULT_STK_CAPACITY)
    //     return STACK_USES_MUCH_MEM_ERR;
    return NO_ERROR;
}
