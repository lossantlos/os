
.PHONY: build clean

build: ${P_ROOT}/kernel.bin

clean:
	rm -rf *.o ${P_ROOT}/kernel.bin

boot.o: arch/${ARCH}/boot.s
	${TARGET}-as $^ -o $@

${P_ROOT}/kernel.bin: main.o boot.o
	${CC} -T arch/${ARCH}/linker.ld -o $@ ${LDFLAGS} $^ -lgcc
