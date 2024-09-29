#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#ifndef NDEBUG
#define ON_DEBUG(...) __VA_ARGS__
#define CANARY_STACK_VALUE 0xBAD57ACCBAD57ACC
#define CANARY_DATA_VALUE  0xBADDA7A0BADDA7A0
#define CREATE_STACK(stk) {CANARY_STACK_VALUE, #stk, __FILE__, __LINE__, __func__, 0, 0, 0, 0, 0, CANARY_STACK_VALUE}
#define STACK_HASH(stk) StackHash(stk)
#else
#define ON_DEBUG(...)
#define CREATE_STACK(stk) {}
#define STACK_HASH(stk)
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
    OUT_OF_MEMORY_ERR,
    STACK_USES_MUCH_MEM_ERR,
    STKSTRUCT_INFO_CORRUPT_ERR,
    STKDATA_INFO_CORRUPT_ERR,
};

struct stack_t
{
    ON_DEBUG(uint64_t canary_left);
    ON_DEBUG(const char* stk_name);
    ON_DEBUG(const char* init_file);
    ON_DEBUG(int init_line);
    ON_DEBUG(const char* init_func);
    ON_DEBUG(unsigned long hash_struct);
    ON_DEBUG(unsigned long hash_data);
    StackElem_t* data;
    int index;
    int capacity;
    ON_DEBUG(uint64_t canary_right);
};

const size_t DEFAULT_STK_CAPACITY = 8;
const int RESIZE_COEF      = 2;
const int RESIZE_COEF_DOWN = RESIZE_COEF * 2;
static const int SIZE_OF_CANARY = sizeof(uint64_t);

void StackDtor           (stack_t* stk);
CodeError StackHash      (stack_t* stk);
CodeError StackHashData  (stack_t* stk);
CodeError StackHashStruct(stack_t* stk);
CodeError StackResizeDown(stack_t* stk);
CodeError StackResizeUp  (stack_t* stk);
CodeError StackInit      (stack_t* stk);
CodeError StackPop       (stack_t* stk, StackElem_t* var);
CodeError StackPush      (stack_t* stk, StackElem_t value);

#endif
