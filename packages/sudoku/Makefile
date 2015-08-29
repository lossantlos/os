
.PHONY: all clean

all: sudoku

clean:
	rm -rf *.o

SRCS = $(shell find . -name '*.c')

sudoku: $(SRCS:.c=.o)
	${CC} -o $@ $^
