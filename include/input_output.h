/*!
    \file
    File with functions and macros for input and output
*/

#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "settings.h"

#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"

/*! -----------------------------------------------------------------------------------------------------
    Prints result of program running
    \param[in]  code_error  Type of code error or "no error"-state
    ----------------------------------------------------------------------------------------------------- */
void PrintCodeError(CodeError code_error);

#endif
