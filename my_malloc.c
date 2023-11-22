#include "my_malloc_manager.h"

void *my_malloc(size_t nbytes)
{
    if (nbytes == 0)
    {
        return NULL;
    }

    // Calcula las unidades necesarias, incluyendo el AllocationHeader
    size_t units_needed = (nbytes + sizeof(AllocationHeader) + UNIT_SIZE - 1) / UNIT_SIZE;

    // Si el primer chunk no existe, crearlo
    if (!first_chunk)
    {
        first_chunk = create_new_chunk();
    }

    MemoryChunkHeader *current = first_chunk;

    while (current)
    {
        // Busca espacio libre en el chunk actual
        int start_index = first_fit(current->bitmap, current->bitmap_size, units_needed);
        if (start_index != -1)
        {
            // Calcula el índice de byte y bit de inicio
            size_t start_byte = start_index / 8;
            size_t start_bit = start_index % 8;

            // Marca el espacio como ocupado en el bitmap
            set_or_clear_bits(1, current->bitmap, start_byte, start_bit, units_needed);

            // Actualiza las unidades disponibles en el chunk
            current->chunk_available_units -= units_needed;

            // Calcula la dirección del bloque asignado
            AllocationHeader *alloc_header = (AllocationHeader *)((char *)current + sizeof(MemoryChunkHeader) + start_index * UNIT_SIZE);
            alloc_header->nunits = units_needed;
            alloc_header->bit_index = start_index;

            // Retorna un puntero al espacio justo después del AllocationHeader
            return (void *)(alloc_header + 1);
        }

        // Mueve al siguiente chunk si no hay espacio suficiente en el actual
        current = current->next;
    }

    // Si no se encuentra espacio en ningún chunk, retorna NULL
    return NULL;
}
