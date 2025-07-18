/*!
    \file
    File with utilities
*/

#ifndef STACK_UTILS_H
#define STACK_UTILS_H

#include <stdint.h>

#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"

/*!
    Checks whether two doubles are equal or not
    \param[in]  num1  First number to compare
    \param[in]  num1  Second number to compare
    \return True (if numbers are equal), false (otherwise)
*/
bool IsEqual(double num1, double num2);

/*!
    Gets random number
    \return Random number as an unsigned long long
*/
long long unsigned int MyGetRandom64();

/*!
    Hashes info using DJB2 algorithm
    \param[in]  ptr              Pointer to the first byte
    \param[in]  number_of_bytes  Number of bytes that should be hashed
    \return Hash <number_of_butes> elements with the first one on <ptr>
*/
unsigned long MyHash(void* ptr, int number_of_bytes);

#endif
