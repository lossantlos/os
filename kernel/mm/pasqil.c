
#include <stdint.h>
/*
typedef struct mm_data_struct {
    void *ptr;
    void *ptr_end;
    uint8_t flags;
    struct mm_data_struct *prev;
    struct mm_data_struct *next;
} mm_data_t;

extern uint32_t *kernel_end;

mm_data_t _t;
mm_data_t *mem = &t;

void mm_init() {
    printf("Initializing MM\n");
    printf("kernel_end=%x\n", &kernel_end);
    mem->ptr = kernel_end;
    mem->ptr_end = kernel_end + 0x100000; //TODO fixed size 100MB
    mem->prev = NULL;
    mem->next = NULL;
}


void *kmalloc(uint32_t size)
{
    mm_data_t *a = mem;
    while()
    {
        if((a->ptr - a->ptr_end) > size)
        {

        }
    }
}

void kfree(uint32_t *ptr)
{

}
*/



#include <kernel/kernel.h>
#include <stdlib.h>

typedef struct mm_phys {
	void *ptr;
	unsigned long size;
	struct mm_phys *next;
	struct mm_phys *prev;
} mm_phys_t;

mm_phys_t _a;
mm_phys_t *a = &_a;

void mm_phys_add(void *begin, unsigned long size, int flag)
{
//	printf("mm_phys_add %p (size: %i)\n", begin, size);
}

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
