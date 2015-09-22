# Declare constants used for creating a multiboot header
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO | 0x20  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard
.section .multiboot_header, "a", @progbits
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


.section .text_phys
.global entry
.type entry, @function

# The kernel entry point
entry:
    # Setting up a stack
	movl $stack_phys_addr, %esp
    movl $stack_phys_addr, %ebp


    # Arguments for kernel early
    pushl %ebx # Pointer to multiboot header
	pushl %eax # Magic

    # Map 0-896MB to 3072-3968
    call paging_prepare
    call enable_paging

    # Switch stack to Higher Half - TODO variable offset
    movl %esp, %eax
	add $kernel_virtual_offset, %eax
    mov %eax, %esp
    movl %ebp, %eax
	add $kernel_virtual_offset, %eax
    mov %eax, %ebp

    # Jump to Higher Half
    jmp higher_half
hang0:
    hlt
    jmp hang0

enable_paging:
    movl $pd, %eax
    movl %eax, %cr3
    movl %cr4, %eax
    orl $0x00000010, %eax
    movl %eax, %cr4
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0
    ret

.section .text
higher_half:
    call kernel_early
    call kernel_main
    cli
hang1:
    hlt
    jmp hang1
