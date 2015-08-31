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
