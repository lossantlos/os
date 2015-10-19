/**
@file main.c
*/

#include <stdint.h>
#include <stddef.h>

#include <kernel/asm.h>
#include <kernel/kernel.h>
#include <kernel/regs.h>

#include <kernel/tty.h>
#include <stdlib.h>

#include <kernel/gdt.h>
#include <kernel/idt.h>

#include "arch/i386/multiboot.h"

#include <tar.h>

extern char _binary_initrd_tar_start[];
extern char _binary_initrd_tar_end[];

#include <kernel/fs.h>



ssize_t tty_write(struct node_t *n, const void *buf, ssize_t nbyte)
{
	char *b = buf;
	ssize_t x;
	for (x = 0; x < nbyte; x++)
		putchar(b[x]);
	return x;
}


ssize_t vfs_tar_read(node_t *n, const void *buf, ssize_t nbyte)
{
	tar_file_read(_binary_initrd_tar_start, n->name, buf, nbyte);
	return 0;
}

node_t *fds[10];
uint32_t nodes = 0;

uint32_t open(const char *path, uint32_t oflag)
{
	node_t *n = kmalloc(sizeof(node_t));
	if(!strcmp(path, "/tty"))
	{
		n->name = kmalloc(strlen(path));
		strcpy(n->name, path);
		n->write = tty_write;
	} else {
		n->name = kmalloc(strlen(path));
		strcpy(n->name, path);
		n->read = vfs_tar_read;
	}
	fds[++nodes] = n;
	return nodes;
}



ssize_t write(uint32_t fd, const void *buf, ssize_t nbyte)
{
	if(fds[fd]->write)
		return fds[fd]->write(fds[fd], buf, nbyte);
}

ssize_t read(uint32_t fd, const void *buf, ssize_t nbyte)
{
	if(fds[fd]->read)
		return fds[fd]->read(fds[fd], buf, nbyte);
}

char *testt(uint32_t n, uint8_t base)
{
    if(n == 0) return "0";
    static char buf[11] = {0};
    uint32_t val = n;
	uint32_t i = sizeof(buf);
	for(; val && i ; --i, val /= base) buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
}

/*
uint32_t gs, fs, es, ds;      /* pushed the segs last
uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha'
uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this *
uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically *

*/

void regs_dump_handler(struct regs *r)
{
//	printf("esp=%s\n", testt(r->esp, 16));
	printf("\nRegisters dump:\n"
			"  EAX=%08x EBX=%08x ECX=%08x EDX=%08x\n"
			"  ESI=%08x EDI=%08x EBP=%08x ESP=%08x\n"
			"  EIP=%08x EFL=%08x "
			"\n"
			,	r->eax, r->ebx, r->ecx, r->edx,
				r->esi, r->edi, r->ebp, r->esp,
				r->eip, r->eflags);
}

void kernel_early(uint32_t magic, uint32_t addr)
{
	cli();
	vga_init();
	tty_init();
	gdt_init();
	idt_init();
	paging_init();
	isrs_init();
	irq_init();
	irq_handler_set(0x81, &regs_dump_handler);
	mm_init();
	pic_init();
	kb_init();
	sti();
	syscall_init();

	fds[1]->write = tty_write;
	nodes = 3;

	if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Magic \'%#x\' invalid!\n", magic);
		panic("Can't get memory map!\n");
	}

	multiboot_info_t *mbi = (multiboot_info_t *) addr;


	if(mbi->flags & MULTIBOOT_INFO_ELF_SHDR) printf("ELF shdr\n");
	if(mbi->flags & MULTIBOOT_INFO_MEM_MAP)
	{
		multiboot_memory_map_t *mmap = mbi->mmap_addr;

		printf("System RAM:\n");

		const char *mmap_type[] = {
			"0", "AVAILABLE\t", "RESERVED\t", "ACPI_RECLAIMABLE", "NVS\t\t\t", "BADRAM\t\t"
		};

		for(uint32_t x = 0; x < mbi->mmap_length / sizeof(multiboot_memory_map_t); x++)
			printf("  %#011x - %#011x %12i bytes  |  %s\n",
				(uint32_t) mmap[x].addr,
				(uint32_t) mmap[x].len + (uint32_t) mmap[x].addr - 1,
				(uint32_t) mmap[x].len,
				mmap_type[(uint32_t) mmap[x].type]);
	}

	printf("Founded PCI devices:\n");
	pci_init();
}


/*
struct dirent *tar_readdir(fs_node_t *node, uint32_t index)
{
	unsigned int i = 0, off = 0;
	while(1)
	{
		tar_header_t *a = (tar_header_t *)(_binary_initrd_tar_start + off);
		if(!a->filename[0]) break;

		unsigned int size = getsize(a->size);
		if(i == index)
		{
			struct dirent b, *de = &b; //TODO just for testing


			strcpy(de->name, a->filename);

			return a;
		}
		off += ((size / 512) + 1) * 512;
		if (size % 512) off += 512;
		i++;
	}
	return NULL;
}*/

void initrd_init()
{
//	tar_list_all(_binary_initrd_tar_start);

//	char *buf;
//	tar_file_read(_binary_initrd_tar_start, "./initrd/bin/test", buf, 10);
//	printf("%s\n", buf);
}

extern uint32_t *kernel_end;



uint64_t rdtsc(void) {
	uint64_t tick;
	__asm__ volatile("rdtsc":"=A"(tick));
    return tick;
}

#include <syscall.h>

void kernel_main()
{
//	printf("%x%x\n", (uint32_t)(rdtsc() >> 32), (uint32_t)rdtsc());


//	uint32_t f1 = open("/tty", 0);

//	write(1, "test\n", 5);

/*	write(f1, "test\n", 5);

	char *b = kmalloc(20 * sizeof(char));
	uint32_t f2 = open("initrd/file1", 0);
	read(f2, b, 20);

	write(f1, b, 20);

	printf("\n");*/

//	__asm__("int $0x81");

	printk("Initialized!\n"); //y\\a

/*	tar_list_all(_binary_initrd_tar_start);
	uint32_t fd = open("initrd/flatbinary/test.bin", 0);
    uint8_t *buf = kmalloc(30);
    read(fd, buf, 10);
	for(int x = 0; x < 10; x++) printf("%x ", (uint8_t)buf[x]);
	printf("\n");

	__asm__("call 0xc010c7dd");*/


/*	uint32_t *esp, *ebp;
	__asm__ ("mov %%esp, %0; mov %%ebp, %1" : "=m"(esp), "=m"(ebp));

	printf("esp=%08x\nebp=%08x\n", &esp, &ebp);

	for(uint32_t x=0; x < 25; x++)
	{
		printf("esp[%08x]=%08x%s\n", &esp-x, *(&esp-x), (&esp-x != ebp) ? "" : " <- esp");
	}*/


//	printf("kernel_end = %x\n", &kernel_end);

//	initrd_init();
//	read_rtc();


/*	DIR *d = opendir("/");
	struct dirent *de;
	while((de = readdir(d)) != NULL)
		printf("%s\n", de->name);
	closedir(d);*/

//	tar_list_all(_binary_initrd_tar_start);

/*	while(1)
	{
		for (uint32_t x = 0; x % 32000; x++) __asm__("nop");
		__asm__ ("int $0x81");
	}*/

	//memmove


//	printf("%#x\n", read_eip());

//	__asm__ ("mov $0, %eax; int $70");


	#define sys_write 4

	char *trol = "ahoj\n";

	int32_t ret = 0;
	ret = syscall(sys_write, 2, trol, 5);
	printf("%i\n", ret);

//	extern int shell_cmd_invaders(int, char **);
//	shell_cmd_invaders(0, NULL);
	shell();

	while(1);
}
