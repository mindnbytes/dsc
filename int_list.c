/*
 * IntList - dynamic array.
 * Educational dynamic array implementation for type int.
 * Not-generic, no extra features, only basics.
 * It allows a caller to create a new object, push to the end,
 * pop from the end, free object.
 */

#include "int_list.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool int_list_grow(IntList *list);

// Init a valid list
// - caller owns list
// - caller must free via int_list_free
void int_list_init(IntList *list) {
  list->cap = 0;
  list->len = 0;
  list->data = NULL;
}

// Frees only internal data and resets the
// caller's owned object.
void int_list_free(IntList *list) {
  free(list->data);
  list->cap = 0;
  list->len = 0;
  list->data = NULL;
}

// Creates list on the heap and returns pointer
// - API owns resources
// - Caller must release via int_list_destroy
IntList *int_list_create(void) {
  IntList *list = malloc(sizeof(*list));
  if (list == NULL)
    return NULL;
  int_list_init(list);
  return list;
}

// Destroys list object created via
// int_list_create
// - must not use pointer after
void int_list_destroy(IntList *list) {
  if (list == NULL)
    return;
  int_list_free(list);
  free(list);
}

// Push on top of the IntList
// returns false, if failed to grow
// doesn't change old pointer on failure
bool int_list_push(IntList *list, int val) {
  if (list->len == list->cap) {
    if (!int_list_grow(list))
      return false;
  }
  list->data[list->len] = val;
  list->len++;
  return true;
}

// Pop from the list into ret_value arg
// returns false if pop attempted on empty list
// doesn't touch ret_value on failure
bool int_list_pop(IntList *list, int *ret_val) {
  if (list->len == 0)
    return false;
  list->len--;
  *ret_val = list->data[list->len];
  return true;
}

// Private helper, grow internal array
// return false if it fais, true otherwise
static bool int_list_grow(IntList *list) {
  // next list->cap will overflow
  if (list->cap > SIZE_MAX / 2)
    return false;
  size_t new_cap = list->cap == 0 ? 8 : list->cap * 2;
  // check for overflow again
  if (new_cap > SIZE_MAX / sizeof(list->data[0]))
    return false;

  int *new_data = realloc(list->data, new_cap * sizeof(list->data[0]));
  if (new_data == NULL)
    return false;

  list->data = new_data;
  list->cap = new_cap;

  return true;
}
