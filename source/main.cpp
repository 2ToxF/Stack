/*!
    \author Toxic
    \date 01.10.2024
*/

#include <stddef.h>
#include <stdio.h>

#include "input_output.h"
#include "stack.h"

CodeError RunProgram();


int main()
{
    CodeError code_err = RunProgram();
    PrintCodeError(code_err);
    return code_err;
}


CodeError RunProgram()
{
    size_t stack = 0;
    if (CREATE_STACK(&stack) != STK_NO_ERROR)
        return STACK_ERR;

    int numbers_counter = 0;
    printf("begin\n");
    for (StackElem_t i = 2; i < 100; i++)
    {
        if (StackPush(stack, i) != STK_NO_ERROR)
            return STACK_ERR;

        printf("num_to_put = %d\n", i);
        numbers_counter++;
    }

    int printed_numbers_counter = 0;
    StackElem_t var_for_print = 0;
    printf("\n\nprint result\n");
    for (int i = 2; i < 100; i++)
    {
        if (StackPop(stack, &var_for_print) != STK_NO_ERROR)
            return STACK_ERR;

        printf("var_for_print = %d\n", var_for_print);
        printed_numbers_counter++;
    }

    StackDtor(&stack);
    return NO_ERROR;
}
