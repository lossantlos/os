
.PHONY: build clean

build: ${P_ROOT}/kernel.bin

clean:
	rm -rf ./*.o ./*/*.o ${P_ROOT}/kernel.bin

${ARCH}_%_s.o: arch/${ARCH}/%.s
	${AS} $^ -o $@

${ARCH}_%_asm.o: arch/${ARCH}/%.asm
	nasm -o $@ -f elf $^

${ARCH}_%_c.o: arch/${ARCH}/%.c
	${CC} ${CFLAGS} -c -o $@ $<


ifeq (${DEBUG}, 0)
.c.o:
	@echo -en "  CC\t$@\n"
	@${CC} -c ${CFLAGS} -o $@ $< #&> /dev/null #<- for non verbose output
endif


#ASM_SRCS = $(shell find . -name '*.asm' -o -name '*.s' )
SRCS = $(shell find . -not \( -path './arch/*' -prune \) \( -name '*.c' \))
OBJS = $(SRCS:.c=.o) $(shell echo ${ARCH}_{entry_s,entry_c,common_asm,common_s}.o)

${P_ROOT}/kernel.bin: ${OBJS} ${P_ROOT}/libc/lib/libc.a ${P_ROOT}/initrd.o ${P_ROOT}/packages/*/*.o
	${CC} -T arch/${ARCH}/linker.ld -L${P_ROOT}/libc/lib/ -o $@ ${LDFLAGS} $^ -lc
