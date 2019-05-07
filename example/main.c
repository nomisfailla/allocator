#include "../src/allocator.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    const int size = 30 + (3 * sizeof(alloc_header_t));
    char my_heap[size];
    heap_init((void*)my_heap, size);

    void* a = my_malloc(10);
    void* b = my_malloc(10);
    void* c = my_malloc(10);

    my_free(a);
    my_free(c);
    // At this stage the heap should look like;
    // AAAA|BBBB|CCCC
    // where A and C are free blocks.

    my_free(b);
    // Now we want the heap to look like;
    // AAAAAAAAAAAAAA
    // where A is one big free block, but instead we get;
    // AAAA|BBBB|CCCC
    // where A, B and C are free blocks, but we cannot allocate something
    // that is bigger than any one of these individual blocks.

    heap_state_t state = get_heap_state();
    printf("used: %d\n", state.used);
    printf("used_total: %d\n", state.used_total);
    printf("active_allocations: %d\n", state.active_allocations);
    printf("header_size: %ld\n", state.header_size);
    printf("block_count: %d\n", count_blocks());

    FILE* ptr = fopen("heap.bin", "wb");
    fwrite(my_heap, size, 1, ptr);
    fclose(ptr);

    return 0;
}
