#include "input_output.h"
#include "stack.h"
#include "verify.h"

CodeError StackVerify(const stack_t* stk, const char* file_name, int line_number)
{
    if (stk == NULL)
        return NULL_STK_STRUCT_PTR_ERR;
    if (stk->data == NULL)
        return NULL_STK_DATA_PTR_ERR;
    if (stk->capacity < 0)
        return NEG_STK_CAPACITY_ERR;
    if (stk->index < 0)
        return STACK_ANTIOVERFLOW_ERR;
    if (stk->index >= stk->capacity)
        return STACK_OVERFLOW_ERR;

    if (stk->index < stk->capacity / RESIZE_COEF_DOWN - 1 && (size_t) stk->capacity > DEFAULT_STK_CAPACITY)
    {
        StackDump(stk, file_name, line_number);
        return STACK_USES_MUCH_MEM_ERR;
    }

    #ifndef NDEBUG
        if (stk->canary_left != CANARY_STACK_VALUE || stk->canary_right != CANARY_STACK_VALUE)
        {
            StackDump(stk, file_name, line_number);
            return CANARY_STACK_ERR;
        }

        if (*((uint64_t*) ((char*) stk->data - SIZE_OF_CANARY)) != CANARY_DATA_VALUE
            || *((uint64_t*) (stk->data + stk->capacity)) != CANARY_DATA_VALUE)
        {
            StackDump(stk, file_name, line_number);
            return CANARY_DATA_ERR;
        }
    #endif

    return NO_ERROR;
}
