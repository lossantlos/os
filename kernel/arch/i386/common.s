
.text

#---------------------------

.global gdt_flush
gdt_flush:
   mov 4(%esp), %eax # -4 i think #Get the pointer to the GDT, passed as a parameter.
   lgdt (%eax)        #Load the new GDT pointer

   mov $0x10, %ax    #0x10 is the offset in the GDT to our data segment
   mov %ax, %ds      #Load all data segment selectors
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   mov %ax, %ss
   ljmp $0x08, $flush   #0x08 is the offset to our code segment: Far jump!
flush:
   ret


#--------------------------

.global loadPageDirectory
loadPageDirectory:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

.global paging_enable
paging_enable:
	 movl $page_directory, %eax
	 movl %eax, %cr3
	 movl %cr4, %eax
	 orl $0x00000010, %eax
	 movl %eax, %cr4
	 movl %cr0, %eax
	 orl $0x80000000, %eax
	 movl %eax, %cr0
	 ret

.global read_eip
read_eip:
	pop %eax
	jmp %eax


#------------------------- IRQ/ISR ------------------------------------

.extern irq_common_stub
.extern isr_common_stub

.altmacro

.macro isr_noerr num
.global isr\num
isr\num:
#    cli
	pushl 0
	pushl $ \num
    jmp isr_common_stub
.endm

.macro isr_err num
.global isr\num
.warning "isr-\num"
isr\num:
#    cli
	pushl $ \num
    jmp isr_common_stub
.endm

.macro isr_noerr_interval start, end
isr_noerr %start
.if \end-\start
isr_noerr_interval (\start+1),\end
.endif
.endm


#Interrupt requests (hardware interrupts)
isr_noerr  0				#Divide By Zero Exception
isr_noerr  1				#Debug Exception
isr_noerr  2				#Non Maskable Interrupt Exception
isr_noerr  3				#Int 3 Exception
isr_noerr  4				#INTO Exception
isr_noerr  5				#Out of Bounds Exception
isr_noerr  6				#Invalid Opcode Exception
isr_noerr  7				#Coprocessor Not Available Exception
isr_err    8				#Double Fault Exception
isr_noerr  9				#Coprocessor Segment Overrun Exception
isr_err   10				#Bad TSS Exception
isr_err   11				#Segment Not Present Exception
isr_err   12				#Stack Fault Exception
isr_err   13				#General Protection Fault Exception
isr_err   14				#Page Fault Exception
isr_noerr 15				#Reserved Exception
isr_noerr 16				#Floating Point Exception
isr_noerr 17				#Alignment Check Exception
isr_noerr 18				#Machine Check Exception
isr_noerr_interval 19, 31	#Reserved

#Software Interrupts
isr_noerr_interval 32, 131
isr_noerr_interval 132, 231
isr_noerr_interval 232, 255
