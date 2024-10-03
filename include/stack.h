/*!
    \file
    File with constructions for main work with stack
*/

#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#include "debug_mode.h"

#ifndef NDEBUG
/// @brief Is replaced with it's arguements only in debug mode
#define ON_DEBUG(...) __VA_ARGS__
/// @brief Creates stack structure depending on debug mode
#define CREATE_STACK(stk) {0, #stk, __FILE__, __LINE__, __func__}
#else
/// @brief Is replaced with it's arguements only in debug mode
#define ON_DEBUG(...)
/// @brief Creates stack structure depending on debug mode
#define CREATE_STACK(stk) {}
#endif

/// @brief Type of stack elements
typedef int StackElem_t;

/// @brief Type of canaries on the stack sides
typedef uint64_t canary_t;

/// @brief Enumerated types of code errors or 0 for "no error"-state
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
    STKSTRUCT_CANARY_CORRUPT_ERR,
    STKDATA_CANARY_CORRUPT_ERR,
    STKSTRUCT_INFO_CORRUPT_ERR,
    STKDATA_INFO_CORRUPT_ERR,
};

/// @brief Sructure with stack info
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

/// @brief Default number of elements that can be put to stack
const size_t DEFAULT_STK_CAPACITY = 8;

/// @brief Coefficeint for upsizing stack
const int RESIZE_COEF      = 2;

/// @brief Coefficient that sets ratio of minimum and maximum number of elements (except default capacity)
const int RESIZE_COEF_DOWN = RESIZE_COEF * 2;

/// @brief Canary value for securing stack structure
const canary_t STACK_CANARY_VALUE = 0xBAD57ACCBAD57ACC;

/// @brief Canary value for securing stack elements
const canary_t DATA_CANARY_VALUE  = 0xBADDA7A0BADDA7A0;

/// @brief Size of all canaries
const size_t SIZE_OF_CANARY       = sizeof(canary_t);

/*! -----------------------------------------------------------------------------------------------------
    Destruct stack
    \param[in, out]  stk  Pointer to stack sructure
    ----------------------------------------------------------------------------------------------------- */
void      StackDtor      (stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Calculates both structure and data (elements) hashes of stack
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackHash      (stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Calculates stack data (elements) hash
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackHashData  (stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Calculates stack structure hash
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackHashStruct(stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Initializes stack
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackInit      (stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Extractes value from stack
    \param[in]   stk  Pointer to stack sructure
    \param[out]  var  Pointer to variable where extracted value should be put
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackPop       (stack_t* stk, StackElem_t* var);

/*! -----------------------------------------------------------------------------------------------------
    Extractes value from stack
    \param[in]  stk    Pointer to stack sructure
    \param[in]  value  Value that should be put to stack
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackPush      (stack_t* stk, StackElem_t value);

/*! -----------------------------------------------------------------------------------------------------
    Downsizes the stack
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackResizeDown(stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Upsizes the stack
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackResizeUp  (stack_t* stk);

#endif
