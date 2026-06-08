#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define RUN_TEST(fn)                                                           \
  do {                                                                         \
    printf("%s...\n", #fn);                                                    \
    fn();                                                                      \
  } while (0)

static void test_init_fail_null(void) {
  List *list = NULL;
  assert(!list_init(list, 2));
}

int main(void) {
  RUN_TEST(test_init_fail_null);
  puts("All tests passed.");
  return EXIT_SUCCESS;
}
