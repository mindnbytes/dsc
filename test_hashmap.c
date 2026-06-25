#include "hashmap.h"

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

// Validate non-empty HashMap invariant
static void assert_hm_wit_cap_valid(HashMap *hm) {
  assert(hm);
  assert(hm->cap > 0);
  assert(hm->len >= 0 && hm->len <= hm->cap);
  assert(hm->entries);
  size_t len = 0;
  for (size_t i = 0; i < hm->cap; i++) {
    if (hm->entries[i].occupied) {
      len++;
      assert(hm->entries[i].key);
    }
  }
  assert(hm->len == len);
  assert((hm->len + 1) * 10 < hm->cap * 7);
}

// init with capacity
static void test_init_with_cap_fail(void) {
  HashMap *hm = NULL;
  assert(!hm_init_with_cap(hm, 16));

  HashMap hmap;
  assert(!hm_init_with_cap(&hmap, 0));
}

static void test_init_with_cap_success(void) {
  HashMap hm;
  size_t cap = 16;

  assert(hm_init_with_cap(&hm, cap));
  assert(hm.cap == cap);
  assert_hm_wit_cap_valid(&hm);

  hm_free(&hm);
}

int main(void) {
  RUN_TEST(test_init_with_cap_fail);
  RUN_TEST(test_init_with_cap_success);

  puts("All tests passed.");
  return EXIT_SUCCESS;
}
