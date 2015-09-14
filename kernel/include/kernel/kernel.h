
#ifndef _KERNEL_H
#define _KERNEL_H

#include <kernel/asm.h>

#define panic(msg) {regs_dump(); cli(); printf("KERNEL PANIC!!! %s:%i: %s\n", __FILE__, __LINE__, msg); for(;;);};

#endif
