
#include "../include/stdio.h"
#include "../include/stdlib.h"
#include "../include/math.h"
#include "../include/string.h"

#include <kernel/tty.h>
#include <stdint.h>

int putchar(int c)
{
//    write(1, c, 1);
    tty_putchar(c);
    #warning TODO
}

/*
/**
@brief Writes the character to stream.
@param c character to be written (later casted to an unsigned char)
@param stream File stream where to write
* /
int fputc(int c, FILE *stream) {
    //cast c to (unsigned char)
}

/**


* /
int fputs(const char *s, FILE *stream) {
    for(int x = 0; s[x]; x++) fputc(s[x], stream);
}

/**


* /
int putc(int c, FILE *stream) {

}

/**


* /
int putchar(int c) {
    fputc(c, stdio);
}

int puts(const char *s) {
    fputs(s, stdout);
    fputc('\n', stdout);
}
*/

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
