#include <kernel/regs.h>
#include <kernel/irq.h>

volatile static unsigned int tt = 0; //pic ticks

void pic_handler(struct regs *r)
{
    tt++; //18.222Hz
}

void pic_init()
{
    irq_handler_set(IRQ0, pic_handler);
}

void pic_ticks()
{
    return tt;
}

void pic_reset()
{
    tt = 0;
}

void pic_wait(int ticks)
{
    #warning TODO repair BUG caused by variable overflow
    for(unsigned int t = tt + ticks; tt < t;);
    __asm__("int $0x81");
}
