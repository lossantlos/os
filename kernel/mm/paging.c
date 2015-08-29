
#include <stdint.h>

extern uint32_t pd[1024];

extern void loadPageDirectory(unsigned int*);
/*extern*/ void paging_enable();

/*void loadPageDirectory(unsigned int *pd)
{
    __asm__ volatile (
        "push %ebp; "
    	"mov %esp, %ebp; "
    	"mov 8(%esp), %eax; "
    	"mov %eax, %cr3; "
    	"mov %ebp, %esp; "
    	"pop %ebp"
    );

}*/

/*void paging_enable()
{
    __asm__ volatile (
        "push %ebp; "
	    "mov %esp, %ebp; "
	    "mov %cr0, %eax; "
	    "or $0x80000000, %eax; "
      	"mov %eax, %cr0; "
    	"mov %ebp, %esp; "
	    "pop %ebp; ");
}*/

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

#include <kernel/regs.h>
#include <kernel/asm.h>

#define panic(msg) {printf("KERNEL PANIC!!! %s:%i: %s\n", __FILE__, __LINE__, msg); cli(); for(;;);};

void *page_fault(struct regs *r)
{
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r" (faulting_address));
    printf("page fault at 0x%x (", faulting_address);

    if(!(r->err_code & 0x1)) printf("present ");
    if(r->err_code & 0x2) printf("read-only ");
    if(r->err_code & 0x4) printf("user-mode ");
    if(r->err_code & 0x8) printf("present ");

    printf("\b)\n");
    panic("Page fault");
}

void cpuid(uint32_t op, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

void paging_init()
{
/*    //set each entry to not present
    for(int a = 0; a < 1024; a++)
        page_directory[a] = 0x00000002; //flags: supervisor: only kernel-mode, r/w, page not present
*/
/*
    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(unsigned int i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }

    // attributes: supervisor level, read/write, present
    page_directory[0] = ((unsigned int)first_page_table) | 3;
*/

/*    // Map 0-896MB to 3072-3968
    for(uint32_t i = 0; i < 224; i++)
    {
        page_directory[i] = (i * 4096 * 1024) | (1<<7) | (1<<1) | 1; // 4MB; Writable; Present
        page_directory[i + 768] = (i * 4096 * 1024) | (1<<7) | (1<<1) | 1; // 4MB; Writable; Present
    }*/


/*    uint32_t higher_half[1024] __attribute__((aligned(4096)));
    for(unsigned int i = 0; i < 1024; i++)
        higher_half[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    page_directory[768] = ((unsigned int)first_page_table) | 3;*/

    irq_handler_set(14, page_fault);


//    map_page(0x1000, 0xC0000000, 0);

//    loadPageDirectory(page_directory);
//    paging_enable();

    /*uint32_t *ptr = (uint32_t*)0x0a000000;
    uint32_t do_page_fault = *ptr;
    printf("%i\n", do_page_fault);*/

}


void map_page(void *physaddr, void *virtualaddr, uint32_t flags)
{
}
