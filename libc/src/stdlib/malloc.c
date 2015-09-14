
#warning TODO

#include <stdint.h>

#define size_t uint32_t

void *malloc(size_t size)
{
    return kmalloc(size);
}
/*
void free(void *ptr)
{

}

void *calloc(size_t nmemb, size_t size)
{
    return memset(malloc(nmemb * size), 0, nmemb * size);
}

void *realloc(void *ptr, size_t size)
{
    //malloc
    //get size of previous allocated memory
    //memcpy
    //free
    //return
}
*/
