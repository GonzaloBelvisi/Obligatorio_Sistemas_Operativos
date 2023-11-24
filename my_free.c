#include "my_malloc_manager.h"
#include <assert.h>
#include <sys/mman.h>
#include <stdio.h>

static MemoryChunkHeader *find_chunk(void *ptr) {
    AllocationHeader *alloc_header = (AllocationHeader *)ptr - 1;
    return (MemoryChunkHeader *)((char *)alloc_header - alloc_header->bit_index * UNIT_SIZE - sizeof(MemoryChunkHeader));
}


void my_free(void *ptr) {
    if (!ptr) {
        return;
    }

    AllocationHeader *alloc_header = (AllocationHeader *)ptr - 1;

    MemoryChunkHeader *prev_chunk = NULL;
    MemoryChunkHeader *chunk_header = find_chunk(ptr);
    assert(chunk_header != NULL);

    if (chunk_header->is_large_allocation) {
        printf("Will eliminate chunk id %d, freed memory at address %p\n", chunk_header->id, ptr);
        if (prev_chunk) {
            prev_chunk->next = chunk_header->next;
        } else {
            first_chunk = chunk_header->next;
        }
        munmap(chunk_header, chunk_header->chunk_total_units * UNIT_SIZE);
        return;
    }

    uint16_t start_byte_index = alloc_header->bit_index / 8;
    uint16_t start_bit_index = alloc_header->bit_index % 8;
    
    // Establecer los bits en el bitmap correspondientes a la asignación liberada a 0
    set_or_clear_bits(0, chunk_header->bitmap, start_byte_index, start_bit_index, alloc_header->nunits);

    // Actualizar el contador de unidades disponibles en el chunk
    chunk_header->chunk_available_units += alloc_header->nunits;
}
