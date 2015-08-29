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
; Service Routines (ISRs) & IRQs

extern irq_handler

; This is our common ISR and IRQ stub. It saves the processor state, sets up
; for kernel mode segments, calls the C-level handler, and finally restores
; the stack frame.
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

%macro irq 1
global irq%1
irq%1:
    cli
    push byte 0    ; Note that these don't push an error code on the stack:
                   ; We need to push a dummy error code
    push 32+%1
    jmp irq_common_stub
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
irq 16
irq 17
irq 18
irq 19
irq 20
irq 21
irq 22
irq 23
irq 24
irq 25
irq 26
irq 27
irq 28
irq 29
irq 30
irq 31
irq 32
irq 33
irq 34
irq 35
irq 36
irq 37
irq 38
irq 39
irq 40
irq 41
irq 42
irq 43
irq 44
irq 45
irq 46
irq 47
irq 48
irq 49
irq 50
irq 51
irq 52
irq 53
irq 54
irq 55
irq 56
irq 57
irq 58
irq 59
irq 60
irq 61
irq 62
irq 63
irq 64
irq 65
irq 66
irq 67
irq 68
irq 69
irq 70
irq 71
irq 72
irq 73
irq 74
irq 75
irq 76
irq 77
irq 78
irq 79
irq 80
irq 81
irq 82
irq 83
irq 84
irq 85
irq 86
irq 87
irq 88
irq 89
irq 90
irq 91
irq 92
irq 93
irq 94
irq 95
irq 96
irq 97
irq 98
irq 99
irq 100
irq 101
irq 102
irq 103
irq 104
irq 105
irq 106
irq 107
irq 108
irq 109
irq 110
irq 111
irq 112
irq 113
irq 114
irq 115
irq 116
irq 117
irq 118
irq 119
irq 120
irq 121
irq 122
irq 123
irq 124
irq 125
irq 126
irq 127
irq 128
irq 129
irq 130
irq 131
irq 132
irq 133
irq 134
irq 135
irq 136
irq 137
irq 138
irq 139
irq 140
irq 141
irq 142
irq 143
irq 144
irq 145
irq 146
irq 147
irq 148
irq 149
irq 150
irq 151
irq 152
irq 153
irq 154
irq 155
irq 156
irq 157
irq 158
irq 159
irq 160
irq 161
irq 162
irq 163
irq 164
irq 165
irq 166
irq 167
irq 168
irq 169
irq 170
irq 171
irq 172
irq 173
irq 174
irq 175
irq 176
irq 177
irq 178
irq 179
irq 180
irq 181
irq 182
irq 183
irq 184
irq 185
irq 186
irq 187
irq 188
irq 189
irq 190
irq 191
irq 192
irq 193
irq 194
irq 195
irq 196
irq 197
irq 198
irq 199
irq 200
irq 201
irq 202
irq 203
irq 204
irq 205
irq 206
irq 207
irq 208
irq 209
irq 210
irq 211
irq 212
irq 213
irq 214
irq 215
irq 216
irq 217
irq 218
irq 219
irq 220
irq 221
irq 222
irq 223
irq 224
