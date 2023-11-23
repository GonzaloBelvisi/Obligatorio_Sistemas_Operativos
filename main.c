#include "my_malloc_manager.h"
#include <stdio.h>

void test_allocation(size_t size) {
    size_t units_needed = (size + sizeof(AllocationHeader) + UNIT_SIZE - 1) / UNIT_SIZE;
    printf("\n--- Probando asignación de %zu bytes ---\n", size);

    if (IS_LARGE_ALLOCATION(units_needed)) {
        printf("Para el large allocation...\n");
    }

    void *allocated = my_malloc(size);
    if (allocated != NULL) {
        printf("Asignación exitosa en: %p\n", allocated);
        my_free(allocated);
        printf("Memoria liberada de: %p\n", allocated);
    } else {
        printf("Fallo en la asignación de %zu bytes.\n", size);
    }
}


void print_chunk_info(MemoryChunkHeader *chunk, int chunk_count) {
    printf("\n[ Chunk #%d ]\n", chunk_count);
    if (chunk == NULL) {
        printf("Chunk: NULL\n");
        return;
    }
    printf("ID: %d | Total Units: %d | Available Units: %d\n",
           chunk->id, chunk->chunk_total_units, chunk->chunk_available_units);
    if (chunk->bitmap) {
        printf("Bitmap:\n");
        print_bitmap(chunk->bitmap, chunk->bitmap_size);
    } else {
        printf("Sin bitmap (chunk grande)\n");
    }
}

void test_various_allocations() {
    size_t test_sizes[] = {24, 256, 1024, 2048, 4096};
    size_t num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    for (size_t i = 0; i < num_tests; ++i) {
        test_allocation(test_sizes[i]);

        // Imprimir información de cada chunk
        MemoryChunkHeader *current = first_chunk;
        int chunk_count = 0;
        while (current != NULL) {
            print_chunk_info(current, ++chunk_count);
            current = current->next;
        }
    }
}

int main() {
    printf("\n=== Iniciando pruebas de asignación de memoria ===\n");
    test_various_allocations();
    printf("\n=== Pruebas completadas ===\n");
    return 0;
}
