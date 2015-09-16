
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <stdint.h>

uint32_t __attribute__((always_inline)) _syscall0(uint32_t n)
{
	uint32_t ret;
	__asm__ volatile("int $0x80"
					: "=a" (ret)
					: "0" (n)
					: "cc", "edi", "esi", "memory");
	return ret;
}

uint32_t __attribute__((always_inline)) _syscall1(uint32_t n, uint32_t p1)
{
	uint32_t ret;
	__asm__ volatile("int $0x80"
					: "=a" (ret)
					: "0" (n), "b" (p1)
					: "cc", "memory");
	return ret;
}

uint32_t __attribute__((always_inline)) _syscall2(uint32_t n, uint32_t p1, uint32_t p2)
{
	uint32_t ret;
	__asm__ volatile("int $0x80"
					: "=a" (ret)
					: "0" (n), "b" (p1), "c" (p2)
					: "cc", "memory");
	return ret;
}

uint32_t __attribute__((always_inline)) _syscall3(uint32_t n, uint32_t p1, uint32_t p2, uint32_t p3)
{
	uint32_t ret;
	__asm__ volatile("int $0x80"
					: "=a" (ret)
					: "0" (n), "b" (p1), "c" (p2), "d" (p3)
					: "cc", "memory");
	return ret;
}

#warning TODO check (esi and edi registers)

uint32_t __attribute__((always_inline)) _syscall4(uint32_t n, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4)
{
	uint32_t ret;
	__asm__ volatile("mov %4, %%esi; "
                     "int $0x80; "
					: "=a" (ret)
					: "0" (n), "b" (p1), "c" (p2), "d" (p3), "S" (p4)
                    : "cc", "memory");
					//: "cc", "esi", "memory");
	return ret;
}

uint32_t __attribute__((always_inline)) _syscall5(uint32_t n, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5)
{
	uint32_t ret;
	__asm__ volatile("int $0x80"
					: "=a" (ret)
					: "0" (n), "b" (p1), "c" (p2), "d" (p3), "S" (p4), "D" (p5)
					: "cc", "memory");
					//: "cc", "edi", "esi", "memory");
	return ret;
}

#define MKFN(fn,...) MKFN_N(fn,##__VA_ARGS__,9,8,7,6,5,4,3,2,1,0)(__VA_ARGS__)
#define MKFN_N(fn,NR,n0,n1,n2,n3,n4,n5,n6,n7,n8,n,...) fn##n
#define syscall(...) MKFN(_syscall,##__VA_ARGS__)


#endif
