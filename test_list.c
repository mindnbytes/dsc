#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(void) {
  RUN_TEST(test_init_pass);
  RUN_TEST(test_init_fail);

  puts("All tests passed.");
  return EXIT_SUCCESS;
}
