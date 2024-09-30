#include "stack.h"
#include "verify.h"

CodeError StackVerify(stack_t* stk)
{
    #ifndef NDEBUG
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
            return STACK_USES_MUCH_MEM_ERR;
    #endif

    if (stk->left_canary != STACK_CANARY_VALUE || stk->right_canary != STACK_CANARY_VALUE)
        return STKSTRUCT_INFO_CORRUPT_ERR;
    if (*((canary_t*) ((char*) stk->data - SIZE_OF_CANARY)) != DATA_CANARY_VALUE
        || *((canary_t*) (stk->data + stk->capacity)) != DATA_CANARY_VALUE)
        return STKDATA_INFO_CORRUPT_ERR;

    unsigned long temp_hash_data = stk->hash_data, temp_hash_struct = stk->hash_struct;
    StackHash(stk);
    if (temp_hash_struct != stk->hash_struct)
        return STKSTRUCT_INFO_CORRUPT_ERR;
    if (temp_hash_data != stk->hash_data)
        return STKDATA_INFO_CORRUPT_ERR;

    return NO_ERROR;
}
