#include "allocator.h"

void* heap_start;

void heap_init(uintptr_t start, size_t size)
{
    heap_start = (void*)start;

    alloc_header_t* first = (alloc_header_t*)heap_start;
    first->size = size - sizeof(alloc_header_t);
    first->flags = 0;
}

void* malloc(size_t size)
{
    return 0;
}

void free(void* ptr)
{

}
