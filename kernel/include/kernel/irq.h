#ifndef _IRQ_H
#define _IRQ_H


#include <kernel/regs.h>

#include <stdint.h>
#include <stddef.h>

void irq_handler_set(int irq, void (*handler)(struct regs *r));
void irq_handler_free(int irq);
void irq_init();

#define IRQ0    32 //pit
#define IRQ1    33 //keyboard
#define IRQ2    34 //reserved
#define IRQ3    35 //COM2
#define IRQ4    36 //COM1
#define IRQ5    37 //LPT2
#define IRQ6    38 //floppy
#define IRQ7    39 //LPT1
#define IRQ8    40 //CMOS
#define IRQ9    41 //Free for peripherals
#define IRQ10   42 //Free for peripherals
#define IRQ11   43 //Free for peripherals
#define IRQ12   44 //PS2 mouse
#define IRQ13   45 //FPU
#define IRQ14   46 //Primary ATA hdd
#define IRQ15   47 //Secondary ATA hdd

#endif
