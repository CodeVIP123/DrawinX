#include "types.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

address_type vga = (address_type) 0xB8000;

void cprintf(string content, uint_8 color)
{
    for (uint_8 i = 0; content[i] != '\0'; i++)
    {
        vga[i * 2] = content[i];
        vga[i * 2 + 1] = color;
    }
}

void cprintc(char c, uint_8 color)
{
    uint_16 cursor = 0;
    vga[cursor * 2] = c;
    vga[cursor * 2 + 1] = color;
    cursor++;

    if (cursor >= VGA_WIDTH * VGA_HEIGHT)
        cursor = 0;
}