/*!
    \file
    File with constructions for main work with stack
*/

#ifndef STACK_H
#define STACK_H



#ifndef NDEBUG
/// @brief Is replaced with it's arguements only in debug mode
#define ON_DEBUG(...) __VA_ARGS__
/// @brief Creates stack structure depending on debug mode
#define CREATE_STACK(stk) StackInit(stk, #stk, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#else
/// @brief Is replaced with it's arguements only in debug mode
#define ON_DEBUG(...)
/// @brief Creates stack structure depending on debug mode
#define CREATE_STACK(stk) StackInit(stk, NULL, NULL, NULL, NULL)
#endif

/// @brief Type of stack elements
typedef int StackElem_t;

/// @brief Enumerated types of code errors or 0 for "no error"-state
enum CodeError
{
    NO_ERROR,
    IMPOSSIBLE_STACK_INIT_ERR,
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
    \param[in, out]  stk  Pointer to stack sructure
    ----------------------------------------------------------------------------------------------------- */
CodeError StackDtor      (stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Initializes stack
    \param[in, out]  stk            Pointer to stack sructure
    \param[in]       stk_name       Name of stack pointer variable
    \param[in]       stk_init_file  Name of file where stack is initialized
    \param[in]       stk_init_line  Number of line where stack is initialized
    \param[in]       stk_init_func  Name of function where stack is initialized
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
CodeError StackInit      (stack_t* stk, const char* stk_name, const char* stk_init_file,
                          int stk_init_line, const char* stk_init_func)

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

#endif
