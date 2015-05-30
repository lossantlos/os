
#ifndef _STDLIB_H
#define _STDLIB_H

#define NULL ((void *)0)

int atoi(const char *s); //string to number

//memory
void *malloc(size_t n);
void free(void *ptr);
void *calloc(size_t n, size_t s);
void *realloc(void *ptr, size_t n);

#endif
