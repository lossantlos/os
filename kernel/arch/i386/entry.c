#include <stdint.h>

uint32_t pd[1024] __attribute__ ((section(".data_phys")));

void paging_prepare(void) __attribute__ ((section(".text_phys")));
void paging_prepare(void)
{
    // Set all pages to non-present
    for(int a = 0; a < 1024; a++)
        pd[a] = 0x00000002; //flags: supervisor: only kernel-mode, r/w, page not present

    // Map 0-896MB to 3072-3968
    for(uint32_t i = 0; i < 224; i++) {
        pd[i] = (i * 4096 * 1024) | (1<<7) | (1<<1) | 1; // 4MB; Writable; Present
        pd[i + 768] = (i * 4096 * 1024) | (1<<7) | (1<<1) | 1; // 4MB; Writable; Present
    }
}
/*
uint32_t pd[1024] __attribute__ ((section(".data_phys")));

uint64_t page_dir_ptr_tab[4] __attribute__((aligned(0x20), section(".data_phys")));

uint64_t page_dir[512] __attribute__((aligned(0x1000), section(".data_phys")));
uint64_t page_tab[512] __attribute__((aligned(0x1000), section(".data_phys")));

void paging_prepare(void) __attribute__ ((section(".text_phys")));
void paging_prepare(void)
{
    page_dir_ptr_tab[0] = (uint64_t)&page_dir | 1;
    page_dir[0] = (uint64_t)&page_tab | 3;

//    unsigned int i, address = 0;
//    for(i = 0; i < 512; i++)
//    {
//        page_tab[i] = address | 3; // map address and mark it present/writable
//        address = address + 0x1000;
//    }

    unsigned int i, address = 0;
    for(i = 0; i < 512; i++)
    {
        page_tab[i] = address | 3; // map address and mark it present/writable
        address = address + 0x1000;
    }

    __asm__ volatile ("movl %cr4, %eax; bts $5, %eax; movl %eax, %cr4");
    __asm__ volatile ("movl %%eax, %%cr3" :: "a" (&page_dir_ptr_tab));
    __asm__ volatile ("movl %cr0, %eax; orl $0x80000000, %eax; movl %eax, %cr0;");
}
*/
