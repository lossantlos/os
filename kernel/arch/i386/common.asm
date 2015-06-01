global gdt_flush
gdt_flush:
   mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer

   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
   ret

;------------------------------------------------------------------------------
; Service Routines (ISRs)

%macro isr_noerr 1
global isr%1
isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro isr_err 1
global isr%1
isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro


isr_noerr  0 ; Divide By Zero Exception
isr_noerr  1 ; Debug Exception
isr_noerr  2 ; Non Maskable Interrupt Exception
isr_noerr  3 ; Int 3 Exception
isr_noerr  4 ; INTO Exception
isr_noerr  5 ; Out of Bounds Exception
isr_noerr  6 ; Invalid Opcode Exception
isr_noerr  7 ; Coprocessor Not Available Exception
isr_err    8 ; Double Fault Exception
isr_noerr  9 ; Coprocessor Segment Overrun Exception
isr_err   10 ; Bad TSS Exception
isr_err   11 ; Segment Not Present Exception
isr_err   12 ; Stack Fault Exception
isr_err   13 ; General Protection Fault Exception
isr_err   14 ; Page Fault Exception
isr_noerr 15 ; Reserved Exception
isr_noerr 16 ; Floating Point Exception
isr_noerr 17 ; Alignment Check Exception
isr_noerr 18 ; Machine Check Exception
isr_noerr 19 ; Reserved
isr_noerr 20 ; Reserved
isr_noerr 21 ; Reserved
isr_noerr 22 ; Reserved
isr_noerr 23 ; Reserved
isr_noerr 24 ; Reserved
isr_noerr 25 ; Reserved
isr_noerr 26 ; Reserved
isr_noerr 27 ; Reserved
isr_noerr 28 ; Reserved
isr_noerr 29 ; Reserved
isr_noerr 30 ; Reserved
isr_noerr 31 ; Reserved



extern fault_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

;------------------------------------------------------------------------------
; IRQs


%macro irq 1
global irq%1
irq%1:
    cli
    push byte 0    ; Note that these don't push an error code on the stack:
                   ; We need to push a dummy error code
    push byte 32+%1
    jmp irq_common_stub
%endmacro

irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15


extern irq_handler

; This is a stub that we have created for IRQ based ISRs. This calls
; '_irq_handler' in our C code. We need to create this in an 'irq.c'
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
