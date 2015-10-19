OUTPUT_FORMAT(binary)
ENTRY(main)

/*STARTUP(start.o)*/

SECTIONS
{
    . = 0;

    .text ALIGN(4):
    {
        start.o(.text)
        *(.text)
    }

    .data ALIGN(4):
    {
        *(.data)
    }

    .rodata ALIGN(4):
    {
        *(.rodata*)
    }
}
