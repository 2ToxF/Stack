/*!
    \author Toxic
    \date 01.10.2024
*/

#include <stdio.h>

#include "stack.h"
#include "input_output.h"

CodeError run_program();

int main()
{
    CodeError code_err = run_program();
    print_code_error(code_err);
    return code_err;
}


CodeError run_program()
{
    CodeError code_err = NO_ERROR;

    stack_t stack = CREATE_STACK(stack);
    if ((code_err = StackInit(&stack)) != NO_ERROR)
        return code_err;

    int numbers_counter = 0;
    printf("begin\n");
    for (StackElem_t i = 2; i < 100; i++)
    {
        if ((code_err = StackPush(&stack, i)) != NO_ERROR)
            return code_err;

        printf("num_to_put = %d, stack_idx = %d, stack_size = %d\n", i, stack.index, stack.capacity);
        numbers_counter++;
    }

    int printed_numbers_counter = 0;
    StackElem_t var_for_print = 0;
    printf("\n\nprint result\n");
    for (int i = 2; i < 100; i++)
    {
        if ((code_err = StackPop(&stack, &var_for_print)) != NO_ERROR)
            return code_err;

        printf("var_for_print = %d, stack_idx = %d, stack_size = %d\n", var_for_print, stack.index, stack.capacity);
        printed_numbers_counter++;
    }

    StackDtor(&stack);
    return code_err;
}
