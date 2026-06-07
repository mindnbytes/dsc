#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// Valid List invariant:
//
// - data == NULL iff cap == 0
// - len <= cap
// - if cap > 0, data points to cap elements
// - elem_size > 0
// - element = elem_size bytes of data
typedef struct {
  size_t cap;
  size_t len;
  size_t elem_size;
  unsigned char *data;
} List;

// List initializer
// fails on NULL pointer or 0 elem_size
// return false in this case
// otherwise return true (success)
bool list_init(List *l, size_t elem_size) {
  if (l == NULL || elem_size == 0)
    return false;
  l->cap = 0;
  l->len = 0;
  l->elem_size = elem_size;
  l->data = NULL;

  return true;
}

// Clears the list to the state
// of freshly initialized one.
// return false on failure,
// true otherwise
// fails if l == null
bool list_clear(List *l) {
  if (l == NULL)
    return false;
  free(l->data);
  l->cap = 0;
  l->len = 0;
  // keep elem_size

  return true;
}
