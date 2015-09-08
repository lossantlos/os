# Makefile for my os

P_ROOT=$(PWD)

include make.conf

ifneq (${DEBUG},0)
DEBUG = 0
else
CFLAGS += -g -D DEBUG
LDFLAGS += -g
QEMU_FLAGS += -s -S
endif

include kernel/arch/${ARCH}/make.config

export

.PHONY: run all clean help debug debug_run doc

all: kernel.bin


test:
	@echo $(vpath %.c ./kernel/src)



doc: ./config.dox
	doxygen $<

libc/lib/libc.a: $(shell find libc/ -not -type d -not -name *.a)
	make -C libc/ build

packages/*/*.o: packages/*/*.c
	make -C packages/

kernel.bin: libc/lib/libc.a $(shell find kernel -name *.c -o -name *.s) initrd.o packages/*/*.o
	make -C kernel/ build

run: kernel.bin
	${QEMU} ${QEMU_FLAGS} -kernel $^

clean:
	-rm *.o initrd.tar doc/doxygen-out/
	make -C libc/ clean
	make -C kernel/ clean
	make -C packages/ clean



initrd.o: initrd.tar
	${TARGET}ld -r -b binary -o $@ $^
	${TARGET}objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@

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
