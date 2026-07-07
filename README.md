# dsc

Minimal C implementations of common data structures for learning C engineering.

## Current structures

- `IntList`: growable list of `int`
- `List`: growable list of fixed-size elements copied by value
- `HashMap`: string-keyed hash table using FNV-1a and linear probing; owns copied keys and stores `size_t` values

## Build and test

On MacOS ensure
```sh
clang --version >= 22
```
or

```sh
nix develop
make test
```

## Notes

This project favors simple, readable C over clever abstractions.

`List` stores shallow byte copies. If an element contains pointers, the pointed-to data remains owned by the caller.

`HashMap` owns its copied keys. Call `hm_free` before reinitializing a live map.
