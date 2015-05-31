
#include "../include/stdio.h"
#include "../include/stdlib.h"
#include "../include/math.h"

#include <kernel/tty.h>

int putchar(int c)
{
    tty_putchar(c);
    #warning TODO
}

int puts(const char *s)
{
    for(int x = 0; s[x]; x++) putchar(s[x]);
}


int putint(int n, int base)
{
    int val = n;
    static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
    puts(&buf[i+1]);
}
