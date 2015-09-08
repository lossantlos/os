
#include <string.h>

#warning TODO rewrite strcpy!

void *strcpy(void *dest, void *src)
{
    char *d = (char*) dest;
    char *s = (char*) src;
    for(unsigned int i = 0; s[i]; i++) d[i] = s[i];
    return (void *) d;
}
