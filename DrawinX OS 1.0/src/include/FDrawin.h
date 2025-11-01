#include "vga.h"

#define MAX_FILES 32
#define FILENAME_LENGTH 8

typedef struct __attribute__((packed))
{
    char name[FILENAME_LENGTH];
    uint_32 start_sector;
    uint_32 size; // in bytes
} FileEntry;

typedef struct
{
    FileEntry directory[MAX_FILES];
    uint_8 file_types[MAX_FILES];
    uint_8 file_parent_indices[MAX_FILES];
    int open_files[MAX_FILES];
    uint_8 current_parent_index;
    uint_8 current_own_index;
} FSContext;

static FSContext fs_ctx = {.current_parent_index = 0}; // All data in one place

static uint_8 sector_buffer[SECTOR_SIZE];

void fs_flush_directory()
{
    memset(sector_buffer, 0, SECTOR_SIZE);
    memcpy(sector_buffer, &fs_ctx, sizeof(FSContext));
    device_write_sector(0, sector_buffer);
}

void fs_init()
{
    device_read_sector(0, sector_buffer);
    memcpy(&fs_ctx, sector_buffer, sizeof(FSContext));
}

int find_empty_slot()
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (fs_ctx.directory[i].name[0] == 0)
        {
            return i;
        }
    }
    return -1;
}

int find(const char *filename)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (strncmp(fs_ctx.directory[i].name, filename, FILENAME_LENGTH) == 0)
        {
            return i;
        }
    }
    return -1;
}

int fs_create(const char *filename, uint_8 type)
{
    int slot = find_empty_slot();
    if (slot == -1)
        return -1;
    if (slot == 0)
        slot = 1; // Reserve slot 0 for root directory

    memset(&fs_ctx.directory[slot], 0, sizeof(FileEntry));
    strncpy(fs_ctx.directory[slot].name, filename, FILENAME_LENGTH - 1);
    fs_ctx.directory[slot].name[FILENAME_LENGTH - 1] = '\0';

    fs_ctx.directory[slot].start_sector = 1 + (slot * 10);
    fs_ctx.directory[slot].size = 0;

    fs_ctx.file_parent_indices[slot] = fs_ctx.current_parent_index;
    fs_ctx.file_types[slot] = type;
    fs_ctx.current_own_index = slot;

    fs_flush_directory();
    return 0;
}

int fs_open(const char *filename)
{
    int idx = find(filename);
    if (idx >= 0)
    {
        fs_ctx.open_files[idx] = 1;
    }
    return idx;
}

uint_8 fs_opendir(string name)
{
    int idx = find(name);
    if (idx >= 0)
    {
        fs_ctx.current_parent_index = idx;
    }
    else
    {
        return -1;
    }

    return 0;
}

void fs_close(int file_index)
{
    fs_ctx.open_files[file_index] = 0;
}

int fs_write(int file_index, const uint_8 *data, uint_32 size)
{
    if (!fs_ctx.open_files[file_index])
        return -1;

    if (SECTOR_SIZE == 0)
        return -1;

    uint_32 sectors_needed = (size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    uint_32 start = fs_ctx.directory[file_index].start_sector;

    uint_32 org_size = size;

    for (uint_32 i = 0; i < sectors_needed; i++)
    {
        memset(sector_buffer, 0, SECTOR_SIZE);
        uint_32 copy_size = (size > SECTOR_SIZE) ? SECTOR_SIZE : size;
        memcpy(sector_buffer, data + (i * SECTOR_SIZE), copy_size);
        device_write_sector(start + i, sector_buffer);
        size -= copy_size;
    }

    fs_ctx.directory[file_index].size += org_size;
    fs_flush_directory();
    return 0;
}

int fs_read(int file_index, uint_8 *buffer, uint_32 size)
{
    if (!fs_ctx.open_files[file_index])
        return -1;

    if (SECTOR_SIZE == 0)
        return -1;

    uint_32 sectors_needed = (size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    uint_32 start = fs_ctx.directory[file_index].start_sector;

    for (uint_32 i = 0; i < sectors_needed; i++)
    {
        device_read_sector(start + i, sector_buffer);
        uint_32 copy_size = (size > SECTOR_SIZE) ? SECTOR_SIZE : size;
        memcpy(buffer + (i * SECTOR_SIZE), sector_buffer, copy_size);
        size -= copy_size;
    }
    return 0;
}

// linea: The line from which you want to start printing
char* fs_list()
{
    static char names[FILENAME_LENGTH];  // enough space for many filenames
    int pos = 0;             // position in names array

    for (int i = 0; i < MAX_FILES; i++)
    {
        // Only list files inside the current directory
        if (fs_ctx.file_parent_indices[i] == fs_ctx.current_parent_index)
        {
            // Copy file/directory name
            for (int j = 0; j < FILENAME_LENGTH && fs_ctx.directory[i].name[j] != '\0'; j++)
            {
                names[pos++] = fs_ctx.directory[i].name[j];
            }

            // Add space between entries
            names[pos++] = ' ';
        }
    }

    // Null terminate
    names[pos] = '\0';
    return names;
}