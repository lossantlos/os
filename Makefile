TARGET=x86_64-elf
QEMU= qemu-system-x86_64

CC=${TARGET}-gcc
LD=${TARGET}-ld
CFLAGS= -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS= -ffreestanding -O2 -nostdlib

P_ROOT=$(PWD)
export

.PHONY: run build clean help

build:
	make -C src/ build

run: build
	${QEMU} -kernel ${P_ROOT}/kernel.bin

clean:
	make -C src/ clean

help:
	@echo "make [option]\n\
options:\n\
\trun\tbuild kernel and run in qemu\n\
\tbuild\tcompile and link kernel\n\
\tclean\tclean executable and object files"
