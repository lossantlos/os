
#include <kernel/syscall.h>


#include <kernel/regs.h>

static void test()
{
    printf("trol\n");
}

static void *syscall[] = {
    &test
};

void syscall_handler(struct regs *r)
{
	if(r->eax >= sizeof(syscall) / sizeof(typeof(syscall))) return;

	void *loc = syscall[r->eax];

	uint32_t ret;

	__asm__ (
		"push %1; "
		"push %2; "
		"push %3; "
		"push %4; "
		"push %5; "
		"call *%6; "
		"pop %%ebx; "
		"pop %%ebx; "
		"pop %%ebx; "
		"pop %%ebx; "
		"pop %%ebx; "

		: "=a" (ret) : "r" (r->edi), "r" (r->esi), "r" (r->edx),
		"r" (r->ecx), "r" (r->ebx), "r" (loc));

	r->eax = ret;
}

void syscall_init()
{
    irq_handler_set(0x80, &syscall_handler);
}
