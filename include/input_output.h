/*!
    \file
    File with functions and macros for input and output
*/

#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "debug_mode.h"
#include "stack.h"

/*!
    Prints result of program running
    \param[in]  code_error  Type of code error or "no error"-state
*/
void print_code_error(CodeError code_error);

#ifndef NDEBUG
    /*!
        Prints stack info (ONLY DEFINED IN DEBUG MODE)
        \param[in]  stk          Pointer to structure of stack that should be printed
        \param[in]  file_name    Name of file where function was called
        \param[in]  line_number  Number of line where function was called
    */
    void StackDump(const stack_t* stk, const char* file_name, int line_number);
#endif

#endif
