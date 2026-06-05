#ifndef INT_LIST_H
#define INT_LIST_H

#include <stdbool.h>
#include <stddef.h>

// Valid IntList invariant:
//
// - data == NULL iff cap == 0
// - len <= cap
// - if cap > 0, data points to cap ints
typedef struct {
  size_t cap;
  size_t len;
  int *data;
} IntList;

#define INT_LIST_INIT {0}

void int_list_init(IntList *list);
void int_list_free(IntList *list);
IntList *int_list_create(void);
void int_list_destroy(IntList *list);
bool int_list_push(IntList *list, int val);

#endif
