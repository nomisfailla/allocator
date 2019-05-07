#include "allocator.h"

void* heap_start;

void heap_init(void* start, size_t size)
{
    heap_start = start;

    printf("sizeof(alloc_header_t) = %ld\n", sizeof(alloc_header_t));

    alloc_header_t* first = (alloc_header_t*)heap_start;
    first->sig = ALLOC_SIG;
    first->flags = 0;
    first->size = size - sizeof(alloc_header_t);
}

void* my_malloc(size_t size)
{
    void* current = heap_start;

    while(1)
    {
        alloc_header_t* block = (alloc_header_t*)current;
        if(block->flags == 0 && block->size >= size)
        {
            size_t oldSize = block->size;

            block->flags = 1;
            block->size = size;

            alloc_header_t* next = (alloc_header_t*)(current + block->size + sizeof(alloc_header_t));
            next->sig = ALLOC_SIG;
            next->flags = 0;
            next->size = oldSize - (block->size + sizeof(alloc_header_t));

            return (void*)(current + sizeof(alloc_header_t));
        }
        else
        {
            current += (sizeof(alloc_header_t) + block->size);
        }
    }

    return 0;
}

void my_free(void* ptr)
{
    alloc_header_t* block = (alloc_header_t*)(ptr - sizeof(alloc_header_t));
    block->flags = 0;
}
