
#ifndef _TASK_H
#define _TASK_H


typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} regs_t;

typedef struct {
    regs_t regs;
} task_t;

#endif
