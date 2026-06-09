#include "list.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUN_TEST(fn)                                                           \
  do {                                                                         \
    printf("%s...\n", #fn);                                                    \
    fn();                                                                      \
  } while (0)

static void test_init_pass(void) {
  List a;
  assert(list_init(&a, 2));
  assert(a.cap == 0);
  assert(a.len == 0);
  assert(a.elem_size == 2);
}

static void test_init_fail(void) {
  List *a = NULL;
  List b;
  assert(!list_init(a, 2));
  assert(!list_init(&b, 0));
}

static void test_push_fail_null(void) {
  List *a = NULL;
  int var = 11;
  assert(!list_push(a, &var));
  List b;
  assert(list_init(&b, sizeof(int)));
  int *elem = NULL;
  assert(!list_push(&b, elem));
  assert(list_clear(&b));
}

static void test_push_fail_grow(void) {
  List a;
  assert(list_init(&a, sizeof(int)));
  // make cap artificially large
  size_t size = SIZE_MAX / 2 / a.elem_size;
  a.cap = size + 1;
  a.len = a.cap;
  int val = 2;
  assert(!list_push(&a, &val));
  assert(list_clear(&a));
}

static void test_push_success(void) {
  List a;
  assert(list_init(&a, sizeof(float)));
  for (float f = 0; f < 10; f++) {
    assert(list_push(&a, &f));
    unsigned char *top = a.data + (a.len - 1) * a.elem_size;
    assert(memcmp(&f, top, a.elem_size) == 0);
  }
  assert(list_clear(&a));
}

int main(void) {
  RUN_TEST(test_init_pass);
  RUN_TEST(test_init_fail);
  RUN_TEST(test_push_fail_null);
  RUN_TEST(test_push_fail_grow);
  RUN_TEST(test_push_success);

  puts("All tests passed.");
  return EXIT_SUCCESS;
}
