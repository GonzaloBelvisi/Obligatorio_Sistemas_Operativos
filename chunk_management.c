#include "my_malloc_manager.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

MemoryChunkHeader *first_chunk = NULL;
uint16_t chunk_current_id = 0;

void *create_new_chunk(uint16_t units_needed, int is_large_allocation, MemoryChunkHeader *next) {
    // Verificar que el número de unidades no sea excesivamente grande
    if (units_needed > MAX_UNITS_ALLOWED) {
        printf("Solicitud de chunk demasiado grande: %u unidades.\n", units_needed);
        return NULL;
    }

    size_t chunk_size = units_needed * UNIT_SIZE;
    if (!is_large_allocation) {
        chunk_size = UNITS_PER_CHUNK * UNIT_SIZE;
    }

    void *mem = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED) {
        perror("Error en mmap para create_new_chunk");
        return NULL;
    }


    // Inicializar el encabezado del chunk
    MemoryChunkHeader *header = (MemoryChunkHeader *)mem;
    header->id = chunk_current_id++;
    header->is_large_allocation = is_large_allocation;
    header->chunk_total_units = units_needed;
    header->next = next;


    //Si es Large allocation
    if (is_large_allocation) {
        header->chunk_available_units = units_needed;
        header->bitmap = NULL;
        header->bitmap_size = 0;
        printf("Chunk grande creado: %u unidades.\n", units_needed);
    } else { //si es normal
        header->chunk_available_units = UNITS_PER_CHUNK - STRUCT_CH_UNITS - BITMAP_UNITS;
        header->bitmap = (Bitmap)(mem + STRUCT_CH_UNITS * UNIT_SIZE);
        header->bitmap_size = BITMAP_SIZE;

        set_or_clear_bits(0, header->bitmap, 0, 0, header->chunk_total_units);
        size_t occupied = STRUCT_CH_UNITS + BITMAP_UNITS;
        set_or_clear_bits(1, header->bitmap, 0, 0, occupied);
        printf("Chunk normal creado: %u unidades.\n", units_needed);
    }

    return header;
}
