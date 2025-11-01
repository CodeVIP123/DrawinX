#include "paging.h"

#define HEAP_START 0x800000 // 8MB se heap start
#define HEAP_SIZE 0x100000 // 1MB heap size

typedef struct HeapBlock
{
    uint_32 size; // Size of the block
    uint_8 used; // 0 if free, 1 if used
    struct HeapBlock* next; // Pointer to the next block
} HeapBlock;

static HeapBlock* heap_start = (HeapBlock*)HEAP_START;
static int heap_initialized = 0;

void* malloc(uint_32 size) {
    if (!heap_initialized) {
        heap_start->size = HEAP_SIZE - sizeof(HeapBlock);
        heap_start->used = 0;
        heap_start->next = 0;
        heap_initialized = 1;
    }

    HeapBlock* current = heap_start;
    while (current) {
        if (!current->used && current->size >= size) {
            if (current->size > size + sizeof(HeapBlock)) {
                // Split
                HeapBlock* new_block = (HeapBlock*)((uint_8*)current + sizeof(HeapBlock) + size);
                new_block->size = current->size - size - sizeof(HeapBlock);
                new_block->used = 0;
                new_block->next = current->next;

                current->next = new_block;
                current->size = size;
            }

            current->used = 1;
            return (void*)((uint_8*)current + sizeof(HeapBlock));
        }
        current = current->next;
    }

    return 0;  // No space
}

void free(void* ptr) {
    if (!ptr) return;

    HeapBlock* block = (HeapBlock*)((uint_8*)ptr - sizeof(HeapBlock));
    block->used = 0;

    // Merge with next if it's free
    if (block->next && block->next->used == 0) {
        block->size += sizeof(HeapBlock) + block->next->size;
        block->next = block->next->next;
    }
}

void* calloc(uint_32 n, uint_32 size) {
    void* ptr = malloc(n * size);
    if (!ptr) return 0;
    
    uint_8* p = (uint_8*)ptr;
    for (uint_32 i = 0; i < n * size; i++) p[i] = 0;

    return ptr;
}

