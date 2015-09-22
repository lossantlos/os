;------------------------------------------------------------------------------
; Service Routines (ISRs) & IRQs

extern irq_handler

; This is our common ISR and IRQ stub. It saves the processor state, sets up
; for kernel mode segments, calls the C-level handler, and finally restores
; the stack frame.

global irq_common_stub
global isr_common_stub

irq_common_stub:
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
    mov eax, irq_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
