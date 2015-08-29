# Declare constants used for creating a multiboot header
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread [stack pointer register (esp)]
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:


.section .text
.global entry
.type entry, @function

# The kernel entry point
entry:
    # Setting up a stack
	movl $stack_top, %esp
    movl $stack_top, %ebp

    # Push pointer to multiboot and magic
    pushl %ebx
	pushl %eax

    # Map 0-896MB to 3072-3968
    call paging_prepare
    call enable_paging

    # Switch stack to Higher Half - TODO variable offset
    movl %esp, %eax
    add $0xC0000000, %eax
    mov %eax, %esp
    movl %ebp, %eax
    add $0xC0000000, %eax
    mov %eax, %ebp

    # Jump to Higher Half
    jmp higher_half
hang:
    hlt
    jmp hang

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

.section .higher_half_text
higher_half:
    call kernel_early
    call kernel_main
    cli
hangA:
    hlt
    jmp hangA
