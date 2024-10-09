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

#endif
