/*!
    \file
    File with constructions for main work with stack
*/

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "utils.h"

/// @brief Type of canaries on the stack sides
typedef uint64_t canary_t;

/// @brief Key to find real pointer using XOR
static size_t key_for_ptr_dec = 0;

/// @brief Default number of elements that can be put to stack
static const size_t DEFAULT_STK_CAPACITY = 512;

/// @brief Coefficeint for upsizing stack
static const int RESIZE_COEF      = 2;

/// @brief Coefficient that sets ratio of minimum and maximum number of elements (except default capacity)
static const int RESIZE_COEF_DOWN = RESIZE_COEF * 2;

/// @brief Canary value for securing stack structure
static const canary_t STACK_CANARY_VALUE = 0xBAD57ACCBAD57ACC;

/// @brief Canary value for securing stack elements
static const canary_t DATA_CANARY_VALUE  = 0xBADDA7A0BADDA7A0;

/// @brief Size of all canaries
static const size_t SIZE_OF_CANARY       = sizeof(canary_t);

/// @brief Number of bits temporary hash should shift by
static const int           HASH_SHIFT_COEF = 5;

/// @brief Start value of temporary hash
static const unsigned long START_HASH      = 5381;

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

/*! -----------------------------------------------------------------------------------------------------
    Calculates both structure and data (elements) hashes of stack
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
static CodeError StackHash      (stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Calculates stack data (elements) hash using DJB2-algorithm
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
static CodeError StackHashData  (stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Calculates stack structure hash using DJB2-algorithm
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
static CodeError StackHashStruct(stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Calculates the real pointer to stack structure using XOR with key_for_decode
    \param[in]  ptr_do_decode  Encoded (decoded) pointer to stack sructure
    \return Decoded (encoded) pointer to stack structure
    ----------------------------------------------------------------------------------------------------- */
static size_t    StackPtrXOR    (size_t ptr_to_decode);

/*! -----------------------------------------------------------------------------------------------------
    Downsizes the stack
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
static CodeError StackResizeDown(stack_t* stk);

/*! -----------------------------------------------------------------------------------------------------
    Upsizes the stack
    \param[in, out]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
    ----------------------------------------------------------------------------------------------------- */
static CodeError StackResizeUp  (stack_t* stk);

/*!
    Verifies stack's sructure and data (elements)
    \param[in]  stk  Pointer to stack sructure
    \return Type of code error or 0 for "no error"-state
*/
static CodeError StackVerify(stack_t* stk);


#ifndef NDEBUG
    /*!
        Prints stack info (ONLY DEFINED IN DEBUG MODE)
        \param[in]  stk          Pointer to structure of stack that should be printed
        \param[in]  file_name    Name of file where function was called
        \param[in]  line_number  Number of line where function was called
    */
    void StackDump(const stack_t* stk, const char* file_name, int line_number);

    /// @brief Macro for verifying stack
    #define STACK_VERIFY(stk)                                    \
        do                                                       \
        {                                                        \
            CodeError temp_code_err = NO_ERROR;                  \
            if ((temp_code_err = StackVerify(stk)) != NO_ERROR)  \
            {                                                    \
                if (temp_code_err >= STACK_ANTIOVERFLOW_ERR)     \
                    StackDump(stk, __FILE__, __LINE__);          \
                return temp_code_err;                            \
            }                                                    \
        } while(0)
#else
    /// @brief Macro for verifying stack
    #define STACK_VERIFY(stk)                                    \
        do {                                                     \
            CodeError temp_code_err = NO_ERROR;                  \
            if ((temp_code_err = StackVerify(stk)) != NO_ERROR)  \
                return temp_code_err;                            \
        } while(0)
#endif


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//----------------------------------------------------------------------------------------------------------------
// ------> !!! STACK PART !!! <-----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


CodeError StackDtor(size_t* stk_enc_ptr)
{
    stack_t* stk = (stack_t*) StackPtrXOR(*stk_enc_ptr);

    STACK_VERIFY(stk);

    free((char*) stk->data - SIZE_OF_CANARY); stk->data = NULL;
    stk->index = 0;
    stk->capacity = 0;
    free(stk); stk = NULL;

    *stk_enc_ptr = 0;

    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

static CodeError StackHash(stack_t* stk)
{
    CodeError code_err = NO_ERROR;

    if ((code_err = StackHashStruct(stk)) != NO_ERROR)
        return code_err;

    if ((code_err = StackHashData(stk)) != NO_ERROR)
        return code_err;

    return code_err;
}

//----------------------------------------------------------------------------------------------------------------------

static CodeError StackHashData(stack_t* stk)
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

static CodeError StackHashStruct(stack_t* stk)
{
    stk->hash_struct = 0;
    unsigned long temp_hash_data = stk->hash_data;
    stk->hash_data = 0;

    unsigned long calc_hash = START_HASH;
    char* cur_byte = (char*) stk;
    const size_t sizeof_stk_struct = sizeof(*stk);

    for (size_t i = 0; i < sizeof_stk_struct; i++)
    {
        calc_hash = ((calc_hash << HASH_SHIFT_COEF) + calc_hash) + *cur_byte;
        cur_byte++;
    }

    stk->hash_struct = calc_hash;
    stk->hash_data = temp_hash_data;
    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackInit(size_t* stk_enc_ptr, const char* stk_name, const char* stk_init_file,
                    int stk_init_line, const char* stk_init_func)
{
    stack_t* stk = (stack_t*) calloc(1, sizeof(stack_t));

    #ifndef NDEBUG
        if (stk->data != NULL || stk->index != 0 || stk->capacity != 0)
        {
            StackDump(stk, __FILE__, __LINE__);
            return STACK_ALREADY_INITED_ERR;
        }

        stk->stk_name = stk_name;
        stk->init_file = stk_init_file;
        stk->init_line = stk_init_line;
        stk->init_func = stk_init_func;
    #endif

    if (key_for_ptr_dec == 0)
    {
        if ((key_for_ptr_dec = MyGetRandom64()) == 0)
            return CANT_CREATE_RAND_NUM_ERR;
    }

    stk->left_canary = stk->right_canary = STACK_CANARY_VALUE;

    char* new_data = (char*) calloc(DEFAULT_STK_CAPACITY*sizeof(StackElem_t) + SIZE_OF_CANARY*2, sizeof(char));
    *((canary_t*) new_data) = DATA_CANARY_VALUE;
    *((canary_t*) (new_data + DEFAULT_STK_CAPACITY*sizeof(StackElem_t) + SIZE_OF_CANARY)) = DATA_CANARY_VALUE;

    stk->data = (StackElem_t*) (new_data + SIZE_OF_CANARY);
    stk->index = 0;
    stk->capacity = DEFAULT_STK_CAPACITY;

    StackHash(stk);
    STACK_VERIFY(stk);

    *stk_enc_ptr = StackPtrXOR((size_t) stk);

    return NO_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

CodeError StackPop(size_t stk_enc_ptr, StackElem_t* var)
{
    stack_t* stk = (stack_t*) StackPtrXOR(stk_enc_ptr);

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

CodeError StackPush(size_t stk_enc_ptr, StackElem_t value)
{
    stack_t* stk = (stack_t*) StackPtrXOR(stk_enc_ptr);

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

static size_t StackPtrXOR(size_t ptr_to_decode)
{
    return ptr_to_decode ^ key_for_ptr_dec;
}

//----------------------------------------------------------------------------------------------------------------------

static CodeError StackResizeDown(stack_t* stk)
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

static CodeError StackResizeUp(stack_t* stk)
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


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//----------------------------------------------------------------------------------------------------------------
// ------> !!! VERIFY PART !!! <----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


static CodeError StackVerify(stack_t* stk)
{
    #ifndef NDEBUG
        if (stk == NULL)
            return NULL_STK_STRUCT_PTR_ERR;
        if (stk->data == NULL)
            return NULL_STK_DATA_PTR_ERR;
        if (stk->capacity < 0)
            return NEG_STK_CAPACITY_ERR;

        if (stk->index < 0)
            return STACK_ANTIOVERFLOW_ERR;
        if (stk->index >= stk->capacity)
            return STACK_OVERFLOW_ERR;
        if (stk->index < stk->capacity / RESIZE_COEF_DOWN - 1 && (size_t) stk->capacity > DEFAULT_STK_CAPACITY)
            return STACK_USES_MUCH_MEM_ERR;
    #endif

    if (stk->left_canary != STACK_CANARY_VALUE || stk->right_canary != STACK_CANARY_VALUE)
        return STKSTRUCT_CANARY_CORRUPT_ERR;
    if (*((canary_t*) ((char*) stk->data - SIZE_OF_CANARY)) != DATA_CANARY_VALUE
        || *((canary_t*) (stk->data + stk->capacity)) != DATA_CANARY_VALUE)
        return STKDATA_CANARY_CORRUPT_ERR;

    unsigned long temp_hash_data = stk->hash_data, temp_hash_struct = stk->hash_struct;
    StackHash(stk);
    if (temp_hash_struct != stk->hash_struct)
        return STKSTRUCT_INFO_CORRUPT_ERR;
    if (temp_hash_data != stk->hash_data)
        return STKDATA_INFO_CORRUPT_ERR;

    return NO_ERROR;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//----------------------------------------------------------------------------------------------------------------
// ------> !!! INPUT/OUTPUT PART !!! <----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#ifndef NDEBUG
    void StackDump(const stack_t* stk, const char* file_name, int line_number)
    {
        int idx_first_of_same = 0;
        char index_string[12] = {};

        #define PRINT_CELLS_VALUE_(idx)                                             \
            {                                                                       \
                if (idx_first_of_same == (idx))                                     \
                    sprintf(index_string, "*[%d]", (idx));                          \
                else                                                                \
                    sprintf(index_string, "*[%d - %d]", idx_first_of_same, (idx));  \
                printf("\t\t%-12s  =  %d\n", index_string, stk->data[idx]);         \
                idx_first_of_same = (idx + 1);                                      \
            }


        printf(RED "stack_t " MAG "%s [0x%p] " BLU "at %s:%d " YEL "born at %s:%d (%s)\n",
            stk->stk_name, stk,
            file_name, line_number,
            stk->init_file, stk->init_line, stk->init_func);
        printf(GRN "{\n"
            "%-10s= %d\n"
            "%-10s= %d\n"
            "\n"
            MAG "\tdata [0x%p]:\n"
            CYN "\t{\n",
            "\tindex", stk->index,
            "\tcapacity", stk->capacity,
            stk->data);

        for (int i = 1; i < stk->capacity; i++)
        {
            if (stk->data[i] != stk->data[i-1])
                PRINT_CELLS_VALUE_(i-1);
        }
        PRINT_CELLS_VALUE_(stk->capacity-1);

        printf("\t}\n"
            GRN "}\n" WHT);

        #undef PRINT_CELLS_VALUE
    }
#endif
