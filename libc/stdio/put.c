
#include "../include/stdio.h"
#include "../include/stdlib.h"
#include "../include/math.h"

#include <kernel/tty.h>
#include <stdint.h>

int putchar(int c)
{
    tty_putchar(c);
    #warning TODO
}

int puts(const char *s)
{
    for(int x = 0; s[x]; x++) putchar(s[x]);
}

int putint(uint32_t n, uint8_t base)
{
    if (n==0) {
        putchar('0');
        return;
    }
    int val = n;
    static char buf[11] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
    puts(&buf[i+1]);
}
