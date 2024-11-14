#include "keyboard.h"
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

void putc(char c, uint_16 cursor)
{
        vga[cursor * 2] = c;
        vga[cursor * 2 + 1] = 0x0F;
}

void changePositionOfCursor(uint_32 position)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint_8) (position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint_8) ((position >> 8) && 0xFF));
}

