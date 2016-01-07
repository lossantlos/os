# Makefile for my os

P_ROOT=$(PWD)

include make.conf

ifneq (${DEBUG},1)
DEBUG = 0
else
CFLAGS += -g -D DEBUG
LDFLAGS += -g
QEMU_FLAGS += -s -S
endif

include kernel/arch/${ARCH}/make.config

export

.PHONY: all run clean help debug doc todo disk_update

all: kernel.bin

doc: ./config.dox
	doxygen $<

libc/lib/libc.a: $(shell find libc/ -not -type d -not -name *.a)
	make -C libc/ build

packages/*/*.o: packages/*/*.c
	make -C packages/

kernel.bin: libc/lib/libc.a $(shell find kernel -name *.c -o -name *.s) initrd.o packages/*/*.o
	make -C kernel/ build



run: kernel.bin disk_update
ifeq ($(EMULATOR),bochs)
	${BOCHS} -q
else ifeq ($(EMULATOR),qemu)
	${QEMU} ${QEMU_FLAGS}
else
	@echo -e "Unknown emulator (variable EMULATOR), please look up help\n"
endif

clean:
	-rm -r *.o initrd.tar doc/doxygen-out/ disk.raw
	make -C libc/ clean
	make -C kernel/ clean
	make -C packages/ clean

todo:
	find kernel/ libc/ -type f -print0 | xargs -0 grep "TODO"

initrd.o: initrd.tar
	${TARGET}ld -r -b binary -o $@ $^
	${TARGET}objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@

initrd.tar: ./initrd/
	tar -c -f $@ $^ -C $^

disk_update: kernel.bin disk.raw
	sudo ./scripts/disk-image-mount.sh ./disk.raw ./mnt/
	sudo cp kernel.bin ./mnt/
	sudo cp -r ./filesystem/ ./mnt/
	sudo ./scripts/disk-image-umount.sh ./mnt/

disk.raw:
	sudo ./scripts/disk-image-new.sh $@
	sudo chown kuba $@ #TODO
	sudo chgrp kuba $@ #TODO

help:
	@echo -e "Default target:\n\
  compile and link the kernel\n\
\n\
Targets:\n\
  run          run kernel in (EMULATOR) default is qemu\n\
  clean        remove kernel executable and object files\n\
\n\
Variables:\n\
  DEBUG        enable or disable debuging (1=yes or 0=false)\n\
  DISK_IMG     generate disk image (1=yes or 0=false )\n\
  EMULATOR     choose emulator (\"qemu\" or \"bochs\")\n"
