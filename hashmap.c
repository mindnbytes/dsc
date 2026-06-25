/* Minimal, educational Hashmap in C
with FNV-1a hash function; open address + linear probing
*/
#include "hashmap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// FNV-1a hash function
static uint64_t hash_cstr(const char *s) {
  uint64_t hash = 14695981039346656037ull;

  while (*s) {
    hash ^= (unsigned char)*s;
    hash *= 1099511628211ull;
    s++;
  }

  return hash;
}

// C-str dup function
static char *cstr_dup(const char *s) {
  size_t len = strlen(s);
  char *copy = malloc(len + 1);
  if (!copy)
    return NULL;

  memcpy(copy, s, len + 1);
  return copy;
}

bool hm_init_with_cap(HashMap *hm, size_t cap) {
  if (!hm || cap == 0) {
    return false;
  }
  HmEntry *entries = calloc(cap, sizeof(*entries));
  if (!entries) {
    return false;
  }
  hm->entries = entries;
  hm->cap = cap;
  hm->len = 0;
  return true;
}

void hm_free(HashMap *hm) {
  if (!hm) {
    return;
  }
  for (size_t i = 0; i < hm->cap; i++) {
    if (hm->entries[i].occupied) {
      free(hm->entries[i].key);
    }
  }
  free(hm->entries);
}
