#include "FDrawin.h"


#define PAGE_TABLES ((uint_32*)0x100000)
#define PAGE_DIRECTORY ((uint_32*)0x80000)

#define PAGE_SIZE      0x1000
#define PAGE_FLAGS     3 // Present + RW
#define NUM_PAGE_TABLES 1024
#define ENTRIES_PER_TABLE 1024

void init_paging() {
    // Fill all 1024 page tables
    for (int i = 0; i < NUM_PAGE_TABLES; i++) {
        uint_32* table = (uint_32*)(PAGE_TABLES + (i * ENTRIES_PER_TABLE));

        for (int j = 0; j < ENTRIES_PER_TABLE; j++) {
            uint_32 physical_addr = ((i * ENTRIES_PER_TABLE) + j) * PAGE_SIZE;
            table[j] = physical_addr | PAGE_FLAGS;
        }

        PAGE_DIRECTORY[i] = ((uint_32)table) | PAGE_FLAGS;
    }

    // Load page directory into CR3
    asm volatile("movl %0, %%cr3" : : "r"(PAGE_DIRECTORY));

    // Enable paging
    uint_32 cr0;
    asm volatile("movl %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("movl %0, %%cr0" : : "r"(cr0));
}

