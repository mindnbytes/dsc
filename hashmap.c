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

// zero init
void hm_init(HashMap *hm) {
  hm->cap = 0;
  hm->len = 0;
  hm->entries = NULL;
}

// init with capacity
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

// find slot for the key helper
static size_t hm_find_slot(HashMap *hm, const char *key, bool *found) {
  size_t idx = hash_cstr(key) % hm->cap;
  while (hm->entries[idx].occupied) {
    if (strcmp(key, hm->entries[idx].key) == 0) {
      *found = true;
      return idx;
    }
    idx = (idx + 1) % hm->cap;
  }
  *found = false;
  return idx;
}

// internal inserter of owned key, helper for the resize
// knows it can simply use pointer, no need to copy
static void hm_insert_owned(HashMap *hm, char *key, size_t value) {
  size_t idx = hash_cstr(key) % hm->cap;
  while (hm->entries[idx].occupied) {
    idx = (idx + 1) % hm->cap;
  }
  hm->entries[idx].key = key;
  hm->entries[idx].value = value;
  hm->entries[idx].occupied = true;
  hm->len++;
}

// internal resize
static bool hm_resize(HashMap *hm, size_t new_cap) {
  size_t old_cap = hm->cap;
  HmEntry *old_entries = hm->entries;

  HmEntry *new_entries = calloc(new_cap, sizeof(*new_entries));
  if (!new_entries) {
    return false;
  }
  hm->entries = new_entries;
  hm->cap = new_cap;
  hm->len = 0;

  for (size_t i = 0; i < old_cap; i++) {
    if (old_entries[i].occupied) {
      hm_insert_owned(hm, old_entries[i].key, old_entries[i].value);
    }
  }
  free(old_entries);
  return true;
}

bool hm_put(HashMap *hm, const char *key, size_t value) {
  if (!hm || !key) {
    return false;
  }
  // need non-null storage (check cap)
  if (hm->cap == 0) {
    if (!hm_resize(hm, 16)) {
      return false;
    }
  }
  // need to check if it should be resized (check load factor)
  // load factor 0.7

  // first avoid wrap-around of size_t
  if ((hm->len + 1) >= SIZE_MAX / 10 || hm->cap >= SIZE_MAX / 7) {
    // we are too large
    return false;
  }
  if ((hm->len + 1) * 10 >= hm->cap * 7) {
    if (!hm_resize(hm, hm->cap * 2)) {
      return false;
    }
  }
  // find slot
  bool found;
  size_t idx = hm_find_slot(hm, key, &found);
  // update value and return only if key is found
  if (found) {
    hm->entries[idx].value = value;
    return true;
  }
  // copy key to own it, can still fail to allocate
  char *key_copy = cstr_dup(key);
  if (!key_copy) {
    return false;
  }

  // update entry
  hm->entries[idx].key = key_copy;
  hm->entries[idx].value = value;
  hm->entries[idx].occupied = true;
  hm->len++;
  return true;
}
