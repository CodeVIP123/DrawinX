#include "keymaps.h"

#define KEYBOARD_PORT 0x60

#define PIC_EOI 0x20

#define PIC1_COMMAND 0x20

uint_32 prompt_length = 9;
uint_32 line = 2;
uint_32 aLine = 169;
char buffer[256];
uint_8 buffer_index = 0;

void keyboardHandler(InterruptRegisters *regs)
{
    uint_8 sc = ipd(KEYBOARD_PORT);

    if (sc & 0x80)
    {
        outb(PIC1_COMMAND, PIC_EOI);
        return;
    }

    char ascii = scan_to_ascii(sc);

    switch(ascii)
    {
        case '\0':
            break;
        case '\n':
            buffer[buffer_index] = '\0';
            printf("\n", line, 0);
            line++;
            printf(buffer, line, 0);
            line++;
            printf("\n", line, 0);
            line++;
            printf("DrawinX>", line, 0);
            for (int i = 0; i < 256; i++)
            {
                buffer[i] = 0;
            }
            buffer_index = 0;
            aLine = line * VGA_WIDTH + prompt_length;
            changePositionOfCursor(aLine);
            break;
        case '\b':
            break;
        default:
            putc(ascii, aLine);
            buffer[buffer_index++] = ascii;
            aLine++;
            changePositionOfCursor(aLine);
            break;
    }
    outb(PIC1_COMMAND, PIC_EOI);
}