#Makefile for C library please run make from upper directory

ECHO = echo -en

.PHONY: build clean

ifeq ($(DEBUG), 0)
CC := @$(CC)
AR := @$(AR)
ECHO := @$(ECHO)
else
ECHO := @\#$(ECHO)
endif

SRCS = $(shell find . -name *.c)



build: lib/libc.a

clean:
	rm -rf lib/libc.a src/*/*.o

.c.o:
	$(ECHO) "  CC\t$@\n"
	$(CC) -c $(CFLAGS) -o $@ $<

lib/libc.a: $(SRCS:.c=.o)
	$(ECHO) "  AR\t$@\n"
	$(AR) r $@ $?
