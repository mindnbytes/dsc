ifeq ($(origin CC),default)
CC := clang
endif

CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -g -O1
SANFLAGS ?= -fsanitize=address,undefined,leak -fno-omit-frame-pointer

TEST_BINS := test_hashmap test_int_list test_list

test_hashmap: test_hashmap.c hashmap.c hashmap.h
	$(CC) $(CFLAGS) $(SANFLAGS) test_hashmap.c hashmap.c -o test_hashmap

test_int_list: test_int_list.c int_list.c int_list.h
	$(CC) $(CFLAGS) $(SANFLAGS) test_int_list.c int_list.c -o test_int_list

test_list: test_list.c list.c list.h
	$(CC) $(CFLAGS) $(SANFLAGS) test_list.c list.c -o test_list

test: $(TEST_BINS)
	./test_hashmap
	./test_int_list
	./test_list

clean:
	rm -f $(TEST_BINS)

.PHONY: test clean $(TEST_BINS)
