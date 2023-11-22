#include "my_malloc_manager.h"
#include <stdio.h>

void print_bitmap(unsigned char *bitmap, size_t bitmap_size)
{
    size_t byte, bit;
    size_t bits_total = bitmap_size * 8;

    for (size_t i = 0; i < bits_total; ++i)
    {
        byte = i / 8;
        bit = i % 8;

        printf("%d", (bitmap[byte] & (1 << (7 - bit))) ? 1 : 0);

        if (bit == 7)
        {
            printf(" ");
        }
    }
    printf("\n");
}
