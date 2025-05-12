#include "DrawATA.h"

#define MAX_FILES          32
#define FILENAME_LENGTH    8

typedef struct {
    char name[FILENAME_LENGTH];
    uint_32 start_sector;
    uint_32 size;    // in bytes
} FileEntry;

static FileEntry directory[MAX_FILES];
static uint_8 sector_buffer[SECTOR_SIZE];
static int open_files[MAX_FILES];

void fs_init()
{
    device_read_sector(0, sector_buffer);
    memcpy(directory, sector_buffer, sizeof(directory));
}

void fs_flush_directory() {
    memcpy(sector_buffer, directory, sizeof(directory));
    device_write_sector(0, sector_buffer);
}


int find_empty_slot() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (directory[i].name[0] == 0) {
            return i;
        }
    }
    return -1;
}

int find_file(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strncmp(directory[i].name, filename, FILENAME_LENGTH) == 0) {
            return i;
        }
    }
    return -1;
}

int fs_create(const char *filename) {
    int slot = find_empty_slot();
    if (slot == -1) return -1;

    memset(&directory[slot], 0, sizeof(FileEntry));
    strncpy(directory[slot].name, filename, FILENAME_LENGTH);
    directory[slot].start_sector = 1 + (slot * 10);  // har file ke liye 10 sectors reserve
    directory[slot].size = 0;

    fs_flush_directory();
}

int fs_open(const char *filename) {
    int idx = find_file(filename);
    if (idx >= 0) {
        open_files[idx] = 1;
    }
    return idx;
}

void fs_close(int file_index) {
    open_files[file_index] = 0;
}

int fs_write(int file_index, const uint_8 *data, uint_32 size) {
    if (!open_files[file_index]) return -1;

    uint_32 sectors_needed = (size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    uint_32 start = directory[file_index].start_sector;
    
    uint_32 org_size = size;

    for (uint_32 i = 0; i < sectors_needed; i++) {
        memset(sector_buffer, 0, SECTOR_SIZE);
        uint_32 copy_size = (size > SECTOR_SIZE) ? SECTOR_SIZE : size;
        memcpy(sector_buffer, data + (i * SECTOR_SIZE), copy_size);
        device_write_sector(start + i, sector_buffer);
        size -= copy_size;
    }

    directory[file_index].size += org_size;
    fs_flush_directory();
    return 0;
}

int fs_read(int file_index, uint_8 *buffer, uint_32 size) {
    if (!open_files[file_index]) return -1;

    uint_32 sectors_needed = (size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    uint_32 start = directory[file_index].start_sector;

    for (uint_32 i = 0; i < sectors_needed; i++) {
        device_read_sector(start + i, sector_buffer);
        uint_32 copy_size = (size > SECTOR_SIZE) ? SECTOR_SIZE : size;
        memcpy(buffer + (i * SECTOR_SIZE), sector_buffer, copy_size);
        size -= copy_size;
    }
    return 0;
}
