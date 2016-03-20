/**
@file main.c
*/

/*----config-------*/

#define CONFIG_MULTIBOOT2

/*---------------*/



#include <stdint.h>
#include <stddef.h>

#include <kernel/asm.h>
#include <kernel/kernel.h>
#include <kernel/regs.h>

#include <kernel/tty.h>
#include <stdlib.h>

#include <kernel/gdt.h>
#include <kernel/idt.h>



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

//CONFIG_MULTIBOOT1, CONFIG_MULTIBOOT2

#ifdef CONFIG_MULTIBOOT1

#include "arch/i386/multiboot1.h"

void multiboot1_parse(uint32_t magic, uint32_t addr)
{
	if(magic != 0x2BADB002)
		panic("multiboot1: Invalid magic number\n");

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
		{
			printf("  %#011x - %#011x %12i bytes  |  %s\n",
				(uint32_t) mmap[x].addr,
				(uint32_t) mmap[x].len + (uint32_t) mmap[x].addr - 1,
				(uint32_t) mmap[x].len,
				mmap_type[(uint32_t) mmap[x].type]);
			mm_phys_add((void *) mmap[x].addr, mmap[x].len, (mmap[x].len == 1) ? 0 : 1); //0 available  - 1 full
		}
	}
}

#endif

#ifdef CONFIG_MULTIBOOT2

#include "arch/i386/multiboot2.h"
#include <elf.h>


char *name(char *ptr, uint32_t index)
{
	uint32_t i = 0;

	for(uint32_t x = 0; ; x++)
	{
		if(i == index) return ptr + x;
		if(ptr[x] == '\0') i++;
	}
}

void multiboot2_parse(uint32_t magic, uint32_t addr)
{
	if(magic != 0x36d76289)
		panic("multiboot2: Invalid magic number\n");

	if(addr & 7)
		panic("unaligned mbi");

	return; /////////////////////////////////////////////////////////////////////////////////////////// delete this return for parsing mb2header

	struct multiboot_tag *tag = addr;
	uint32_t size = tag->type;

	for(tag = (struct multiboot_tag *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
			tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7)))
		switch(tag->type)
		{
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				printf("cmd line: \"%s\"\n", ((struct multiboot_tag_string *) tag)->string);
				break;
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				printf("boot loader name: \"%s\"\n", ((struct multiboot_tag_string *) tag)->string);
				break;

			case MULTIBOOT_TAG_TYPE_MODULE:
				printf ("Module at 0x%x-0x%x. Command line %s\n",
					((struct multiboot_tag_module *) tag)->mod_start,
					((struct multiboot_tag_module *) tag)->mod_end,
					((struct multiboot_tag_module *) tag)->cmdline);
				break;

			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				printf ("meminfo basic\n\tlower = %iKB\n\tupper = %iKB\n",
					((struct multiboot_tag_basic_meminfo *) tag)->mem_lower,
					((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
				break;
			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				printf ("Boot device 0x%x,%i,%i\n",
					((struct multiboot_tag_bootdev *) tag)->biosdev,
					((struct multiboot_tag_bootdev *) tag)->slice,
					((struct multiboot_tag_bootdev *) tag)->part);
			break;

			case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
				{
					struct multiboot_tag_elf_sections *a = (struct multiboot_tag_elf_sections *) tag;
					printf("elf:\n\ttype %i\n\tsize %i\n\tnum %i\n\tshndx %i\n\tsection headers: %#010x\n",
						(uint32_t) a->type, (uint32_t) a->size, (uint16_t) a->num, (uint16_t) a->entsize,
						(uint16_t) a->shndx, &a->sections);
					elf32_shdr_t *h = &a->sections;
					for(uint32_t k = 0; k < 10; k++)
					{
						printf("\t\t%i %i\t%i %#010x\n", k, h[k].sh_name, h[k].sh_type, h[k].sh_addr);

						if(h[k].sh_type == SHT_STRTAB)
						{
							printf("aaaaaaaaaaaa %s\n",  name(h[k].sh_addr, 12));

						}
					}

					return;


						uint8_t *d = &a->sections;
						#define snprintf(X, Y, ...)

						const char *symbol_type[] = {
							"NOTYPE", "OBJECT", "FUNC", "SECTION", "FILE"
						};

						const char *symbol_bind[] = {
							"LOCAL", "GLOBAL", "WEAK"
						};





						elf32_shdr_t *section = &a->sections;



						for(uint32_t x = 0; x < a->num; x++)
						{
							if(section[x].sh_type == 0)
							{
								printf("%s\n",  &section[x].sh_addr);
								printf("%x\n",  section[x].sh_addr);
								return;


								printf("\nSymbol table '%s' contains %lu entries:\n   Num:    Valu"
									   "e  Size Type    Bind   Vis      Ndx Name\n",
									   &d[section[a->shndx].sh_offset+section[x].sh_name],
									   section[x].sh_size / sizeof(elf32_sym_t));

								elf32_sym_t *symbol = &d[section[x].sh_offset];

								//for(uint32_t y = 0; y < section[x].sh_size / sizeof(elf32_sym_t); y++)
								for(uint32_t y = 0; y < 10; y++)
								{
									char buf[10];
									/*if(symbol[y].st_shndx == 0) snprintf(buf, sizeof(buf), "UND");
									else if(symbol[y].st_shndx == 65521) snprintf(buf, sizeof(buf), "ABS");
									else snprintf(buf, sizeof(buf), "%x", symbol[y].st_shndx);*/

									elf32_sym_t *st = &symbol[y];

									printf(" %5i: %08x  %4i %-7s %-6s %-7s  %-4s %s\n",
										   y, st->st_value, st->st_size,
										   ((st->st_info & 0xf) < 5) ? symbol_type[st->st_info & 0xf] :
										   ((st->st_info & 0xf) == STT_LOPROC) ? "LOPROC" :
										   ((st->st_info & 0xf) == STT_HIPROC) ? "HIPROC" : "UNKNOWN",
										   ((st->st_info >> 4 & 0xf) < 3) ? symbol_bind[st->st_info >> 4 & 0xf] :
										   ((st->st_info >> 4 & 0xf) == STB_LOPROC) ? "LOPROC" :
										   ((st->st_info >> 4 & 0xf) == STB_HIPROC) ? "HIPROC" : "UNKNOWN",
					#warning TODO fix
										   "?", //TODO fix allways default in readelf WTF?
										   buf,
										   &d[section[section[x].sh_link].sh_offset+st->st_name]);

								}
							}
						}







					printf("\n");
				}
				break;

			case MULTIBOOT_TAG_TYPE_MMAP:
				{
					multiboot_memory_map_t *mmap;
					printf ("mmap\n");
					for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
						(multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
						mmap = (multiboot_memory_map_t *)
					((unsigned long) mmap
					+ ((struct multiboot_tag_mmap *) tag)->entry_size))
					{
						uint64_t ptr_start = mmap->addr;
						uint64_t ptr_end = ptr_start + mmap->len - 1;
						char *type_str[] = {"0", "AVAILABLE", "RESERVED", "ACPI_RECLAIMABLE", "NVS", "BADRAM"};
						printf ("\t%#010x%08x - %#010x%08x (%s)\n",
							(uint32_t) (ptr_start >> 32),
							(uint32_t) (ptr_start & 0xffffffff),
							(uint32_t) (ptr_end >> 32),
							(uint32_t) (ptr_end & 0xffffffff),
							type_str[(uint32_t) mmap->type]);
					}
				}
				break;


			default:
				printf("Unhandled mbi tag 0x%x (size: 0x%x)\n", tag->type, tag->size);
		}



	/*

	int x = 0;
	//while(x < m->size)
		//printf("0x%x\t0x%x\n", m[x].type, m[x].size);

	uint32_t *b = m;

	//for(int a = 0; a * sizeof(uint32_t) < m->type; a++) //m->type = size actualy
	for(int a = 0; a < 60; a++) //m->type = size actualy
		printf("%08x ", b[a]);
*/
}

#endif

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
	pit_init();
	kb_init();
	sti();
	syscall_init();

	fds[1]->write = tty_write;
	nodes = 3;

	#if defined(CONFIG_MULTIBOOT1)
	multiboot1_parse(magic, addr);
	#elif defined(CONFIG_MULTIBOOT2)
	multiboot2_parse(magic, addr);
	#else
	#error Choose between CONFIG_MULTIBOOT1 or CONFIG_MULTIBOOT1
	#endif

	//printf("Founded PCI devices:\n");
	//pci_init();
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

/*
	uint32_t f1 = open("/tty", 0);

//	write(1, "test\n", 5);

	write(f1, "test\n", 5);

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
	//ret = syscall(sys_write, 2, trol, 5);
	//printf("%i\n", ret);

//	extern int shell_cmd_invaders(int, char **);
//	shell_cmd_invaders(0, NULL);
	shell();

	while(1);
}
