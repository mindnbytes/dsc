# dsc

Minimal C implementations of common data structures for learning C engineering.

## Current structures

- `IntList`: growable list of `int`

## Build and test

```sh
clang -std=c17 -Wall -Wextra -Wpedantic int_list.c test_int_list.c -o test_int_list
./test_int_list
```

## Notes

This project favors simple, readable C over clever abstractions.
