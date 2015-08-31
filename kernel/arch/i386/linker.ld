ENTRY(entry)
OUTPUT_FORMAT(elf32-i386)

kernel_phys_start = 0x1M;
kernel_virtual_offset = 0xC0000000;
kernel_stack_size = 14336; /*14KiB*/

SECTIONS
{
/*---------------------------------------------
| PHYSICAL
+----------------------------------------------*/
    . = kernel_phys_start;

    .multiboot_header ALIGN(4K) : {
        *(.multiboot_header)
    }

    .text_phys ALIGN(4K) : {
        *(.text_phys)
    }

    .data_phys ALIGN(4K) : {
        *(.data_phys)
    }

/*---------------------------------------------
| VIRTUAL
+----------------------------------------------*/
	. += kernel_virtual_offset;
    kernel_virtual_start = .;

	.text ALIGN(4K) : AT(ADDR(.text) - kernel_virtual_offset) {
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - kernel_virtual_offset) {
		*(.rodata*) /* TODO strange... */
	}

	.data ALIGN(4K) : AT(ADDR(.data) - kernel_virtual_offset) {
		*(.data)
	}

	/* Read-write data (uninitialized) */
	.bss ALIGN(4K) : AT(ADDR(.bss) - kernel_virtual_offset) {
		*(COMMON)
		*(.bss)
	}

    .stack_space ALIGN(4K) : AT(ADDR(.stack_space) - kernel_virtual_offset) {
        FILL(0);
        . += kernel_stack_size;
        stack_addr = .;
        stack_phys_addr = stack_addr - kernel_virtual_offset;
    }
    kernel_end = .;

    /DISCARD/ :
    {
        *(*)
    }
}
