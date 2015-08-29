
.text
.global loadPageDirectory
.global paging_enable

loadPageDirectory:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

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

#paging_enable:
#	push %ebp
#	mov %esp, %ebp
#	mov %cr0, %eax
#	or $0x80000000, %eax
#	mov %eax, %cr0
#	mov %ebp, %esp
#	pop %ebp
#	ret


.global read_eip
read_eip:
	pop %eax
	jmp %eax
