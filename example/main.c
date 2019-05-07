#include "../src/allocator.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    const int size = 1024;
    char my_heap[size];
    heap_init((void*)my_heap, size);

    char* str1 = my_malloc(size - sizeof(alloc_header_t));
    if(!str1)
    {
        printf("failed to allocate\n");
    }
    else
    {
        str1[0] = 'A';
        str1[1] = 'B';
        str1[2] = 'C';
        str1[3] = '\0';

        printf("allocated str1 @ %p\n", str1);
        printf("%s\n", str1);
    }

    char* str2 = my_malloc(4);
    if(!str2)
    {
        printf("failed to allocate\n");
    }
    else
    {
        str2[0] = 'D';
        str2[1] = 'E';
        str2[2] = 'F';
        str2[3] = '\0';

        printf("allocated str2 @ %p\n", str2);
        printf("%s | %s\n", str2, str1);
    }

    my_free(str1);
    char* str3 = my_malloc(2);
    if(!str3)
    {
        printf("failed to allocate\n");
    }
    else
    {
        str3[1] = '\0';
        printf("allocated str3 @ %p\n", str3);
        printf("%s\n", str3);
    }

    FILE* ptr = fopen("heap.bin", "wb");
    fwrite(my_heap, size, 1, ptr);
    fclose(ptr);

    return 0;
}
