#include "my_malloc_manager.h"

void clear_bits(Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty)
{
    for (uint16_t i = 0; i < qty; ++i)
    {
        uint16_t byte_index = (start_bit_index + i) / 8 + start_byte_index;
        uint16_t bit_index = (start_bit_index + i) % 8;
        bitmap[byte_index] &= ~(1 << bit_index);
    }
}

void set_or_clear_bits(int set, Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty)
{
    for (uint16_t i = 0; i < qty; ++i)
    {
        uint16_t byte_index = (start_bit_index + i) / 8 + start_byte_index;
        uint16_t bit_index = (start_bit_index + i) % 8;
        if (set)
        {
            bitmap[byte_index] |= (1 << (7 - bit_index));
        }
        else
        {
            bitmap[byte_index] &= ~(1 << (7 - bit_index));
        }
    }
}
