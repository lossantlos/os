
.PHONY: build clean

build: ${P_ROOT}/kernel.bin

clean:
	rm -rf *.o ${P_ROOT}/kernel.bin

boot.o: arch/${ARCH}/boot.s
	${TARGET}-as $^ -o $@

${P_ROOT}/kernel.bin: main.o boot.o tty.o vga.o cpu.o ${P_ROOT}/libc/libc.a
	${CC} -T arch/${ARCH}/linker.ld -L${P_ROOT}/libc/ -o $@ ${LDFLAGS} $^ -lgcc -lc
