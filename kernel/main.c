#include <stdint.h>
#include <stddef.h>

#include <kernel/tty.h>
#include <stdlib.h>

void kernel_early()
{
	vga_init();
	tty_init();
}

void kernel_main()
{
	printk("Initialized!\n\n");


	int a = 100;
	printf("2\t%b\n8\t%o\n10\t%i\n16\t%x\nptr\t%p\n", a, a, a, a, &a);
}
