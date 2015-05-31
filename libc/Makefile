
.PHONY: build clean

build: libc.a

clean:
	rm -rf libc.a *.o */*.o

SRCS = $(shell find . -name *.c)

libc.a: $(SRCS:.c=.o)
	$(AR) rcs $@ $^
