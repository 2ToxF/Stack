#include <stdio.h>

#include "input_output.h"
#include "stack.h"
#include "verify.h"

CodeError StackVerify(stack_t* stk, const char* file_name, int line_number)
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
        {
            StackDump(stk, file_name, line_number);
            return STACK_USES_MUCH_MEM_ERR;
        }

        if (stk->canary_left != CANARY_STACK_VALUE || stk->canary_right != CANARY_STACK_VALUE)
        {
            StackDump(stk, file_name, line_number);
            return STKSTRUCT_INFO_CORRUPT_ERR;
        }
        if (*((uint64_t*) ((char*) stk->data - SIZE_OF_CANARY)) != CANARY_DATA_VALUE
            || *((uint64_t*) (stk->data + stk->capacity)) != CANARY_DATA_VALUE)
        {
            StackDump(stk, file_name, line_number);
            return STKDATA_INFO_CORRUPT_ERR;
        }

        unsigned long temp_hash_struct = stk->hash_struct;
        printf("%lu != %lu\n", temp_hash_struct, stk->hash_struct);
        STACK_HASH(stk);
        printf("%lu\n", stk->hash_struct);
        STACK_HASH(stk);
        printf("%lu\n", stk->hash_struct);
        STACK_HASH(stk);
        printf("%lu\n", stk->hash_struct);
        STACK_HASH(stk);
        printf("%lu\n", stk->hash_struct);
        if (temp_hash_struct != stk->hash_struct)
        {
            StackDump(stk, file_name, line_number);
            return STKSTRUCT_INFO_CORRUPT_ERR;
        }

        unsigned long temp_hash_data = stk->hash_data;
        STACK_HASH(stk);
        if (temp_hash_data != stk->hash_data)
        {
            StackDump(stk, file_name, line_number);
            return STKDATA_INFO_CORRUPT_ERR;
        }
    #endif

    return NO_ERROR;
}
