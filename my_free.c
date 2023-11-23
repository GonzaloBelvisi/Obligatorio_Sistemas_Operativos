#include "my_malloc_manager.h"
#include <assert.h>
#include <sys/mman.h>
#include <stdio.h> // Necesario para printf

// Encuentra el chunk y el chunk previo al puntero dado
static MemoryChunkHeader *find_chunk_and_prev(void *ptr, MemoryChunkHeader **prev_chunk) {
    MemoryChunkHeader *chunk = first_chunk;
    *prev_chunk = NULL;
    while (chunk != NULL) {
        if (ptr > (void *)chunk && ptr < (void *)chunk + chunk->chunk_total_units * UNIT_SIZE) {
            return chunk;
        }
        *prev_chunk = chunk;
        chunk = chunk->next;
    }
    return NULL; // Si no se encuentra, podría ser un assert o un manejo de error
}

void my_free(void *ptr) {
    if (!ptr) {
        // Si el ptr es NULL, no hacer nada
        return;
    }

    // Obtener el AllocationHeader, que está justo antes del bloque de memoria asignado
    AllocationHeader *alloc_header = (AllocationHeader *)ptr - 1;

    // Buscar el chunk y el chunk previo correspondiente
    MemoryChunkHeader *prev_chunk = NULL;
    MemoryChunkHeader *chunk_header = find_chunk_and_prev(ptr, &prev_chunk);
    assert(chunk_header != NULL); // Si no se encuentra el chunk_header, hay un problema grave

    // Si es un chunk grande, liberar la memoria al sistema
    if (chunk_header->is_large_allocation) {
        printf("Will eliminate chunk id %d, freed memory at address %p\n", chunk_header->id, ptr);
        if (prev_chunk) {
            prev_chunk->next = chunk_header->next; // Bypass the chunk being freed if it's not the first one
        } else {
            first_chunk = chunk_header->next; // If it's the first chunk, update the head of the list
        }
        // Liberar la memoria del chunk grande
        munmap(chunk_header, chunk_header->chunk_total_units * UNIT_SIZE);
        return;
    }

    // Para chunks normales, actualizar el bitmap y las unidades disponibles
    uint16_t start_byte_index = alloc_header->bit_index / 8;
    uint16_t start_bit_index = alloc_header->bit_index % 8;
    clear_bits(chunk_header->bitmap, start_byte_index, start_bit_index, alloc_header->nunits);
    chunk_header->chunk_available_units += alloc_header->nunits;
    printf("Freed memory at address %p\n", ptr);
}
