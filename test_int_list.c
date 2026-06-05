#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "int_list.h"

#define RUN_TEST(fn)                                                           \
  do {                                                                         \
    printf("%s...\n", #fn);                                                    \
    fn();                                                                      \
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
  assert(l.data[n - 1] == n - 1);

  int_list_free(&l);
}

static void test_non_destructive_free(void) {
  IntList l = INT_LIST_INIT;
  int first = 24;
  int second = 42;
  assert(int_list_push(&l, first));
  int_list_free(&l);

  assert(int_list_push(&l, second));
  assert(l.len == 1);
  assert(l.data[0] == second);
  int_list_free(&l);
}

static void test_create_destroy(void) {
  IntList *l = int_list_create();
  assert(int_list_push(l, 42));
  assert(l->data[0] == 42);
  int_list_destroy(l);
}

static void test_pop(void) {
  IntList *l = int_list_create();
  assert(int_list_push(l, 1));

  int val;
  assert(int_list_pop(l, &val));
  assert(!int_list_pop(l, &val));
  assert(val == 1);
  assert(l->len == 0);
  int_list_destroy(l);
}

int main(void) {
  RUN_TEST(test_init_macro);
  RUN_TEST(test_init);
  RUN_TEST(test_push_to_empty);
  RUN_TEST(test_push_multiple);
  RUN_TEST(test_non_destructive_free);
  RUN_TEST(test_create_destroy);
  RUN_TEST(test_pop);

  printf("All tests passed.\n");
  return 0;
}
