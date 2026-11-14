CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -g
THREAD_FLAGS = -pthread

.PHONY: all clean asan ubsan tsan

all: debug_demo core_demo

debug_demo: debug_demo.c
	$(CC) $(CFLAGS) $< -o $@

core_demo: core_demo.c
	$(CC) $(CFLAGS) $< -o $@

asan: asan_uaf.c
	$(CC) $(CFLAGS) -fsanitize=address $< -o asan_uaf

ubsan: ubsan_shift.c
	$(CC) $(CFLAGS) -fsanitize=undefined $< -o ubsan_shift

tsan: tsan_race.c
	$(CC) $(CFLAGS) -fsanitize=thread $(THREAD_FLAGS) $< -o tsan_race

clean:
	rm -f debug_demo core_demo asan_uaf ubsan_shift tsan_race
