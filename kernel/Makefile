
.PHONY: build clean

build: ${P_ROOT}/kernel.bin

clean:
	rm -rf *.o ${P_ROOT}/kernel.bin

boot.o: arch/${ARCH}/boot.s
	${TARGET}-as $^ -o $@

${P_ROOT}/kernel.bin: main.o boot.o
	#${CC} -T arch/${ARCH}/linker.ld ${P_ROOT}/libc/libc.a -o $@ ${LDFLAGS} $^ -lgcc
	${CC} -T arch/${ARCH}/linker.ld -L${P_ROOT}/libc/ -o $@ ${LDFLAGS} $^ -lgcc -lc
