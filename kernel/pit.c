#include <kernel/regs.h>
#include <kernel/irq.h>

volatile static unsigned int tt = 0; //pic ticks

void pit_handler(struct regs *r)
{
    tt++; //18.222Hz
}

void pit_init()
{
    irq_handler_set(IRQ0, pit_handler);
}

void pit_ticks()
{
    return tt;
}

void pit_reset()
{
    tt = 0;
}

void pit_wait(int ticks)
{
    #warning TODO repair BUG caused by variable overflow
    for(unsigned int t = tt + ticks; tt < t;);
}
