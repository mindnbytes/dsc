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

// init
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

// push
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

static void test_push_fail_huge_elem_size(void) {
  List a;
  // list_grow fails before list_push reads from dummy.
  unsigned char dummy = 0;

  assert(list_init(&a, SIZE_MAX / 8 + 1));
  assert(!list_push(&a, &dummy));
  assert(a.len == 0);
  assert(a.cap == 0);
  assert(a.data == NULL);
  assert(list_clear(&a));
}

static void test_push_success(void) {
  List a;
  float size = 10;
  assert(list_init(&a, sizeof(float)));
  for (float f = 0; f < size; f++) {
    assert(list_push(&a, &f));
    unsigned char *top = a.data + (a.len - 1) * a.elem_size;
    assert(memcmp(&f, top, a.elem_size) == 0);
  }
  assert(a.len == (size_t)size);
  assert(list_clear(&a));
}

// pop
static void test_pop_fail_null(void) {
  List *a = NULL;
  int a_val;
  assert(!list_pop(a, &a_val));
}

static void test_pop_fail_null_dst(void) {
  List a;
  assert(list_init(&a, sizeof(int)));
  int val = 22;
  assert(list_push(&a, &val));
  int *dst = NULL;
  assert(!list_pop(&a, dst));
  assert(list_clear(&a));
}

static void test_pop_one_fail_empty(void) {
  List a;
  assert(list_init(&a, sizeof(int)));
  int val = 22;
  assert(list_push(&a, &val));
  int dst;
  assert(list_pop(&a, &dst));
  assert(dst == val);
  assert(!list_pop(&a, &dst));
  assert(list_clear(&a));
}

static void test_pop_many(void) {
  List a;
  assert(list_init(&a, sizeof(int)));
  int size = 10;
  for (int i = 0; i < size; i++) {
    assert(list_push(&a, &i));
  }
  int dst;
  for (int i = size - 1; i >= 0; i--) {
    assert(list_pop(&a, &dst));
    assert(dst == i);
  }
  assert(list_clear(&a));
}

// clear
static void test_clear_fail(void) {
  List *a = NULL;
  assert(!list_clear(a));
}

static void test_clear_pass(void) {
  List a;
  assert(list_init(&a, sizeof(long)));
  long val = 42;
  assert(list_push(&a, &val));
  assert(a.len == 1);
  assert(list_clear(&a));
  assert(a.len == 0 && a.cap == 0 && a.data == NULL);
  assert(a.elem_size == sizeof(long));
}

// reuse
static void test_clear_and_use(void) {
  List a;
  int x = 21;
  int y = 42;
  assert(list_init(&a, sizeof(int)));
  assert(list_push(&a, &x));
  assert(list_clear(&a));
  assert(list_push(&a, &y));
  assert(list_clear(&a));
}

int main(void) {
  RUN_TEST(test_init_pass);
  RUN_TEST(test_init_fail);
  RUN_TEST(test_push_fail_null);
  RUN_TEST(test_push_fail_huge_elem_size);
  RUN_TEST(test_push_success);
  RUN_TEST(test_pop_fail_null);
  RUN_TEST(test_pop_fail_null_dst);
  RUN_TEST(test_pop_one_fail_empty);
  RUN_TEST(test_pop_many);
  RUN_TEST(test_clear_fail);
  RUN_TEST(test_clear_pass);
  RUN_TEST(test_clear_and_use);

  puts("All tests passed.");
  return EXIT_SUCCESS;
}
