#ifndef _STRINGS_H
#define _STRINGS_H

#warning external headers
#include <stdint.h>
#include <stddef.h>

//memory
int memcmp(void *ptr1, void *ptr2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *dest, const char c, size_t n);

//string
int strcmp(char *s1, char *s2);
//void *strcpy();

size_t strlen(const char *s);

#endif
