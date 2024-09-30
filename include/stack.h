#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#include "debug_mode.h"

#ifndef NDEBUG
#define ON_DEBUG(...) __VA_ARGS__
#define CREATE_STACK(stk) {0, #stk, __FILE__, __LINE__, __func__}
#else
#define ON_DEBUG(...)
#define CREATE_STACK(stk) {}
#endif

typedef int StackElem_t;
typedef uint64_t canary_t;

enum CodeError
{
    NO_ERROR,
    STACK_ALREADY_INITED_ERR,
    NULL_STK_STRUCT_PTR_ERR,
    NULL_STK_DATA_PTR_ERR,
    NEG_STK_CAPACITY_ERR,
    STACK_ANTIOVERFLOW_ERR,
    STACK_OVERFLOW_ERR,
    OUT_OF_MEMORY_ERR,
    STACK_USES_MUCH_MEM_ERR,
    STKDATA_INFO_CORRUPT_ERR,
    STKSTRUCT_INFO_CORRUPT_ERR,
};

struct stack_t
{
    canary_t left_canary;
    ON_DEBUG(const char* stk_name);
    ON_DEBUG(const char* init_file);
    ON_DEBUG(int init_line);
    ON_DEBUG(const char* init_func);
    unsigned long hash_struct;
    unsigned long hash_data;
    StackElem_t* data;
    int index;
    int capacity;
    canary_t right_canary;
};

const size_t DEFAULT_STK_CAPACITY = 8;
const int RESIZE_COEF      = 2;
const int RESIZE_COEF_DOWN = RESIZE_COEF * 2;
const canary_t STACK_CANARY_VALUE = 0xBAD57ACCBAD57ACC;
const canary_t DATA_CANARY_VALUE  = 0xBADDA7A0BADDA7A0;
const size_t SIZE_OF_CANARY       = sizeof(canary_t);

void      StackDtor      (stack_t* stk);
CodeError StackHash      (stack_t* stk);
CodeError StackHashData  (stack_t* stk);
CodeError StackHashStruct(stack_t* stk);
CodeError StackResizeDown(stack_t* stk);
CodeError StackResizeUp  (stack_t* stk);
CodeError StackInit      (stack_t* stk);
CodeError StackPop       (stack_t* stk, StackElem_t* var);
CodeError StackPush      (stack_t* stk, StackElem_t value);

#endif
