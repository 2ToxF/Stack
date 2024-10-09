/*!
    \author Toxic
    \date 01.10.2024
*/

#include <stddef.h>
#include <stdio.h>

#include "input_output.h"
#include "stack.h"

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

    size_t stack = 0;
    if ((code_err = CREATE_STACK(&stack)) != NO_ERROR)
        return code_err;

    int numbers_counter = 0;
    printf("begin\n");
    for (StackElem_t i = 2; i < 100; i++)
    {
        if ((code_err = StackPush(stack, i)) != NO_ERROR)
            return code_err;

        printf("num_to_put = %d\n", i);
        numbers_counter++;
    }

    int printed_numbers_counter = 0;
    StackElem_t var_for_print = 0;
    printf("\n\nprint result\n");
    for (int i = 2; i < 100; i++)
    {
        if ((code_err = StackPop(stack, &var_for_print)) != NO_ERROR)
            return code_err;

        printf("var_for_print = %d\n", var_for_print);
        printed_numbers_counter++;
    }

    StackDtor(&stack);
    return code_err;
}
