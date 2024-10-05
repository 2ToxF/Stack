#include <stdlib.h>
#include <string.h>

#include "input_output.h"
#include "stack.h"
#include "verify.h"

//----------------------------------------------------------------------------------------------------------------------

static const int HASH_SHIFT_COEF = 5;
static const unsigned long START_HASH = 5381;

//----------------------------------------------------------------------------------------------------------------------

void StackDtor(stack_t* stk)
{
    free((char*) stk->data - SIZE_OF_CANARY); stk->data = NULL;
    stk->index = 0;
    stk->capacity = 0;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackHash(stack_t* stk)
{
    CodeError code_err = NO_ERROR;

    if ((code_err = StackHashStruct(stk)) != NO_ERROR)
        return code_err;

    if ((code_err = StackHashData(stk)) != NO_ERROR)
        return code_err;

    return code_err;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackHashData(stack_t* stk)
{
    stk->hash_data = 0;
    unsigned long calc_hash = START_HASH;
    char* cur_byte = (char*) stk->data;

    for (size_t i = 0; i < stk->capacity*sizeof(StackElem_t); i++)
    {
        calc_hash = ((calc_hash << HASH_SHIFT_COEF) + calc_hash) + *cur_byte;
        cur_byte++;
    }

    stk->hash_data = calc_hash;
    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackHashStruct(stack_t* stk)
{
    stk->hash_struct = 0;
    unsigned long temp_hash_data = stk->hash_data;
    stk->hash_data = 0;

    unsigned long calc_hash = START_HASH;
    char* cur_byte = (char*) stk;
    const size_t sizeof_stk_struct = sizeof(*stk);

    for (size_t i = 0; i < sizeof_stk_struct; i++)
        calc_hash += *cur_byte;

    stk->hash_struct = calc_hash;
    stk->hash_data = temp_hash_data;
    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackInit(stack_t* stk)
{
    #ifndef NDEBUG
        if (stk->data != NULL || stk->index != 0 || stk->capacity != 0)
        {
            StackDump(stk, __FILE__, __LINE__);
            return STACK_ALREADY_INITED_ERR;
        }
    #endif

    stk->left_canary = stk->right_canary = STACK_CANARY_VALUE;

    char* new_data = (char*) calloc(DEFAULT_STK_CAPACITY*sizeof(StackElem_t) + SIZE_OF_CANARY*2, sizeof(char));
    *((canary_t*) new_data) = DATA_CANARY_VALUE;
    *((canary_t*) (new_data + DEFAULT_STK_CAPACITY*sizeof(StackElem_t) + SIZE_OF_CANARY)) = DATA_CANARY_VALUE;

    stk->data = (StackElem_t*) (new_data + SIZE_OF_CANARY);
    stk->index = 0;
    stk->capacity = DEFAULT_STK_CAPACITY;

    StackHash(stk);
    STACK_VERIFY(stk);
    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackPop(stack_t* stk, StackElem_t* var)
{
    STACK_VERIFY(stk);

    CodeError code_err = NO_ERROR;
    #ifndef NDEBUG
        if (stk->index == 0)
        {
            StackDump(stk, __FILE__, __LINE__);
            return STACK_ANTIOVERFLOW_ERR;
        }
    #endif

    if (stk->index == stk->capacity / RESIZE_COEF_DOWN && (size_t) stk->capacity > DEFAULT_STK_CAPACITY)
        if ((code_err = StackResizeDown(stk)) != NO_ERROR)
            return code_err;

    stk->index--;
    *var = stk->data[stk->index];
    stk->data[stk->index] = 0;

    StackHash(stk);
    STACK_VERIFY(stk);
    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackPush(stack_t* stk, StackElem_t value)
{
    STACK_VERIFY(stk);

    CodeError code_err = NO_ERROR;
    if (stk->index == stk->capacity - 1)
        if ((code_err = StackResizeUp(stk)) != NO_ERROR)
            return code_err;

    stk->data[stk->index] = value;
    stk->index++;

    StackHash(stk);
    STACK_VERIFY(stk);
    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackResizeDown(stack_t* stk)
{
    STACK_VERIFY(stk);

    stk->capacity /= RESIZE_COEF;
    char* new_data = (char*) realloc((char*) stk->data - SIZE_OF_CANARY,
                                     stk->capacity*sizeof(StackElem_t) + SIZE_OF_CANARY*2) + SIZE_OF_CANARY;
    *((canary_t*) (new_data + stk->capacity*sizeof(StackElem_t))) = DATA_CANARY_VALUE;
    memset(new_data + stk->index*sizeof(StackElem_t), 0, (stk->capacity - stk->index)*sizeof(StackElem_t));

    if (new_data == NULL)
        return OUT_OF_MEMORY_ERR;
    stk->data = (StackElem_t*) new_data;

    StackHash(stk);
    STACK_VERIFY(stk);
    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackResizeUp(stack_t* stk)
{
    STACK_VERIFY(stk);

    stk->capacity *= RESIZE_COEF;
    char* new_data = (char*) realloc((char*) stk->data - SIZE_OF_CANARY,
                                     stk->capacity*sizeof(StackElem_t) + SIZE_OF_CANARY*2) + SIZE_OF_CANARY;
    *((canary_t*) (new_data + stk->capacity*sizeof(StackElem_t))) = DATA_CANARY_VALUE;
    memset(new_data + stk->index*sizeof(StackElem_t), 0, (stk->capacity - stk->index)*sizeof(StackElem_t));

    if (new_data == NULL)
        return OUT_OF_MEMORY_ERR;
    stk->data = (StackElem_t*) new_data;

    StackHash(stk);
    STACK_VERIFY(stk);
    return NO_ERROR;
}
