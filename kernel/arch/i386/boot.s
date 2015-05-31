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

# The kernel entry point
.section .text
.global _start
.type _start, @function
_start:
	# Setting up a stack, setting the esp register to point to the top of reserved place for stack
	movl $stack_top, %esp

	call kernel_early

	call kernel_main

	cli
.Lhang:
	hlt
	jmp .Lhang

.size _start, . - _start
