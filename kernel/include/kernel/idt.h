/**
@file idt.h
@brief Handling with interrupt descriptor table
*/

#ifndef _IDT_H

#include <stdint.h>
#include <stddef.h>

/**
@brief Interrupt descriptor table entry
*/
struct idt_entry
{
    uint16_t base_lo;   ///<Offset bits 0..15
    uint16_t sel;       ///<A code segment selector in GDT or LDT
    uint8_t  always0;   ///<This will ALWAYS be set to 0!
    uint8_t  flags;     ///<type and attributes
    uint16_t base_hi;   ///<Offset bits 16..31
} __attribute__((packed));

/**
@brief Interrupt descriptor table pointer
*/
struct idt_ptr
{
    uint16_t limit;     ///<Lenght of the IDT in bytes-1 (minimum value is 100h, a value of 1000h means 200h interrupts)
    uint32_t base;      ///<Adress of IDT begin (INT 0)
} __attribute__((packed));


extern struct idt_entry idt[256];
extern struct idt_ptr idtp;

void idt_init();
void idt_load(const struct idt_ptr* ptr);
void idt_set_gate(uint8_t num, unsigned long base, unsigned short sel, uint8_t flags);

#define _IDT_H
#endif
