#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>
#include <stddef.h>

#define sint32_t uint32_t

struct gdt_entry_struct
{
   uint16_t limit_low;
   uint16_t base_low;
   uint8_t  base_middle;
   uint8_t  access;
   uint8_t  granularity;
   uint8_t  base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;


struct gdt_ptr_struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;


extern void gdt_flush(uint32_t);


void init_gdt();
void gdt_set_gate(sint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

#endif
