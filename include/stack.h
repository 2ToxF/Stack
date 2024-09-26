#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#ifndef NDEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...)
#endif

#ifndef NDEBUG
#define CREATE_STACK(stk) {#stk, __FILE__, __LINE__, __func__};
#else
#define CREATE_STACK(stk) {}
#endif

typedef int StackElem_t;

enum CodeError
{
    NO_ERROR,
    STACK_ALREADY_INITED_ERR,
    NULL_STK_STRUCT_PTR_ERR,
    NULL_STK_DATA_PTR_ERR,
    NEG_STK_CAPACITY_ERR,
    STACK_ANTIOVERFLOW_ERR,
    STACK_OVERFLOW_ERR,
    STACK_USES_MUCH_MEM_ERR,
};

struct stack_t
{
    ON_DEBUG(const char* st_name);
    ON_DEBUG(const char* init_file);
    ON_DEBUG(int init_line);
    ON_DEBUG(const char* init_func);
    StackElem_t* data;
    int index;
    int capacity;
};

const size_t DEFAULT_STK_CAPACITY = 8;
const int RESIZE_COEF_UP   = 2;
const int RESIZE_COEF_DOWN = 4;

void StackDtor(stack_t* stk);
CodeError StackResizeDown(stack_t* stk);
CodeError StackResizeUp  (stack_t* stk);
CodeError StackInit(stack_t* stk);
CodeError StackPop (stack_t* stk, StackElem_t* var);
CodeError StackPush(stack_t* stk, StackElem_t value);

#endif
