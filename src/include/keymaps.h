#include "gdt.h"

char keymap[128] = {
    0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x00 - 0x0F
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     // 0x10 - 0x1D
    0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',         // 0x1E - 0x29
    0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,           // 0x2A - 0x35
    '*',  0,    ' ', 0                                                        // 0x36 - 0x39 (shortened)
};

char scan_to_ascii(uint_8 scancode)
{
    if (scancode < sizeof(keymap))
        return keymap[scancode];
    else
        return '\0';
}
