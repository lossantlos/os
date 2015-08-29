
.PHONY: build clean

build: ${P_ROOT}/kernel.bin

clean:
	rm -rf *.o ${P_ROOT}/kernel.bin

common_s.o: arch/${ARCH}/common.s
	${TARGET}-as $^ -o $@

common_asm.o: arch/${ARCH}/common.asm
	nasm -o $@ -f elf $^

entry.o: arch/${ARCH}/entry.s
	${TARGET}-as $^ -o $@

entry_c.o: arch/${ARCH}/entry.c
	${CC} ${CFLAGS} -c -o $@ $<

#ASM_SRCS = $(shell find . -name '*.asm' -o -name '*.s' )
SRCS = $(shell find . -not \( -path './arch/*' -prune \) \( -name '*.c' \))
OBJS = $(SRCS:.c=.o) entry.o entry_c.o common_asm.o common_s.o

${P_ROOT}/kernel.bin: ${OBJS} ${P_ROOT}/libc/libc.a ${P_ROOT}/initrd.o ${P_ROOT}/packages/*/*.o
	${CC} -T arch/${ARCH}/linker.ld -L${P_ROOT}/libc/ -o $@ ${LDFLAGS} $^ -lc
