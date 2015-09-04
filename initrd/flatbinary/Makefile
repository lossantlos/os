
PREFIX = i386-elf-
CC = ${PREFIX}gcc
LD = ${PREFIX}ld
AS = ${PREFIX}as
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs

.PHONY: clean

test.bin: main.o start.o
	${LD} -T flat.ld -o $@ $^

main.o: main.c
	${CC} ${CFLAGS} $^ -c -o $@

start.o: start.s
	${AS} -o $@ $<

clean:
	rm -rf *.o *.bin
