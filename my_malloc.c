#include "my_malloc_manager.h"
#include <stdio.h>

void *my_malloc(size_t nbytes) {
    if (nbytes == 0) {
        return NULL;
    }

    size_t units_needed = (nbytes + sizeof(AllocationHeader) + UNIT_SIZE - 1) / UNIT_SIZE;

    // Si es una asignación grande, manejarla primero
    if (IS_LARGE_ALLOCATION(units_needed)) {
        MemoryChunkHeader *large_chunk = create_new_chunk(units_needed, 1, NULL);
        if (!large_chunk) {
            perror("Error al crear un chunk grande en my_malloc");
            return NULL;
        }

        // Inserta el chunk grande después del primer chunk
        if (first_chunk) {
            large_chunk->next = first_chunk->next;
            first_chunk->next = large_chunk;
        } else {
            first_chunk = large_chunk;
        }

        // Asignar la memoria en el chunk grande
        AllocationHeader *alloc_header = (AllocationHeader *)((char *)large_chunk + sizeof(MemoryChunkHeader));
        alloc_header->nunits = units_needed;
        alloc_header->bit_index = 0;  // No hay bitmap, por lo que el índice de bit siempre será 0

        printf("Asignación exitosa en nuevo chunk grande: %zu bytes en %p\n", nbytes, (void *)(alloc_header + 1));
        return (void *)(alloc_header + 1);
    }

    // Revisa los chunks existentes para asignaciones no grandes
    MemoryChunkHeader *current = first_chunk;
    while (current) {
        if (!current->is_large_allocation && current->chunk_available_units >= units_needed) {
            int start_index = first_fit(current->bitmap, current->bitmap_size, units_needed);
            if (start_index != -1) {
                size_t start_byte = start_index / 8;
                size_t start_bit = start_index % 8;

                set_or_clear_bits(1, current->bitmap, start_byte, start_bit, units_needed);
                current->chunk_available_units -= units_needed;

                AllocationHeader *alloc_header = (AllocationHeader *)((char *)current + sizeof(MemoryChunkHeader) + start_index * UNIT_SIZE);
                alloc_header->nunits = units_needed;
                alloc_header->bit_index = start_index;

                printf("Asignación exitosa: %zu bytes en %p\n", nbytes, (void *)(alloc_header + 1));
                return (void *)(alloc_header + 1);
            }
        }
        current = current->next;
    }

    // Crear un nuevo chunk normal si no se encontró espacio
    MemoryChunkHeader *new_chunk = create_new_chunk(UNITS_PER_CHUNK, 0, NULL);
    if (!new_chunk) {
        perror("Error al crear un nuevo chunk normal en my_malloc");
        return NULL;
    }

    // Inserta el nuevo chunk como el segundo en la lista
    if (first_chunk) {
        new_chunk->next = first_chunk->next;
        first_chunk->next = new_chunk;
    } else {
        first_chunk = new_chunk;
    }

    // Asigna en el nuevo chunk normal
    int start_index = first_fit(new_chunk->bitmap, new_chunk->bitmap_size, units_needed);
    if (start_index != -1) {
        size_t start_byte = start_index / 8;
        size_t start_bit = start_index % 8;

        set_or_clear_bits(1, new_chunk->bitmap, start_byte, start_bit, units_needed);
        new_chunk->chunk_available_units -= units_needed;

        AllocationHeader *alloc_header = (AllocationHeader *)((char *)new_chunk + sizeof(MemoryChunkHeader) + start_index * UNIT_SIZE);
        alloc_header->nunits = units_needed;
        alloc_header->bit_index = start_index;

        printf("Asignación exitosa en nuevo chunk normal: %zu bytes en %p\n", nbytes, (void *)(alloc_header + 1));
        return (void *)(alloc_header + 1);
    }

    printf("No se encontró espacio incluso después de crear un nuevo chunk para %zu bytes.\n", nbytes);
    return NULL;
}
