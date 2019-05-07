#include "../src/allocator.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    const int size = 40 + (4 * sizeof(alloc_header_t));
    char my_heap[size];
    heap_init((void*)my_heap, size);

    void* a = my_malloc(10);
    void* b = my_malloc(10);
    void* c = my_malloc(10);
    void* d = my_malloc(10);

    my_free(a);
    my_free(c);
    my_free(d);
    my_free(b);

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
