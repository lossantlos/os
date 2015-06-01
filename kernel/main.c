#include <stdint.h>
#include <stddef.h>

#include <kernel/asm.h>

#include <kernel/tty.h>
#include <stdlib.h>

#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_early()
{
	cli();
	vga_init();
	tty_init();
	gdt_init();
	idt_init();
	isrs_init();
	irq_init();
	timer_init();
	sti();
}

void keyboard_handler(struct regs *r)
{
    unsigned char scancode = inb(0x60);
	printf(".");
}


void kernel_main()
{
	printk("Initialized!\n");
	beep();

	irq_handler_set(1, keyboard_handler);

	while(1);
}
