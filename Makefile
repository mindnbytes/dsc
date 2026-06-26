ifeq ($(origin CC),default)
CC := clang
endif

CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -g -O1
SANFLAGS ?= -fsanitize=address,undefined -fno-omit-frame-pointer

test_hashmap: test_hashmap.c hashmap.c hashmap.h
	$(CC) $(CFLAGS) $(SANFLAGS) test_hashmap.c hashmap.c -o test_hashmap

test: test_hashmap
	ASAN_OPTIONS=detect_leaks=1 ./test_hashmap

clean:
	rm -f test_hashmap
