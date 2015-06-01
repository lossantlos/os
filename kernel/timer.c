#include <kernel/regs.h>
#include <kernel/irq.h>

volatile static unsigned int tt = 0; //timer ticks

void timer_handler(struct regs *r)
{
    tt++; //18.222Hz
}

void timer_init()
{
    irq_handler_set(0, timer_handler);
}

void timer_ticks()
{
    return tt;
}

void timer_reset()
{
    tt = 0;
}

void timer_wait(int ticks)
{
    #warning TODO repair BUG caused by variable overflow
    for(unsigned int t = tt + ticks; tt < t;);
}
