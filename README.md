# dsc

Minimal C implementations of common data structures for learning C engineering.

## Current structures

- `IntList`: growable list of `int`
- `List`: growable list of fixed-size elements copied by value

## Build and test

```sh
clang -std=c17 -Wall -Wextra -Wpedantic int_list.c test_int_list.c -o test_int_list
./test_int_list

clang -std=c17 -Wall -Wextra -Wpedantic list.c test_list.c -o test_list
./test_list
```

## Notes

This project favors simple, readable C over clever abstractions.

`List` stores shallow byte copies. If an element contains pointers, the pointed-to data remains owned by the caller.
