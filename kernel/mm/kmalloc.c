/*
#include <kernel/kernel.h>
#include <stdlib.h>

extern uint32_t *kernel_end;
uint32_t *mem_last = NULL;

void mm_init() {
	mem_last = &kernel_end; //0xC0005000
}

void *kmalloc(uint32_t size)
{
	if(!mem_last) panic("MM does not know where the kernel ends");
	mem_last += size;
	return mem_last - size;
}
*/
