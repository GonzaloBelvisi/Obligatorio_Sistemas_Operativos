#include "my_malloc_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Prototipos de funciones
void print_chunk_data();
void malloc_shell();

int main() {
    // Inicialización de variables globales
    first_chunk = NULL;
    chunk_current_id = 0;

    // Ejecutar la shell de malloc
    malloc_shell();

    return 0;
}

void malloc_shell() {
    char command[64];
    size_t size;
    void *ptr;

    printf("Malloc Shell\n");
    printf("Commands:\n");
    printf("  malloc <size>   -- Allocates <size> bytes\n");
    printf("  free <address>  -- Frees the memory at <address>\n");
    printf("  quit            -- Exit the shell\n");

    while (1) {
        printf("> ");
        if (scanf("%63s", command) != 1) break;

        if (strcmp(command, "malloc") == 0) {
            if (scanf("%zu", &size) != 1) {
                printf("Error reading size for malloc.\n");
                continue;
            }
            ptr = my_malloc(size);
            printf("Allocated %zu bytes at address %p\n", size, ptr);
            print_chunk_data();
        } else if (strcmp(command, "free") == 0) {
            unsigned long addr;
            if (scanf("%lx", &addr) != 1) {
                printf("Error reading address for free.\n");
                continue;
            }
            my_free((void *)addr);
            printf("Freed memory at address 0x%lx\n", addr);
            print_chunk_data();
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Unknown command: %s\n", command);
        }
    }
}

void print_chunk_data() {
    MemoryChunkHeader *current = first_chunk;
    int chunk_count = 0;

    while (current != NULL) {
        printf("Chunk Data for chunk id %d:\n", current->id);
        printf("Address: %p\n", (void *)current);
        printf("Chunk total units: %d\n", current->chunk_total_units);
        printf("Chunk available units: %d\n", current->chunk_available_units);
        printf("Is Large Allocation: %d\n", current->is_large_allocation);

        if (current->bitmap) {
            printf("Bitmap Address: %p\nBitmap bits:\n", (void *)current->bitmap);
            print_bitmap(current->bitmap, current->bitmap_size);
        } else {
            printf("This is a large chunk with no bitmap.\n");
        }

        printf("Next Chunk Address: %p\n\n", (void *)current->next);
        current = current->next;
        chunk_count++;
    }
}


/*Como probarlo

Prueba de Asignación Normal:

Escribe malloc <size> donde <size> es el número de bytes que deseas asignar. 
Por ejemplo, para asignar 100 bytes, escribe malloc 100. 
Deberías ver una salida que muestra la dirección de memoria asignada.

Prueba de Large Allocation:

Para probar una asignación de large allocation, 
debes asignar una cantidad de memoria que requiera la creación de un chunk grande. 
podrías probar con malloc 5000.






*/
