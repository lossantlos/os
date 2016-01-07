# multibooot1
# Declare constants used for creating a multiboot header
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
#.set FLAGS,    ALIGN | MEMINFO | 0x20  # this is the Multiboot 'flag' field
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard
.section .multiboot_header, "a", @progbits
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
