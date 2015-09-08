
#include "../include/math.h"

#include <stdint.h>
#include <stddef.h>

/**
@brief Simply power number
@param x Base
@param y Exponent (power)
@return This function returns \f$x^y\f$
*/
int pow(int x, int y)
{
    int result = 1;
    for(int i = 0; i < y; i++) result *= x;
    return result;
}
