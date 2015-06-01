#include <kernel/regs.h>

#include <stdint.h>
#include <stddef.h>

void irq_handler_set(int irq, void (*handler)(struct regs *r));
void irq_handler_free(int irq);
void irq_init();
