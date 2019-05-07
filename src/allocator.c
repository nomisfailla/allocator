#include "allocator.h"

heap_state_t state;
void* heap_start;
void* heap_end;

void heap_init(void* start, size_t size)
{
    heap_start = start;
    heap_end = heap_start + size;

    state.used = 0;
    state.used_total = 0;
    state.active_allocations = 0;
    state.header_size = sizeof(alloc_header_t);
    printf("sizeof(alloc_header_t) = %ld\n", sizeof(alloc_header_t));

    alloc_header_t* first = (alloc_header_t*)heap_start;
    first->sig = ALLOC_SIG;
    first->flags = 0;
    first->prev = 0;
    first->size = size - sizeof(alloc_header_t);
}

void* my_malloc(size_t size)
{
    void* current = heap_start;

    while(current < heap_end)
    {
        alloc_header_t* block = (alloc_header_t*)current;
        if(block->sig != ALLOC_SIG)
        {
            // TODO: Narrow this case down to only occuring when we are
            // out of heap space.
            printf("something went very wrong! out of memory perhaps?\n");
            return 0;
        }

        if(block->flags == 0 && block->size >= size)
        {
            size_t old_size = block->size;

            block->flags = 1;
            block->size = size;

            int next_size = old_size - block->size;

            if(next_size < sizeof(alloc_header_t))
            {
                // If we find that we cannot fit another header after this
                // block, we will increase the size of the in-progress
                // allocation to accommodate for the extra bytes that would
                // otherwise be lost.
                // This ensure that when we are iterating over the heap and
                // encounter a new block, we increase the current pointer
                // by the correct amount in order to find the next block.
                int lost_bytes = next_size;
                block->size += lost_bytes;
            }
            else
            {
                alloc_header_t* next = (alloc_header_t*)(current + block->size + sizeof(alloc_header_t));
                next->sig = ALLOC_SIG;
                next->flags = 0;
                next->prev = current;
                next->size = next_size - sizeof(alloc_header_t);
            }

            state.used += block->size;
            state.used_total += block->size + sizeof(alloc_header_t);
            state.active_allocations++;
            return (void*)(current + sizeof(alloc_header_t));
        }
        else
        {
            current += (sizeof(alloc_header_t) + block->size);
        }
    }

    printf("out of heap space!\n");
    return 0;
}

int heap_is_block_free(void* current)
{
    alloc_header_t* block = (alloc_header_t*)current;
    if(block->sig != ALLOC_SIG)
    {
        return 0;
    }

    if(block->flags == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void my_free(void* ptr)
{
    alloc_header_t* block = (alloc_header_t*)(ptr - sizeof(alloc_header_t));
    block->flags = 0;

    state.active_allocations--;
    state.used_total -= block->size + sizeof(alloc_header_t);
    state.used -= block->size;

    // March fowards and eat all free blocks that come after this one.
    void* current = ptr + block->size;
    size_t extra_size = 0;
    while(heap_is_block_free(current))
    {
        alloc_header_t* next = (alloc_header_t*)current;
        size_t total_size = (next->size + sizeof(alloc_header_t));
        extra_size += total_size;
        current += total_size;
    }

    // Forfeit the empty blocks to this newly freed block.
    block->size += extra_size;
}

int count_blocks()
{
    int blocks = 0;

    void* current = heap_start;
    while(current < heap_end)
    {
        alloc_header_t* block = (alloc_header_t*)current;
        printf("block @ %p, size: %ld\n", current, block->size);
        blocks++;
        current += (block->size + sizeof(alloc_header_t));
    }

    return blocks;
}

heap_state_t get_heap_state()
{
    return state;
}
