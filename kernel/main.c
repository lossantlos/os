/**
@file main.c
*/

#include <stdint.h>
#include <stddef.h>

#include <kernel/asm.h>
#include <kernel/kernel.h>

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
	pic_init();
	kb_init();
	sti();
	syscall_init();

	fds[1]->write = tty_write;
	nodes = 3;

	if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Magic \'%#x\' invalid!\n", magic);
		panic("Can't get memory map!\n");
	} else {
		multiboot_info_t *mbi = (multiboot_info_t *) addr;

		const char *mmap_type[] = {
			"0", "AVAILABLE\t", "RESERVED\t", "ACPI_RECLAIMABLE", "NVS\t\t\t", "BADRAM\t\t"
		};

		multiboot_memory_map_t *mmap = mbi->mmap_addr;
		printf("System RAM:\n");
		for(uint32_t x = 0; x < mbi->mmap_length / sizeof(multiboot_memory_map_t); x++)
			printf("  %#011x - %#011x %12i bytes  |  %s\n",
			(uint32_t) mmap[x].addr,
			(uint32_t) mmap[x].len + (uint32_t) mmap[x].addr, (uint32_t) mmap[x].len,
			mmap_type[(uint32_t) mmap[x].type]);
	}
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


uint32_t syscall_write(uint32_t fd, const char *data, uint32_t len)
{
	uint32_t ret = 0;
	__asm__ volatile("int $0x80"
					: "=a" (ret)
					: "0" ((uint32_t)4), "b" (fd), "c" (data), "d" (len)
					: "cc", "edi", "esi", "memory");
	return ret;
}

void kernel_main()
{
//	uint32_t f1 = open("/tty", 0);

//	write(1, "test\n", 5);

/*	write(f1, "test\n", 5);

	char *b = kmalloc(20 * sizeof(char));
	uint32_t f2 = open("initrd/file1", 0);
	read(f2, b, 20);

	write(f1, b, 20);

	printf("\n");*/

	printk("Initialized!\\a\n"); //y

	//printf("kernel_end = %x\n", &kernel_end);

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

	char *trol = "ahoj\n";

	int32_t ret = 0;
	ret = syscall_write(2, trol, 5);
	printf("%i\n", ret);


//	__asm__ ("int $0x80");
//	extern int shell_cmd_invaders(int, char **);
//	shell_cmd_invaders(0, NULL);
	shell();

	while(1);
}
