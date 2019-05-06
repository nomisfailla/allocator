#include "../src/allocator.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    const int size = 16;
    char my_heap[size];
    heap_init((uintptr_t)my_heap, size);

    return 0;
}
