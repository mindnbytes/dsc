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
static void assert_hm_non_empty_valid(HashMap *hm) {
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
  assert_hm_non_empty_valid(&hm);

  hm_free(&hm);
}

// init tolerates null
static void test_init_null(void) { assert(!hm_init(NULL)); }

// ensure lazy init works as expected
static void test_init(void) {
  HashMap hm;
  assert(hm_init(&hm));
  assert(hm.cap == 0 && hm.len == 0 && hm.entries == NULL);

  HashMap hm1 = {0};
  assert(hm1.cap == 0 && hm1.len == 0 && hm1.entries == NULL);
}

// free
// put after free -> ensure no use after free
static void test_free_and_put(void) {
  HashMap hm = {0};
  assert(hm_put(&hm, "first", 1));
  hm_free(&hm);

  assert(hm_put(&hm, "second", 2));
  assert_hm_non_empty_valid(&hm);
  assert(hm.len == 1);
  // TODO: test key:value

  hm_free(&hm);
}

// put
// nulls
static void test_put_null(void) {
  HashMap *hm = NULL;
  assert(!hm_put(hm, "hello", 42));

  HashMap hm1 = {0};
  assert(!hm_put(&hm1, NULL, 42));
}

// put one: ensures empty -> resize and invariant for non-empty after put
static void test_put_one(void) {
  HashMap hm = {0};
  assert(hm_put(&hm, "hello", 42));
  assert_hm_non_empty_valid(&hm);
  // TODO: ensure correct value!
  hm_free(&hm);
}

// put twice to the same key -> ensures key found branch works
static void test_put_key_twice(void) {
  HashMap hm = {0};

  assert(hm_put(&hm, "hello", 42));
  assert(hm_put(&hm, "hello", 999));
  assert_hm_non_empty_valid(&hm);
  assert(hm.len == 1);
  // TODO: ensure correct value!
  hm_free(&hm);
}

// put many values to test resizes
static void test_put_many(void) {
  HashMap hm = {0};
  size_t len = 64;
  char key[64];
  for (size_t i = 0; i < len; i++) {
    if (snprintf(key, sizeof(key), "num: %zu", i) > 0) {
      assert(hm_put(&hm, key, i));
    }
  }
  assert_hm_non_empty_valid(&hm);
  // TODO: assert key value pairs

  hm_free(&hm);
}

int main(void) {
  RUN_TEST(test_init_with_cap_fail);
  RUN_TEST(test_init_with_cap_success);
  RUN_TEST(test_init_null);
  RUN_TEST(test_init);
  RUN_TEST(test_free_and_put);
  RUN_TEST(test_put_null);
  RUN_TEST(test_put_one);
  RUN_TEST(test_put_key_twice);
  RUN_TEST(test_put_many);

  puts("All tests passed.");
  return EXIT_SUCCESS;
}
