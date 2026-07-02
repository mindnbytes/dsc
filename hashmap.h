#ifndef HASHMAP_H
#define HASHMAP_H

/* Minimal, educational Hashmap in C
with FNV-1a hash function; open address + linear probing
*/

#include <stdbool.h>
#include <stddef.h>

// HashMap Entry, own its key
// key != NULL if occupied
typedef struct {
  char *key;     // owned string or NULL
  size_t value;  // associated count
  bool occupied; // wether the slot is in use
} HmEntry;

// HashMap of HmEntry-s
// Invariant:
// - 0 <= len <= cap
// - if cap > 0, then entries != NULL
// - every occupied entry has key != NULL
// - len == number of occupied entries
// - load factor thresholds to 0.7
typedef struct {
  HmEntry *entries;
  size_t len;
  size_t cap;
} HashMap;

void hm_init(HashMap *hm);
bool hm_init_with_cap(HashMap *hm, size_t cap);
void hm_free(HashMap *hm);
bool hm_put(HashMap *hm, const char *key, size_t value);

#endif
