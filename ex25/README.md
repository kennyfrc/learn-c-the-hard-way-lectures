## Mutation Pattern

1. Define null pointer
2. Pass the address of a pointer (esp. bytearray) to a mutating function
3. The mutating function takes the address, then allocates memory to the start of the bytearray (pointer)
4. Then this mutating function returns a return code, such as 1 or 0

## Variadic functions

This enables you to create your own `scanf` or `printf`, with `n` number of arguments

## Calloc vs Malloc

Calloc initializes 0, Malloc doesn't initialize anything so it'll throw a segfault if you don't do anything with it.

## Bytearrays

Plenty of things are essentially blocks of memory or byte arrays.

## Include Guards

```
#ifndef DEBUG_H
#define DEBUG_H

#endif
```
Those are called #include guards.

Once the header is included, it checks if a unique value (in this case HEADERFILE_H) is defined. Then if it's not defined, it defines it and continues to the rest of the page.

When the code is included again, the first ifndef fails, resulting in a blank file.

That prevents double declaration of any identifiers such as types, enums and static variables.
