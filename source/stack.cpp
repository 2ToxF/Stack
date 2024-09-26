#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "verify.h"


void StackDtor(stack_t* stk)
{
    free(stk->data); stk->data = NULL;
    stk->index = 0;
    stk->capacity = 0;
}


CodeError StackResizeDown(stack_t* stk)
{
    STACK_VERIFY(stk)

    stk->capacity /= RESIZE_COEF_UP;
    stk->data = (StackElem_t*) realloc(stk->data, stk->capacity*sizeof(StackElem_t));

    STACK_VERIFY(stk)
    return NO_ERROR;
}


CodeError StackResizeUp(stack_t* stk)
{
    STACK_VERIFY(stk)

    stk->capacity *= RESIZE_COEF_UP;
    stk->data = (StackElem_t*) realloc(stk->data, stk->capacity*sizeof(StackElem_t));

    STACK_VERIFY(stk);
    return NO_ERROR;
}


CodeError StackInit(stack_t* stk)
{
    if (stk->data != NULL || stk->index != 0 || stk->capacity != 0)
    {
        StackDump(stk, __FILE__, __LINE__);
        return STACK_ALREADY_INITED_ERR;
    }

    stk->data = (StackElem_t*) calloc(DEFAULT_STK_CAPACITY, sizeof(StackElem_t));
    stk->index = 0;
    stk->capacity = DEFAULT_STK_CAPACITY;

    STACK_VERIFY(stk);
    return NO_ERROR;
}


CodeError StackPop(stack_t* stk, StackElem_t* var)
{
    CodeError code_err = NO_ERROR;

    STACK_VERIFY(stk);
    if (stk->index == 0)
    {
        StackDump(stk, __FILE__, __LINE__);
        return STACK_ANTIOVERFLOW_ERR;
    }

    stk->index--;
    *var = stk->data[stk->index];

    if (stk->index == stk->capacity / RESIZE_COEF_DOWN && (size_t) stk->capacity > DEFAULT_STK_CAPACITY)
        if ((code_err = StackResizeDown(stk)) != NO_ERROR)
            return code_err;

    STACK_VERIFY(stk);
    return NO_ERROR;
}


CodeError StackPush(stack_t* stk, StackElem_t value)
{
    CodeError code_err = NO_ERROR;

    STACK_VERIFY(stk);
    if (stk->index == stk->capacity - 1)
        if ((code_err = StackResizeUp(stk)) != NO_ERROR)
            return code_err;

    stk->data[stk->index] = value;
    stk->index++;

    STACK_VERIFY(stk);
    return NO_ERROR;
}
