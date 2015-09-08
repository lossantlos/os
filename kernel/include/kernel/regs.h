#ifndef _REGS_H
#define _REGS_H

#include <stdint.h>

/*
UDX_PACK(struct udx_registers{
 udx_uint32 edi;
 udx_uint32 esi;
 udx_uint32 ebp;
 udx_uint32 esp;
 udx_uint32 ebx;
 udx_uint32 edx;
 udx_uint32 ecx;
 udx_uint32 eax;
 udx_uint32 error_code;
 udx_uint32 eip;
 udx_uint16 cs;
 udx_uint16 junk1;
 udx_uint32 eflags;
 // usermode only
 udx_uint32 user_esp;
 udx_uint16 user_ss;
 udx_uint16 junk2;
});*/

struct regs
{
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
} __attribute__((packed));

typedef struct regs regs_t;

#endif
