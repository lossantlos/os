#include <stdint.h>
#include <stddef.h>

#include <kernel/asm.h>

#include <kernel/tty.h>
#include <stdlib.h>

#include <kernel/gdt.h>
#include <kernel/idt.h>




//void kernel_early()

#include "arch/i386/multiboot.h"

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

//	printf("Magic: %#x (%p)\n", magic, addr);
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


#include <tar.h>
#include <kernel/fs.h>

extern char _binary_initrd_tar_start[];
extern char _binary_initrd_tar_end[];

fs_node_t *initrd_root;
fs_node_t *initrd_dev;
fs_node_t initrd_node[32];
int initrd_nodes = 0;

/*
struct file_operations {
//	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char *, size_t, loff_t *);
	int (*readdir) (struct file *, void *, filldir_t);
//	unsigned int (*poll) (struct file *, struct poll_table_struct *);
//	int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
//	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct inode *, struct file *);
//	int (*flush) (struct file *);
	int (*release) (struct inode *, struct file *);
//	int (*fsync) (struct file *, struct dentry *, int datasync);
//	int (*fasync) (int, struct file *, int);
//	int (*lock) (struct file *, int, struct file_lock *);
//	ssize_t (*readv) (struct file *, const struct iovec *, unsigned long, loff_t *);
//	ssize_t (*writev) (struct file *, const struct iovec *, unsigned long, loff_t *);
};*/

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
}

void initrd_init()
{/*
	unsigned int off = 0;
	while(1)
	{
		tar_header_t *a = (tar_header_t *)(_binary_initrd_tar_start + off);
		if(!a->filename[0]) break;

		unsigned int size = getsize(a->size);
		strcpy(initrd_node[initrd_nodes].name, a->filename);
		initrd_nodes++;
//		printf("%s (%i)\n", a->filename, size);
		off += ((size / 512) + 1) * 512;
		if (size % 512) off += 512;
	}
*/
//	tar_list_all(_binary_initrd_tar_start);

//	char *buf = 0x10000;
//	tar_file_read(_binary_initrd_tar_start, "./initrd/bin/test", buf, 10);
//	printf("%s\n", buf);

	fs_node_t a;
	fs_node_t *root = &a;


	strcpy(root->name, "/");
	initrd_root->mask = 0;
	initrd_root->uid = 0;
	initrd_root->gid = 0;
	initrd_root->flags = FS_DIRECTORY;
	initrd_root->inode = 0;
	initrd_root->lenght = 0;
	initrd_root->read = NULL;
	initrd_root->write = NULL;
	initrd_root->open = NULL;
	initrd_root->close = NULL;
	initrd_root->readdir = tar_readdir;
	initrd_root->finddir = NULL;
	initrd_root->ptr = NULL;

	readdir_fs(initrd_root, 2);
}

//-------------------------------
#include <dirent.h>

DIR *opendir(const char *path)
{}

struct dirent *readdir(DIR *dir)
{}

int closedir(DIR *dir)
{}
//---------------------------------


/*
UDX_PACK(struct udx_registers{
 udx_uint32 edi;
 udx_uint32 esi;
 udx_uint32 ebp;
 udx_uint32 esp;
 udx_uint32 ebx;
 udx_uint32 edx;
 udx_uint32 ecx;
 udx_uint32 eax;
 udx_uint32 error_code;
 udx_uint32 eip;
 udx_uint16 cs;
 udx_uint16 junk1;
 udx_uint32 eflags;
 // usermode only
 udx_uint32 user_esp;
 udx_uint16 user_ss;
 udx_uint16 junk2;
});*/

/*
#include <kernel/regs.h>

typedef struct task_struct
{
	regs_t regs;
	void *stack;
//	pagedir_t *cr3;
	struct process_struct *next;
} task_t;

task_t *t_quee = NULL, *t_current = NULL;

void task_switch(regs_t *r)
{
	printf(".\n");
	//memcpy(&t_current->regs, r, sizeof(regs_t));
	memcpy(t_current->regs, r, sizeof(regs_t));
	if(t_current->next != NULL) t_current = t_current->next;
		else t_current = t_quee;
	//memcpy(r, &t_current->regs, sizeof(regs_t));
	memcpy(r, t_current->regs, sizeof(regs_t));
}


void t1()
{
	while(1)
	{
		pic_wait(20);
		printf("1\n");
		__asm__ ("int $0x81");
	}
}

void t2()
{
	while(1)
	{
		pic_wait(20);
		printf("2\n");
		__asm__ ("int $0x81");
	}
}

void task_new(void (*fce)())
{
	__asm__("sti");
	task_t *task = (task_t *) kmalloc(sizeof(task_t));

	task->stack = kmalloc(15000);

	task->regs.esp = task->stack;
	task->regs.ebp = task->stack;
	task->regs.eip = fce;

	task->next = NULL;

	task_t *last = t_quee;

//	__asm__("jmp %0"::"r"(fce));

	if (!last)
	{
		t_quee = task;
		t_current = t_quee;
		return;
	};
	while(last->next != NULL) last = last->next;
	last->next = task;
}*/

void kernel_main()
{
	printk("Initialized!\n\a");

	initrd_init();
	read_rtc();

	DIR *d = opendir("/");
	struct dirent *de;
	while((de = readdir(d)) != NULL)
		printf("%s\n", de->name);
	closedir(d);

	//memmove

//	irq_handler_set(0x81, task_switch);

//	task_new(t1);
//	task_new(t2);

//	printf("%#x\n", read_eip());

//	__asm__ ("mov $0, %eax; int $70");

/*	char *trol = "ahoj";

	__asm__ volatile("int $70"
	:
	: "a" (1), "b" (trol));*/

//	__asm__ ("int $71");
//	test();
//	__asm__ ("int $0x80");

	shell();

	while(1);
}
