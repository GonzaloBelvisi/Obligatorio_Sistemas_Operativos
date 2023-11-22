#include "my_malloc_manager.h"
#include <stdio.h>

void test_first_fit()
{
    unsigned char bitmap[BITMAP_SIZE] = {0};

    // Tamaños de unidades para probar
    size_t test_sizes[] = {5, 10, 3, 200, 80, 7};
    size_t num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    print_bitmap(bitmap, BITMAP_SIZE); // Imprimir el bitmap inicial

    for (size_t i = 0; i < num_tests; ++i)
    {
        size_t units_needed = test_sizes[i];
        int result = first_fit(bitmap, BITMAP_SIZE, units_needed);

        printf("Resultado de first_fit para %zu unidades: %d\n", units_needed, result);
        print_bitmap(bitmap, BITMAP_SIZE); // Imprimir el bitmap después de cada invocación de first_fit
    }
}

void test_my_malloc_free()
{
    // Tamaños de unidades para probar
    size_t test_sizes[] = {5, 10, 100, 200};
    size_t num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
    void *allocated_blocks[num_tests];

    // Probar my_malloc
    for (size_t i = 0; i < num_tests; ++i)
    {
        size_t bytes_needed = test_sizes[i];
        allocated_blocks[i] = my_malloc(bytes_needed);

        printf("Resultado de my_malloc para %zu bytes: %p\n", bytes_needed, allocated_blocks[i]);
        print_bitmap(first_chunk->bitmap, BITMAP_SIZE);
    }

    printf("\n--- Liberando memoria ---\n");

    // Probar my_free
    for (size_t i = 0; i < num_tests; ++i)
    {
        printf("Liberando bloque de %zu bytes: %p\n", test_sizes[i], allocated_blocks[i]);
        my_free(allocated_blocks[i]);
        print_bitmap(first_chunk->bitmap, BITMAP_SIZE);
    }
}

void test_large_allocation()
{
    size_t large_size = 1024; // Cambiado a 1024 bytes

    printf("\nProbando asignación grande de %zu bytes...\n", large_size);

    // Verifica que first_chunk no sea nulo antes de intentar usarlo
    if (!first_chunk)
    {
        printf("Inicializando first_chunk...\n");
        first_chunk = create_new_chunk();
        if (!first_chunk)
        {
            printf("No se pudo inicializar first_chunk.\n\n");
            return;
        }
    }

    void *large_block = my_malloc(large_size);
    if (large_block == NULL)
    {
        printf("No se pudo asignar un bloque grande de memoria.\n\n");
    }
    else
    {
        printf("Bloque grande asignado en: %p\n", large_block);
    }

    // Imprimir el estado del bitmap después de la asignación/intento de asignación
    printf("\nEstado del bitmap después de la asignación/intento de asignación:\n");
    print_bitmap(first_chunk->bitmap, BITMAP_SIZE);

    if (large_block != NULL)
    {
        // Libera el bloque grande si se asignó
        printf("\nLiberando bloque grande...\n");
        my_free(large_block);

        // Imprimir el estado del bitmap después de liberar
        printf("\nEstado del bitmap después de liberar:\n");
        print_bitmap(first_chunk->bitmap, BITMAP_SIZE);
        printf("\n"); // Espacio adicional después de la última salida
    }
}

int main()
{
    int choice;

    printf("\nSeleccione la prueba a ejecutar:\n");
    printf("  1. Test first_fit\n");
    printf("  2. Test my_malloc y my_free\n");
    printf("  3. Test Large Allocation\n\n");
    printf("Ingrese su elección (1, 2 o 3): ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        test_first_fit();
        break;
    case 2:
        test_my_malloc_free();
        break;
    case 3:
        test_large_allocation();
        break;
    default:
        printf("\nOpción no válida.\n");
    }

    return 0;
}