ARCH=i386
TARGET=${ARCH}-elf
QEMU= qemu-system-${ARCH}

P_ROOT=$(PWD)

CC=${TARGET}-gcc
LD=${TARGET}-ld
AR=${TARGET}-ar
CFLAGS= -std=c11 -ffreestanding -O2 -Wall -Wextra -I${P_ROOT}/kernel/include/ -I${P_ROOT}/libc/include/
#CFLAGS := ${CFLAGS} -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow #64bit
LDFLAGS= -ffreestanding -O2 -nostdlib

export

.PHONY: run all clean help

all: kernel.bin

libc/libc.a: $(shell find libc -name *.c)
	make -C libc/ build

kernel.bin: libc/libc.a $(shell find kernel -name *.c -o -name *.s)
	make -C kernel/ build

run: kernel.bin
	${QEMU} -kernel $^ -m 1024 -monitor stdio

clean:
	make -C libc/ clean
	make -C kernel/ clean

help:
	@echo "make [option]\n\
options:\n\
\trun\tbuild kernel and run in qemu\n\
\tbuild\tcompile and link kernel\n\
\tclean\tclean executable and object files"
