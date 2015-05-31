#include "../include/stdlib.h"
#include "../include/math.h"


//string to number
int atoi(const char *s)
{
    int t = 0;
    const size_t l = strlen(s);
    for(int x = l; x > 0; x--) t += char2int(s[x]) * pow(10, l - x);
    return t;
}
