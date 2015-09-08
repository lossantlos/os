
#include <kernel/idt.h>

struct idt_entry idt[256];
struct idt_ptr idtp;

/**
@brief Load new IDT
*/
void idt_load(const struct idt_ptr* ptr)
{
    __asm__ volatile("lidt %0 \n" : : "m"(*ptr));
}

/**
@brief Set IDT gate
@param num   Number of entry
@param base  Address of interrupt service rountine
@param sel   Code segment selector
@param flags Type and attributes
*/
void idt_set_gate(uint8_t num, unsigned long base, unsigned short sel, uint8_t flags)
{
    /* The interrupt routine's base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/**
@brief Initialize IDT
*/
void idt_init()
{
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256); //clear IDT
    idt_load(&idtp);
}
