CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -g

.PHONY: all clean

all: bitmap_demo

bitmap_demo: bitmap.o main.o
	$(CC) $(CFLAGS) $^ -o $@

bitmap.o: bitmap.c bitmap.h
	$(CC) $(CFLAGS) -c bitmap.c -o $@

main.o: main.c bitmap.h
	$(CC) $(CFLAGS) -c main.c -o $@

clean:
	rm -f bitmap.o main.o bitmap_demo
