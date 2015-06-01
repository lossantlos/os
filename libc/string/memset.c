
#include "../include/string.h"

void *memset(void *dest, const int c, size_t n)
{
    unsigned char *d = (unsigned char *) dest;
    for(size_t x = 0; x < n; x++) d[x] = (unsigned char) c;
    return dest;
}
