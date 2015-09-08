/**
@file asm.h
@brief Some assembler functions
*/

#ifndef _ASM_H
#define _ASM_H

#include <stdint.h>
#include <stddef.h>

#warning TODO check inb and outb
#warning TODO add rdtsc instruction

/**
@brief Outputs byte value to port
@param port output port
@param val value
*/
inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    //TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant.
    //TODO: Should %1 be %w1?
}

/**
@brief Read byte from port
@param port address
@return Value from port
*/
inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    //TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant.
    //TODO: Should %1 be %w1?
    return ret;
}

/**
@brief Enable interrupts
*/
inline void sti()
{
    __asm__ volatile ( "sti" );
}

/**
@brief Disable interrupts
*/
inline void cli()
{
    __asm__ volatile ( "cli" );
}



#endif
