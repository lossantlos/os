//
//  elf.h
//  elf
//
//  Created by Jakub Kyzek on 21.08.15.
//  Copyright (c) 2015 kyzek. All rights reserved.
//

#ifndef elf_elf_h
#define elf_elf_h

#include <stdint.h>

#define EI_NIDENT 16

typedef struct {
	uint8_t		e_ident[EI_NIDENT];
#define EI_MAG0			0	//0x7f
#define	EI_MAG1			1	//'E'
#define	EI_MAG2			2   //'L'
#define	EI_MAG3			3	//'F'
#define	EI_CLASS		4	// 1 = 32bit, 2 = 64bit
	#define ELFCLASSNONE	0 //invalid class
	#define ELFCLASS32		1 //32-bit objects
	#define ELFCLASS64		2 //64-bit objects
#define	EI_DATA			5	//endianness
	#define ELFDATANONE	0	//invalid data encoding
	#define ELFDATA2LSB	1									//this
	#define ELFDATA2MSB	2
#define	EI_VERSION		6	// 1 = original elf
	#define EV_NONE			0x00 //none version
	#define EV_CURRENT		0x01 //current version
#define	EI_OSABI		7
		/*
		 0x00	System V
		 0x01	HP-UX
		 0x02	NetBSD
		 0x03	Linux
		 0x06	Solaris
		 0x07	AIX
		 0x08	IRIX
		 0x09	FreeBSD
		 0x0C	OpenBSD
		 0x0D	OpenVMS
		*/
#define	EI_ABIVERSION	8	//???
#define EI_PAD			9	//unused
	
	uint8_t		e_type;
#define ET_NONE			0	//no file type
#define ET_REL			1	//relocatable file
#define ET_EXEC			2	//executable file
#define ET_DYN			3	//shared object file
#define ET_CORE			4	//corefile
#define ET_LOPROC		0xff00 //processor-specific
#define ET_HIPROC		0xffff	//processor-specific
	
	uint16_t	e_machine;
#define EM_NONE			0x00 //No machine
#define EM_M32			0x01 //AT&T WE 32100
#define EM_SPARC		0x02 //SPARC
#define EM_386			0x03 //Intel 80386
#define EM_68K			0x04 //Motorola 68000
#define EM_88K			0x05 //Motorola 88000
#define EM_860			0x07 //Intel 80860
#define EM_MIPS			0x08 //MIPS RS3000
//#define EM_			0x14 //PowerPC
//#define EM_			0x28 //ARM
//#define EM_			0x2a //SuperH
//#define EM_			0x32 //IA-64
//#define EM_			0x3e //x86-64
//#define EM_			0xb7 //AArch64

	uint32_t	e_version; //declared above

	//uint64_t for x86_64
	uint32_t	e_entry;	//memory address of the entry point
	uint32_t	e_phoff;	//points to the start of the program header table. It usually follows the file header immediately making the offset 0x40 for 64-bit ELF executables
	uint32_t	e_shoff;	//points to the start of the section header table
	//--------------------
	uint32_t	e_flags;	//depend on architecture
	uint16_t	e_ehsize;	//size of this header, normally 64 bytes for 64-bit and 52 for 32-bit format
	uint16_t	e_phentsize;//size of a program header table entry
	uint16_t	e_phnum;	//number of entries in the program header table
	uint16_t	e_shentsize;//
	uint16_t	e_shnum;	//number of sections
	uint16_t	e_shstrndx;	//section header string index
} elf32_ehdr_t;


typedef struct {
	uint32_t p_type;
#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff
	
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
#define PF_R		(1 << 0) //read
#define PF_W		(1 << 1) //write
#define PF_X		(1 << 2) //execute
	
	uint32_t p_align;
} elf32_phdr_t; //program header


typedef struct {
	uint32_t sh_name;
	uint32_t sh_type;
#define SHT_NULL		0
#define SHT_PROGBITS	1
#define SHT_SYMTAB		2
#define SHT_STRTAB		3
#define SHT_RELA		4
#define SHT_HASH		5
#define SHT_DYNAMIC		6
#define SHT_NOTE		7
#define	SHT_NOBITS		8
#define SHT_REL			9
#define SHT_SHLIB		10
#define SHT_DYNSYM		11
#define SHT_LOPROC		0x70000000
#define SHT_HIPROC		0x7fffffff
#define SHT_LOUSER		0x80000000
#define SHT_HIUSER		0xffffffff
	
	uint32_t sh_flags;
#define SHT_WRITE		(1 << 0)
#define SHT_ALLOC		(1 << 1)
#define SHT_EXECINSTR	(1 << 2)
#define SHT_MERGE		(1 << 4)
#define SHT_STRINGS		(1 << 5)
#define SHT_INFO_LINK	(1 << 6)
#define SHT_LINK_ORDER	(1 << 7)
#define SHT_OS_NONCONFORMING (1 << 8)
#define SHT_GROUP		(1 << 9)
#define SHT_TLS			(1 << 10)
#define SHT_MASKPROC	0xf0000000
	
	
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	
	uint32_t sh_link;
	uint32_t sh_info;
	
	uint32_t sh_addralign;
	uint32_t sh_entsize;
} elf32_shdr_t; //section header

typedef struct {
	uint32_t st_name;
	uint32_t st_value;
	uint32_t st_size;
	uint8_t st_info;
//bind = (st_info >> 4) & 0xf
#define STB_LOCAL	0
#define STB_GLOBAL	1
#define STB_WEAK	2
#define STB_LOPROC	13
#define STB_HIPROC	15
//type = st_info & 0xf
#define STT_NOTYPE	0
#define STT_OBJECT	1
#define STT_FUNCT	2
#define STT_SECTION	3
#define STT_FILE	4
#define STT_LOPROC	13
#define STT_HIPROC	15
	
	uint8_t st_other;
	uint16_t st_shndx;
} elf32_sym_t; //symbol

#endif
