#ifndef _STRING_H
#define _STRING_H

#warning external headers
#include <stdint.h>
#include <stddef.h>

//memory
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *dest, const int c, size_t n);

//string
int strcmp(const char *s1, const char *s2);
void *strcpy(void *dest, void *src);

size_t strlen(const char *s);

#endif
