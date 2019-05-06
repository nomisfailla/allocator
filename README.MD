# Allocator
Simple allocator written in C.

## Dependencies
The allocator is designed to be dropped into osdev projects as-is. However, it does require fixed width integer types. By default it includes `stdint.h`, but feel free to change that to wherever your types are defined.

## Build Instructions
To build and run the example program, simply run;
```
gcc example/main.c src/allocator.c -o alloc
./alloc
```
