
#ifndef _STDLIB_H
#define _STDLIB_H

#include <stdint.h>
#include <stddef.h>

#define NULL ((void *)0)

#define char2int(x) (x - '0')
#define int2char(x) (x + '0')


int atoi(const char *s); //string to number

//memory
void *malloc(size_t n);
void free(void *ptr);
void *calloc(size_t n, size_t s);
void *realloc(void *ptr, size_t n);



#define getbit (n, x) ((n & (1 << x)) ? 1 : 0)

#endif
