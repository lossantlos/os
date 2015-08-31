#PATH := /Users/kuba/Desktop/x86_64-elf/bin/:$PATH

ARCH=i386
TARGET=${ARCH}-elf
QEMU= qemu-system-x86_64
QEMU_FLAGS = -m 1024 -monitor stdio -soundhw pcspk

P_ROOT=$(PWD)

CC=${TARGET}-gcc
LD=${TARGET}-ld
AR=${TARGET}-ar
CFLAGS= -std=c11 -nostartfiles -nodefaultlibs -ffreestanding -O2 -Wall -Wextra\
		-I${P_ROOT}/kernel/include/ -I${P_ROOT}/libc/include/
LDFLAGS= -ffreestanding -O2 -nostdlib

ifeq (${DEBUG},1)
	CFLAGS += -g -D DEBUG
	LDFLAGS += -g
	QEMU_FLAGS += -s -S
endif

include kernel/arch/${ARCH}/make.config

export

.PHONY: run all clean help debug debug_run

all: kernel.bin

libc/libc.a: $(shell find libc -name *.c)
	make -C libc/ build

packages/*/*.o: packages/*/*.c
	make -C packages/

kernel.bin: libc/libc.a $(shell find kernel -name *.c -o -name *.s) initrd.o packages/*/*.o
	make -C kernel/ build

run: kernel.bin
	${QEMU} ${QEMU_FLAGS} -kernel $^

clean:
	rm -rf *.o initrd.tar binary
	make -C libc/ clean
	make -C kernel/ clean



initrd.o: initrd.tar
	${TARGET}-ld -r -b binary -o $@ $^
	${TARGET}-objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@

initrd.tar: ./initrd/
	tar -c -f $@ $^ -C $^

help:
	@echo "make [option] <vars>\n\
options:\n\
   run          run kernel in qemu\n\
   clean        remove kernel executable and object files\n\
   <none>       compile and link the kernel\n\
vars:\n\
   DEBUG=1      debugging enabled"
