
#include <stddef.h>

#include <kernel/fs.h>

#include <kernel/syscall.h>
#include <kernel/regs.h>

typedef int32_t ssize_t;

ssize_t sys_write(int fd, const void *buf, size_t count)
{
    ssize_t ret = -1;
    if(fds[fd]->write)
		ret = fds[fd]->write(fds[fd], buf, count);
    return ret;
}

void sys_exit(int x)
{
    #warning TODO
}

void sys_none() {};

void *syscall[] = {
    &sys_none,
    &sys_exit,
    0,
    0,
//    &sys_fork,
//    &sys_read,
    &sys_write

};

void syscall_handler(struct regs *r)
{
	if(r->eax >= sizeof(syscall) / sizeof(typeof(syscall))) return;

    uint32_t ret = 0;

	__asm__ volatile(
		"push %1; "
		"push %2; "
		"push %3; "
		"push %4; "
		"push %5; "
		"call *%6; "
        "addl $20, %%esp; " // 5x pop %%ebx
        "movl %%eax, %0; "
		: "=m" (ret)
        : "r" (r->edi), "r" (r->esi), "r" (r->edx), "r" (r->ecx), "r" (r->ebx), "m" (syscall[r->eax])
        : "eax", "cc");

	r->eax = ret;
}

void syscall_init()
{
    irq_handler_set(0x80, &syscall_handler);
}
