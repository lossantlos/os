ENTRY(entry)
OUTPUT_FORMAT(elf32-i386)

kernel_phys_start = 0x1M;
kernel_virtual_offset = 0xC0000000;

SECTIONS
{
/*---------------------------------------------
| PHYSICAL
+----------------------------------------------*/
    . = kernel_phys_start;

    .text_phys ALIGN(4K) :
    {
        *(.multiboot)
        *entry*.o(.text)
    }

    .data_phys ALIGN(4K) :
    {
        *entry*.o(.rodata)
        *entry*.o(.data)
        *entry*.o(.bss)
        *entry*.o(COMMON)
        *entry*.o(.bootstrap_stack)
    }

/*---------------------------------------------
| VIRTUAL
+----------------------------------------------*/
	. += kernel_virtual_offset;
    kernel_virtual_start = .;

	.text ALIGN(4K) : AT(ADDR(.text) - kernel_virtual_offset)
	{
        *(.higher_half_text)
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - kernel_virtual_offset)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - kernel_virtual_offset)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss) - kernel_virtual_offset)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
    kernel_virtual_end = .;

/*    /DISCARD/ :
    {
        *(*)
    }*/

	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}
