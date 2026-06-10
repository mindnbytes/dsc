#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

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

bool list_init(List *l, size_t elem_size);
bool list_push(List *l, const void *val);
bool list_pop(List *l, void *dst);
bool list_clear(List *l);

#endif
