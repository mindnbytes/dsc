/* Minimal, educational Hashmap in C
with FNV-1a hash function; open address + linear probing
*/
#include "hashmap.h"
#include <assert.h>
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
bool hm_init(HashMap *hm) {
  if (!hm) {
    return false;
  }
  hm->cap = 0;
  hm->len = 0;
  hm->entries = NULL;
  return true;
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
  hm->entries = NULL;
  hm->len = 0;
  hm->cap = 0;
}

// find slot for the key helper
// defensive against full table, will return false only if looking for index
// iteration reached hm->cap
static bool hm_find_slot(const HashMap *hm, const char *key, size_t *ret_ix,
                         bool *found) {
  size_t idx = hash_cstr(key) % hm->cap;
  // harden against potential infinite loop
  size_t cnt = 0;
  while (hm->entries[idx].occupied) {
    if (strcmp(key, hm->entries[idx].key) == 0) {
      *found = true;
      *ret_ix = idx;
      return true;
    }
    idx = (idx + 1) % hm->cap;
    cnt++;
    if (cnt >= hm->cap) {
      return false;
    }
  }
  *found = false;
  *ret_ix = idx;
  return true;
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

typedef enum { HM_GROW_NO, HM_GROW_YES, HM_GROW_ERR } HmGrowResult;

// checks if map should grow, writes ret_new_cap if yes
// ERR on sizt_t wrap around
static HmGrowResult hm_should_grow(const HashMap *hm, size_t *ret_new_cap) {
  if (hm->cap == 0) {
    *ret_new_cap = 16;
    return HM_GROW_YES;
  }
  if (hm->len >= SIZE_MAX - 1) {
    return HM_GROW_ERR;
  }
  size_t next_len = hm->len + 1;
  if (next_len > SIZE_MAX / 10 || hm->cap > SIZE_MAX / 7) {
    return HM_GROW_ERR;
  }

  if (next_len * 10 < hm->cap * 7) {
    return HM_GROW_NO;
  }

  if (hm->cap > SIZE_MAX / 2) {
    return HM_GROW_ERR;
  }

  *ret_new_cap = hm->cap * 2;
  return HM_GROW_YES;
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
    if (!hm_init_with_cap(hm, 16)) {
      return false;
    }
  }
  // if key exist, we can update and return early
  bool found;
  size_t idx;
  if (!hm_find_slot(hm, key, &idx, &found)) {
    return false;
  }
  // update value and return only if key is found
  if (found) {
    hm->entries[idx].value = value;
    return true;
  }

  // need to check if it should be resized
  size_t new_cap;
  HmGrowResult grow = hm_should_grow(hm, &new_cap);
  if (grow == HM_GROW_ERR) {
    return false;
  }
  if (grow && !hm_resize(hm, new_cap)) {
    return false;
  }
  // find index after resize
  if (!hm_find_slot(hm, key, &idx, &found)) {
    return false;
  }
  assert(!found); // can't bee true

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

// get
bool hm_get(const HashMap *hm, const char *key, size_t *out_value) {
  if (!hm || !key || !out_value) {
    return false;
  }
  if (hm->len == 0) {
    return false;
  }
  // look for the index
  bool found;
  size_t idx;
  if (!hm_find_slot(hm, key, &idx, &found)) {
    return false;
  }
  if (!found) {
    return false;
  }
  *out_value = hm->entries[idx].value;
  return true;
}
