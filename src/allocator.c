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
    first->size = size - sizeof(alloc_header_t);
}

void* my_malloc(size_t size)
{
    void* current = heap_start;

    while(current < heap_end)
    {
        alloc_header_t* block = (alloc_header_t*)current;
        if(block->flags == 0 && block->size >= size)
        {
            size_t old_size = block->size;

            block->flags = 1;
            block->size = size;

            int next_size = old_size - block->size;

            if(next_size < sizeof(alloc_header_t))
            {
                // If the size of the next block cannot hold another header,
                // we should probably find somewhere else to put this current
                // allocation, but for now, we will allow it.
                // The problem with this is now we have ?some? amount of bytes
                // which are not accounted for by the allocator, these bytes
                // sit between the block currently being allocated and the
                // block which was after it.
                // Here I refer the dead space between allocation blocks as
                // "no mans land", and fill it all with 0xDE to signify it
                // as such.
                // Okay so actually this needs to be fixed as soon as possible.
                // Basically when we are iterating new blocks we jump over the
                // size of them and when we have these dead bytes, it fucks
                // with the whole system because the allocator expects to find
                // another header at the end of the currentl allocation, but
                // instead it finds dead bytes and interprets them as the
                // header. We could check to see if the bytes at the proposed
                // header location are dead bytes and increment till we find
                // the header signature, but thats nasty and I have a better
                // idea, store two sizes for the blob, the size which was
                // asked for by malloc, and the size of the block including
                // dead bytes.

                // Alternative, instead of having two sizes, keep the one size
                // and just add the dead bytes to it, sure the user might get
                // a few extra bytes, but they dont care? The size is used by
                // the allocator only.

                int lost_bytes = next_size;
                char* no_mans_land = current + sizeof(alloc_header_t) + block->size;
                for(int i = 0; i < lost_bytes; i++)
                {
                    *(no_mans_land) = 0xDE;
                }

                printf("cant fit header (%d), ignoring...\n", lost_bytes);
            }
            else
            {
                alloc_header_t* next = (alloc_header_t*)(current + block->size + sizeof(alloc_header_t));
                next->sig = ALLOC_SIG;
                next->flags = 0;
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

    return 0;
}

void my_free(void* ptr)
{
    alloc_header_t* block = (alloc_header_t*)(ptr - sizeof(alloc_header_t));
    block->flags = 0;

    state.active_allocations--;
    state.used_total -= block->size + sizeof(alloc_header_t);
    state.used -= block->size;
}
