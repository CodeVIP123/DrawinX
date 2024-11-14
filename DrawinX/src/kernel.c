#include "DrX.h"

void kernel_main()
{
    cprintf((string) "Welcome to DrawinX OS!", 0x12);
    uint_16 column = 0, row = 1;
    uint_16 line = row * VGA_WIDTH + column;

    char key;
    while (1)
    {
        changePositionOfCursor((uint_32) line);
        key = checkKeys();
        if (key != 0)
        {
            putc(key, line);
            column++;

            if (column >= VGA_WIDTH)
            {
                column = 0;
                row++;

                if (row >= VGA_HEIGHT)
                    row = 0;
            }

            line = row * VGA_WIDTH + column;
        }
    }
}



