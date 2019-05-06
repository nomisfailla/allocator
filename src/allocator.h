#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stdint.h>
#include <stdio.h> // For size_t

typedef struct
{
    size_t size;
    uint8_t flags;
} alloc_header_t;

void heap_init(uintptr_t start, size_t size);

void* malloc(size_t size);
void free(void* ptr);

#endif
