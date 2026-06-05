#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "int_list.h"

#define RUN_TEST(fn)          \
    do {                      \
        printf("%s...\n", #fn); \
        fn();                 \
    } while (0)

static void test_init_macro(void) {
    IntList l = INT_LIST_INIT;

    assert(l.len == 0);
    assert(l.cap == 0);
    assert(l.data == NULL);
}

static void test_init(void) {
    IntList l;
    int_list_init(&l);

    assert(l.len == 0);
    assert(l.cap == 0);
    assert(l.data == NULL);

}

static void test_push_to_empty(void) {
    IntList l = INT_LIST_INIT;
    bool ok = int_list_push(&l, 42);

    assert(ok);
    assert(l.len == 1);
    assert(l.data[0] == 42);
    int_list_free(&l);
}

static void test_push_multiple(void) {
    IntList l = INT_LIST_INIT;
    bool ok;
    int n = 256;

    for (int i = 0; i < n; i++) {
        ok = int_list_push(&l, i);
        assert(ok);
    }
    assert(l.len == (size_t)n);
    assert(l.data[0] == 0);
    assert(l.data[n-1] == n-1);

    int_list_free(&l);
}


int main(void) {
    RUN_TEST(test_init_macro);
    RUN_TEST(test_init);
    RUN_TEST(test_push_to_empty);
    RUN_TEST(test_push_multiple);

    printf("All tests passed.\n");
    return 0;
}
