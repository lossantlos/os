
#include <string.h>

#warning TODO rewrite!

void *memcpy(void *dest, void *src, size_t n)
{
    char *d = (char*) dest;
    char *s = (char*) src;
    for(unsigned int i = 0; i < n; i++) d[i] = s[i];
    return (void *) d;
}
