
#include <stdlib.h>


uint32_t mem_last = 0xC0005000;
void *kmalloc(uint32_t size)
{
	mem_last += size;
	return mem_last - size;
}
