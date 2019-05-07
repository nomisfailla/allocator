#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stdint.h>
#include <stdio.h> // For size_t

#define ALLOC_SIG 0xA110CA1E

typedef struct
{
    // Amount of data allocated.
    uint32_t used;

    // Amount of data allocated, including headers.
    uint32_t used_total;

    // Number of allocations which have not been freed.
    uint32_t active_allocations;

    // Size of the allocation header.
    size_t header_size;
} heap_state_t;

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
