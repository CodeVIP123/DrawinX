#include "types.h"


#define STORAGE_BASE    0x20000000
#define STORAGE_SIZE    0x10000      // 64 KB
#define SECTOR_SIZE     512

#define MMIO_32(addr) (*(volatile uint_32 *)(addr))


void device_read_sector(uint_32 sector_num, uint_8 *buffer) {
    uint_32 addr = STORAGE_BASE + (sector_num * SECTOR_SIZE);
    for (uint_32 i = 0; i < SECTOR_SIZE; i++) {
        buffer[i] = *((volatile uint_8 *)(addr + i));
    }
}

void device_write_sector(uint_32 sector_num, const uint_8 *buffer) {
    uint_32 addr = STORAGE_BASE + (sector_num * SECTOR_SIZE);
    for (uint_32 i = 0; i < SECTOR_SIZE; i++) {
        *((volatile uint_8 *)(addr + i)) = buffer[i];
    }
}

