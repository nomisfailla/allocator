#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stdint.h>
#include <stdio.h> // For size_t

#define ALLOC_SIG 0xA110CA1E

typedef struct
{
    uint32_t sig;
    uint8_t flags;
    size_t size;
} alloc_header_t;

void heap_init(void* start, size_t size);

void* my_malloc(size_t size);
void my_free(void* ptr);

#endif
