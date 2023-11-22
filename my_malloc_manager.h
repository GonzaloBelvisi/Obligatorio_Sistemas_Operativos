#ifndef MY_ALLOC_MANAGER_H // prevent multiple includes
#define MY_ALLOC_MANAGER_H 1

#include <stddef.h> // for size_t
#include <stdio.h>  // for I/O functions
#include <stdint.h> // for uint16_t and other fixed width integers

#define BITMAP_SIZE 16 // in bytes, already defined

// New constants
#define UNIT_SIZE 16                                                              // minimum unit to assign, in bytes
#define UNITS_PER_CHUNK (BITMAP_SIZE * 8)                                         // assuming 1 bit per unit
#define STRUCT_CH_UNITS ((sizeof(MemoryChunkHeader) + UNIT_SIZE - 1) / UNIT_SIZE) // Cuantas unidades ocupa el chunk header
#define BITMAP_UNITS ((BITMAP_SIZE + UNIT_SIZE - 1) / UNIT_SIZE)                  // Cuantas unidades ocupa el Bitmap

typedef unsigned char *Bitmap; // Bitmap type definition

// Memory management structures
typedef struct MemoryChunkHeader
{
    uint16_t id;                    // id of the chunk, useful for debugging
    uint16_t is_large_allocation;   // Flag to indicate if this is for a single large allocation
    uint16_t chunk_total_units;     // Size of the memory block in units
    uint16_t chunk_available_units; // How many units are available
    Bitmap bitmap;                  // Pointer to the bitmap for this chunk, NULL if is_large_allocation
    uint16_t bitmap_size;           // Size of bitmap in bytes, 0 if is_large_allocation
    struct MemoryChunkHeader *next; // Pointer to the next MemoryChunkHeader, NULL if last one
} MemoryChunkHeader;

typedef struct AllocationHeader
{
    uint16_t nunits;    // number of units
    uint16_t bit_index; // offset from the MemoryChunkHeader struct
} AllocationHeader;

// Existing function declarations
extern int first_fit(Bitmap bitmap, size_t bitmap_size, size_t units_needed);
extern void print_bitmap(Bitmap bitmap, size_t bitmap_size);

// New function declarations
extern void clear_bits(Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty);
extern void set_or_clear_bits(int set, Bitmap bitmap, uint16_t start_byte_index, uint16_t start_bit_index, uint16_t qty);

// Declaration for create_new_chunk
extern void *create_new_chunk(void);

// External variable declarations
extern MemoryChunkHeader *first_chunk; // pointer to the first chunk of memory
extern uint16_t chunk_current_id;      // External variable, to be defined elsewhere
extern void my_free(void *ptr);
extern uint16_t chunk_current_id; // current chunk ID
extern void *my_malloc(size_t nbytes);
#endif
