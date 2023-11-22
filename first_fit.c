#include "my_malloc_manager.h"

int first_fit(unsigned char *bitmap, size_t bitmap_size, size_t units_needed)
{
    size_t bits_total = bitmap_size * 8;
    size_t i;
    size_t free_count = 0;
    size_t start_index = 0;

    for (i = 0; i < bits_total; i++)
    {
        size_t byte = i / 8;
        size_t bit = i % 8;

        if (!(bitmap[byte] & (1 << (7 - bit))))
        {
            if (free_count == 0)
            {
                start_index = i;
            }
            free_count++;

            if (free_count == units_needed)
            {
                size_t start_byte = start_index / 8;
                size_t start_bit = start_index % 8;

                set_or_clear_bits(1, bitmap, start_byte, start_bit, units_needed);

                return start_index;
            }
        }
        else
        {
            free_count = 0;
        }
    }

    return -1;
}
