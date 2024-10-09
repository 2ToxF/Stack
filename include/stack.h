/*!
    \file
    File with constructions for main work with stack
*/

#ifndef STACK_H
#define STACK_H

#include "debug_mode.h"

#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"

#ifndef NDEBUG
/// @brief Is replaced with it's arguements only in debug mode
#define ON_DEBUG(...) __VA_ARGS__
/// @brief Creates stack structure depending on debug mode
#define CREATE_STACK(stk_enc_ptr) StackInit(stk_enc_ptr, #stk_enc_ptr, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#else
/// @brief Is replaced with it's arguements only in debug mode
#define ON_DEBUG(...)
/// @brief Creates stack structure depending on debug mode
#define CREATE_STACK(stk_enc_ptr) StackInit(stk_enc_ptr, NULL, NULL, NULL, NULL)
#endif

/// @brief Type of stack elements
typedef int StackElem_t;

/// @brief Enumerated types of code errors or 0 for "no error"-state
enum CodeError
{
    NO_ERROR,
    CANT_CREATE_RAND_NUM_ERR,
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
struct stack_t;

/*! -----------------------------------------------------------------------------------------------------
    Destruct stack
    \param[in, out]  stk_enc_ptr  Encoded pointer to stack sructure
    ----------------------------------------------------------------------------------------------------- */
CodeError StackDtor      (size_t* stk_enc_ptr);

/*! -----------------------------------------------------------------------------------------------------
    Initializes stack
    \param[in, out]  stk_enc_ptr    Encoded pointer to stack sructure
    \param[in]       stk_name       Name of stack pointer variable
    \param[in]       stk_init_file  Name of file where stack is initialized
    \param[in]       stk_init_line  Number of line where stack is initialized
    \param[in]       stk_init_func  Name of function where stack is initialized
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackInit      (size_t* stk_enc_ptr, const char* stk_name, const char* stk_init_file,
                          int stk_init_line, const char* stk_init_func);

/*! -----------------------------------------------------------------------------------------------------
    Extractes value from stack
    \param[in]   stk_enc_ptr  Encoded pointer to stack sructure
    \param[out]  var          Pointer to variable where extracted value should be put
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackPop       (size_t stk_enc_ptr, StackElem_t* var);

/*! -----------------------------------------------------------------------------------------------------
    Extractes value from stack
    \param[in]  stk_enc_ptr Encoded pointer to stack sructure
    \param[in]  value       Value that should be put to stack
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackPush      (size_t stk_enc_ptr, StackElem_t value);

#endif
