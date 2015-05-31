
#include "../include/math.h"

#include <stdint.h>
#include <stddef.h>

int pow(int base, int exp)
{
    int r = 1; //result
    for (size_t i = 0; i < exp; i++) r *= base;
    return r;
}
