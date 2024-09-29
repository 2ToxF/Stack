#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "input_output.h"
#include "stack.h"
#include "verify.h"


void StackDtor(stack_t* stk)
{
    free((char*) stk->data - SIZE_OF_CANARY); stk->data = NULL;
    stk->index = 0;
    stk->capacity = 0;
}


CodeError StackResizeDown(stack_t* stk)
{
    STACK_VERIFY(stk);

    stk->capacity /= RESIZE_COEF;
    char* new_data = (char*) ((char*) realloc((char*) stk->data - SIZE_OF_CANARY,
                                              stk->capacity*sizeof(StackElem_t) + SIZE_OF_CANARY*2) + SIZE_OF_CANARY);
    *((uint64_t*) (new_data + stk->capacity*sizeof(StackElem_t))) = CANARY_DATA_VALUE;

    if (new_data == NULL)
        return OUT_OF_MEMORY_ERR;
    stk->data = (StackElem_t*) new_data;

    STACK_VERIFY(stk);
    return NO_ERROR;
}


CodeError StackResizeUp(stack_t* stk)
{
    STACK_VERIFY(stk);

    stk->capacity *= RESIZE_COEF;
    char* new_data = (char*) realloc((char*) stk->data - SIZE_OF_CANARY,
                                     stk->capacity*sizeof(StackElem_t) + SIZE_OF_CANARY*2) + SIZE_OF_CANARY;
    printf("left canary = %llu\n", *((uint64_t*) (new_data - SIZE_OF_CANARY)));
    printf("data[0] = %d\n", *((StackElem_t*) new_data));
    *((uint64_t*) (new_data + stk->capacity*sizeof(StackElem_t))) = CANARY_DATA_VALUE;

    if (new_data == NULL)
        return OUT_OF_MEMORY_ERR;
    stk->data = (StackElem_t*) new_data;

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

    char* new_data = (char*) calloc(DEFAULT_STK_CAPACITY*sizeof(StackElem_t) + SIZE_OF_CANARY*2, sizeof(char));
    *((uint64_t*) new_data) = CANARY_DATA_VALUE;
    *((uint64_t*) (new_data + DEFAULT_STK_CAPACITY*sizeof(StackElem_t) + SIZE_OF_CANARY)) = CANARY_DATA_VALUE;

    stk->data = (StackElem_t*) (new_data + SIZE_OF_CANARY);
    stk->index = 0;
    stk->capacity = DEFAULT_STK_CAPACITY;

    STACK_VERIFY(stk);
    return NO_ERROR;
}


CodeError StackPop(stack_t* stk, StackElem_t* var)
{
    STACK_VERIFY(stk);

    CodeError code_err = NO_ERROR;
    if (stk->index == 0)
    {
        StackDump(stk, __FILE__, __LINE__);
        return STACK_ANTIOVERFLOW_ERR;
    }

    stk->index--;
    *var = stk->data[stk->index];
    stk->data[stk->index] = 0;

    if (stk->index == stk->capacity / RESIZE_COEF_DOWN && (size_t) stk->capacity > DEFAULT_STK_CAPACITY)
        if ((code_err = StackResizeDown(stk)) != NO_ERROR)
            return code_err;

    STACK_VERIFY(stk);
    return NO_ERROR;
}


CodeError StackPush(stack_t* stk, StackElem_t value)
{
    STACK_VERIFY(stk);

    CodeError code_err = NO_ERROR;
    if (stk->index == stk->capacity - 1)
        if ((code_err = StackResizeUp(stk)) != NO_ERROR)
            return code_err;

    stk->data[stk->index] = value;
    stk->index++;

    STACK_VERIFY(stk);
    return NO_ERROR;
}
