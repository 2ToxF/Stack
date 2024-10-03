#include <stdio.h>

#include "input_output.h"
#include "stack.h"

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


#ifndef NDEBUG
    void StackDump(const stack_t* stk, const char* file_name, int line_number)
    {
        int idx_first_of_same = 0;
        char index_string[12] = {};

        #define PRINT_CELLS_VALUE_(idx)                                             \
            {                                                                       \
                if (idx_first_of_same == (idx))                                     \
                    sprintf(index_string, "*[%d]", (idx));                          \
                else                                                                \
                    sprintf(index_string, "*[%d - %d]", idx_first_of_same, (idx));  \
                printf("\t\t%-12s  =  %d\n", index_string, stk->data[idx]);         \
                idx_first_of_same = (idx + 1);                                      \
            }


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

        for (int i = 1; i < stk->capacity; i++)
        {
            if (stk->data[i] != stk->data[i-1])
                PRINT_CELLS_VALUE_(i-1);
        }
        PRINT_CELLS_VALUE_(stk->capacity-1);

        printf("\t}\n"
            GRN "}\n" WHT);

        #undef PRINT_CELLS_VALUE
    }
#endif
