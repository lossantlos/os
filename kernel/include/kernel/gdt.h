/**
@file gdt.h
@brief Handling with global descriptor table
*/

#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>
#include <stddef.h>


/*
#define SEG_DATA_RD        0x00 ///< Read-Only
#define SEG_DATA_RDA       0x01 ///< Read-Only, accessed
#define SEG_DATA_RDWR      0x02 ///< Read/Write
#define SEG_DATA_RDWRA     0x03 ///< Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 ///< Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 ///< Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 ///< Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 ///< Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 ///< Execute-Only
#define SEG_CODE_EXA       0x09 ///< Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A ///< Execute/Read
#define SEG_CODE_EXRDA     0x0B ///< Execute/Read, accessed
#define SEG_CODE_EXC       0x0C ///< Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D ///< Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E ///< Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F ///< Execute/Read, conforming, accessed

*/


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
    uint16_t limit; ///<Lenght of the GDT in bytes-1
    uint32_t base;  ///<Adress of GDT begin
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;


extern void gdt_flush(uint32_t);


void init_gdt();
void gdt_set_gate(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

#endif
