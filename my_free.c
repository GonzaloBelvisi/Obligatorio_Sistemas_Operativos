#include "my_malloc_manager.h"
#include <assert.h>

static MemoryChunkHeader *find_chunk(void *ptr)
{
    MemoryChunkHeader *chunk = (MemoryChunkHeader *)first_chunk;
    while (chunk != NULL)
    {
        if (ptr > (void *)chunk && ptr < (void *)chunk + chunk->chunk_total_units * UNIT_SIZE)
        {
            return chunk;
        }
        chunk = chunk->next;
    }
    return NULL; // Si no se encuentra, algo está mal. Podría ser un assert o un manejo de error.
}

void my_free(void *ptr)
{
    if (!ptr)
    {
        // Si el ptr es NULL, no hacer nada
        return;
    }

    // Obtener el AllocationHeader, que está justo antes del bloque de memoria asignado
    AllocationHeader *alloc_header = (AllocationHeader *)ptr - 1;

    // Obtener el MemoryChunkHeader correspondiente
    MemoryChunkHeader *chunk_header = find_chunk(ptr);
    assert(chunk_header != NULL); // Si no se encuentra el chunk_header, hay un problema grave.

    // Para chunks grandes, simplemente incrementar las unidades disponibles
    if (chunk_header->is_large_allocation) {
        chunk_header->chunk_available_units += alloc_header->nunits;
        return;
    }

    // Para chunks normales, actualizar el bitmap
    uint16_t start_byte_index = alloc_header->bit_index / 8;
    uint16_t start_bit_index = alloc_header->bit_index % 8;

    clear_bits(chunk_header->bitmap, start_byte_index, start_bit_index, alloc_header->nunits);
    chunk_header->chunk_available_units += alloc_header->nunits;
}
