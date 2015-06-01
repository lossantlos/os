
#include <kernel/idt.h>

struct idt_entry idt[256];
struct idt_ptr idtp;

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

void idt_init() //installs the IDT
{
    //sets the special IDT pointer up
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256); //clear IDT

    //Since this moment add any new ISRs to the IDT idt_set_gate

    //set up new IDT
    __asm__ __volatile__ ( "lidt %0 \n" : : "m"(idtp) );
}
