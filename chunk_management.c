#include "my_malloc_manager.h"
#include <sys/mman.h> // For mmap
#include <stdlib.h>   // For EXIT_FAILURE
#include <errno.h>    // For errno
#include <error.h>    // For error function

MemoryChunkHeader *first_chunk;
uint16_t chunk_current_id;

void *create_new_chunk(void)
{

    // Use mmap to request memory from the OS
    void *mem = mmap(NULL, UNITS_PER_CHUNK * UNIT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED)
    {
        error(EXIT_FAILURE, errno, "Memory mapping failed");
    }

    // Initialize the MemoryChunkHeader
    MemoryChunkHeader *header = (MemoryChunkHeader *)mem;
    header->id = chunk_current_id++;
    header->chunk_total_units = UNITS_PER_CHUNK;
    header->chunk_available_units = UNITS_PER_CHUNK - STRUCT_CH_UNITS - BITMAP_UNITS;
    header->bitmap = (Bitmap)(mem + STRUCT_CH_UNITS * UNIT_SIZE); // The bitmap follows immediately after the MemoryChunkHeader
    header->bitmap_size = BITMAP_SIZE;
    header->next = NULL;

    set_or_clear_bits(0, header->bitmap, 0, 0, header->chunk_total_units);
    size_t occupied = STRUCT_CH_UNITS + BITMAP_UNITS;
    set_or_clear_bits(1, header->bitmap, 0, 0, occupied);

    // extern void set_or_clear_bits(int set, Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty);

    return header;
}
