#include "keyboard.h"

uint_16 calculate_position_from_x_y_coords(uint_16 x, uint_16 y)
{
    return y * VGA_WIDTH + x;
}

void showSplashScreen() {
    // Clear screen to WHITE background
    for (int i = 0; i < 80 * 25; i++) {
        vga[i * 2] = ' ';       // Empty space
        vga[i * 2 + 1] = 0xF0;
    }

    int lowerStartY = 6;  
    for (int i = 0; i < 9; i++) {  // 9 rows for lower legs
        int posLeft = calculate_position_from_x_y_coords(35 + i, lowerStartY + i);
        int posRight = calculate_position_from_x_y_coords(44 - i, lowerStartY + i);

        vga[posLeft * 2] = ' ';
        vga[posLeft * 2 + 1] = 0x0F;

        vga[posRight * 2] = ' ';
        vga[posRight * 2 + 1] = 0x0F;
    }

    // Draw "DrawinX OS" text
    char* text = "  DrawinX OS  ";  
    int textRow = 18;
    int textCol = (80 - 14) / 2;
    int textPos = calculate_position_from_x_y_coords(textCol, textRow);

    for (int i = 0; text[i] != '\0'; i++) {
        vga[(textPos + i) * 2] = text[i];
        vga[(textPos + i) * 2 + 1] = 0x0F;
    }
    for (volatile int i = 0; i < 100000000; i++);
}
