CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -g

.PHONY: all clean

all: array_demo

array_demo: array.o main.o
	$(CC) $(CFLAGS) $^ -o $@

libarray.a: array.o
	ar rcs $@ $^

array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c -o $@

main.o: main.c array.h
	$(CC) $(CFLAGS) -c main.c -o $@

clean:
	rm -f array.o main.o array_demo manual_demo library_demo libarray.a
