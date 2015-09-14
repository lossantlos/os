
#include "../include/stdio.h"
#include "../include/stdlib.h"
#include "../include/math.h"
#include "../include/string.h"

#include <kernel/tty.h>
#include <stdint.h>


FILE *stdin;
FILE *stderr;

/**
@brief Open file stream
@param path Path of the file
@param mode How to access that
@return Pointer to file stream
*/
FILE *fopen(const char *path, const char *mode) {
    FILE *f = malloc(sizeof(FILE));
    #warning TODO

    return f;
}

/**
@brief Writes the character to stream.
@param c character to be written (later casted to an unsigned char)
@param stream File stream where to write
*/
int fputc(int c, FILE *stream) {
    //cast c to (unsigned char)
    tty_putchar(c);
}

/**
@brief write character s to stream (without \\0)
*/
int fputs(const char *s, FILE *stream) {
    for(int x = 0; s[x]; x++) fputc(s[x], stream);
}

/**
@brief Equivalent to fputc() except that it may be implemented as a macro which evaluates stream more than once.
*/
int putc(int c, FILE *stream) {
    #warning TODO remake
    return fputc(c, stream);
}

/**
@brief Equivalent to putc(c, stdout)
*/
int putchar(int c) {
    return fputc(c, stdout);
}

/**
@brief Write string
*///and \\n to stdout
int puts(const char *s) {
//    int ret = fputs(s, stdout);
//    if(ret < 0) return ret;
//    return fputc('\n', stdout);
    return fputs(s, stdout);
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
