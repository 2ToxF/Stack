#include <stdio.h>

#include "input_output.h"
#include "stack.h"

void print_code_error(CodeError code_error)
{
    #define ERR_DESCR_(error)                                   \
        case (error):                                           \
            printf("\n\n" RED "CODE_ERROR: " #error WHT "\n");  \
            break

    switch(code_error)
    {
        case NO_ERROR:
            printf("\n\n" GRN "Code was completed without errors" WHT "\n");
            break;

        ERR_DESCR_(CANT_CREATE_RAND_NUM_ERR);
        ERR_DESCR_(STACK_ALREADY_INITED_ERR);
        ERR_DESCR_(NULL_STK_STRUCT_PTR_ERR);
        ERR_DESCR_(NULL_STK_DATA_PTR_ERR);
        ERR_DESCR_(NEG_STK_CAPACITY_ERR);
        ERR_DESCR_(STACK_ANTIOVERFLOW_ERR);
        ERR_DESCR_(STACK_OVERFLOW_ERR);
        ERR_DESCR_(OUT_OF_MEMORY_ERR);
        ERR_DESCR_(STACK_USES_MUCH_MEM_ERR);
        ERR_DESCR_(STKSTRUCT_CANARY_CORRUPT_ERR);
        ERR_DESCR_(STKDATA_CANARY_CORRUPT_ERR);
        ERR_DESCR_(STKSTRUCT_INFO_CORRUPT_ERR);
        ERR_DESCR_(STKDATA_INFO_CORRUPT_ERR);

        default:
            printf("\n\n" RED "CODE_ERROR: UNKNOWN_ERROR" WHT "\n");
            break;
    }

    #undef ERR_DESCR_
}
