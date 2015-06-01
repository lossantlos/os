
.PHONY: build clean

build: ${P_ROOT}/kernel.bin

clean:
	rm -rf *.o ${P_ROOT}/kernel.bin

boot.o: arch/${ARCH}/boot.s
	${TARGET}-as $^ -o $@

asm.o: arch/${ARCH}/common.asm
	nasm -o $@ -f elf $^
	#${TARGET}-as $^ -o $@

OBJS = main.o boot.o tty.o vga.o cpu.o asm.o gdt.o idt.o irq.o isrs.o timer.o pcspkr.o

${P_ROOT}/kernel.bin: ${OBJS} ${P_ROOT}/libc/libc.a
	${CC} -T arch/${ARCH}/linker.ld -L${P_ROOT}/libc/ -o $@ ${LDFLAGS} $^ -lgcc -lc
