
.PHONY: build clean

OBJS = stdio/printf.o stdio/putchar.o stdio/puts.o string/strlen.o

build: libc.a

clean:
	rm -rf libc.a *.o */*.o

libc.a: ${OBJS}
	$(AR) rcs $@ $^
