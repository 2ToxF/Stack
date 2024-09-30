#include <stdio.h>

#include "input_output.h"

#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"


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

        ERR_DESCR_(STACK_ALREADY_INITED_ERR);
        ERR_DESCR_(NULL_STK_STRUCT_PTR_ERR);
        ERR_DESCR_(NULL_STK_DATA_PTR_ERR);
        ERR_DESCR_(NEG_STK_CAPACITY_ERR);
        ERR_DESCR_(STACK_ANTIOVERFLOW_ERR);
        ERR_DESCR_(STACK_OVERFLOW_ERR);
        ERR_DESCR_(OUT_OF_MEMORY_ERR);
        ERR_DESCR_(STACK_USES_MUCH_MEM_ERR);
        ERR_DESCR_(STKDATA_INFO_CORRUPT_ERR);
        ERR_DESCR_(STKSTRUCT_INFO_CORRUPT_ERR);

        default:
            printf("\n\n" RED "CODE_ERROR: UNKNOWN_ERROR" WHT "\n");
            break;
    }

    #undef ERR_DESCR_
}


void StackDump(const stack_t* stk, const char* file_name, int line_number)
{
    printf(RED "stack_t " MAG "%s [0x%p] " BLU "at %s:%d " YEL "born at %s:%d (%s)\n",
           stk->stk_name, stk,
           file_name, line_number,
           stk->init_file, stk->init_line, stk->init_func);
    printf(GRN "{\n"
           "%-10s= %d\n"
           "%-10s= %d\n"
           "\n"
           MAG "\tdata [0x%p]:\n"
           CYN "\t{\n",
           "\tindex", stk->index,
           "\tcapacity", stk->capacity,
           stk->data);

    for (int i = 0; i < stk->index; i++)
        printf("\t\t*[%d] = %d\n", i, stk->data[i]);

    if (stk->index != stk->capacity - 1)
        printf("\t\t[%d - %d] = NULL\n", stk->index, stk->capacity - 1);

    printf("\t}\n"
           GRN "}\n" WHT);
}
