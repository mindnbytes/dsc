/*
 * List stores shallow copies of pushed elements.
 * If an element contains pointers, the pointed-to data remains owned by the
 * caller. list_clear only releases the list's internal storage.
 */
#include "list.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Private helper, grow internal array
// return false if it fais, true otherwise
static bool list_grow(List *list) {
  // next list->cap will overflow
  if (list->cap > SIZE_MAX / 2 / list->elem_size)
    return false;
  size_t new_cap = list->cap == 0 ? 8 : list->cap * 2;

  unsigned char *new_data = realloc(list->data, new_cap * list->elem_size);
  if (new_data == NULL)
    return false;

  list->data = new_data;
  list->cap = new_cap;

  return true;
}

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

// Push the shallow copy of the element
// on the list.
// - val must point to at least l->elem_size readable bytes.
// - returns false on failure
// - returns true on success
bool list_push(List *l, const void *val) {
  if (l == NULL || val == NULL)
    return false;

  if (l->len == l->cap) {
    if (!list_grow(l))
      return false;
  }
  // copy bytes
  unsigned char *dst = l->data + l->len * l->elem_size;
  memcpy(dst, val, l->elem_size);
  l->len++;
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
