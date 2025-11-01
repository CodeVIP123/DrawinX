#include "DrawATA.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

uint_16 cursor_row;
uint_16 cursor_column;
address_type vga = (address_type)0xB8000;

void changePositionOfCursor(uint_32 position)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint_8)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint_8)((position >> 8) && 0xFF));
}

void putc(char c, uint_16 cursor)
{
    vga[cursor * 2] = c;
    vga[cursor * 2 + 1] = 0x0F;
    changePositionOfCursor(cursor);
}

void cprintf(string content, uint_8 color)
{
    for (uint_8 i = 0; content[i] != '\0'; i++)
    {
        vga[i * 2] = content[i];
        vga[i * 2 + 1] = color;
    }
}

void itoa(int number, char buffer[], int base)
{
    int isNegative = 0;
    int index = 0;
    if (number == 0)
    {
        buffer[index++] = '0';
        buffer[index] = '\0';
    }

    if (number < 0)
    {
        isNegative = 1;
        number = -number;
    }

    while (number > 0)
    {
        int rem = number % base;
        if (base == 10)
            buffer[index++] = rem + '0';
        else if (base == 16)
            buffer[index++] = (rem > 9) ? (rem - 10 + 'A') : (rem + '0');
        number = (int)number / base;
    }

    if (isNegative)
    {
        buffer[index++] = '-';
    }

    int start = 0, end = index - 1;
    while (start <= end)
    {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    buffer[index] = '\0';
}

void clearScreen()
{
    for (uint_16 i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = 0x0F;
    }
}

uint_16 printf(string content, uint_16 row, uint_16 column, ...)
{
    va_list args;
    va_start(args, column);
    uint_16 cursor;
    for (uint_8 i = 0; content[i] != '\0'; i++)
    {
        if (content[i] == '\n')
        {
            row = row + 1;
            column = 0;
        }
    }

    cursor_row = row;
    cursor_column = column;
    cursor = cursor_row * VGA_WIDTH + cursor_column;

    for (uint_8 i = 0; content[i] != '\0'; i++)
    {
        putc(content[i], cursor);
        cursor++;
    }
    va_end(args);
    return cursor;
}

void setRowColumnForDebugPrintf(uint_16 row, uint_16 column)
{
    cursor_row = row;
    cursor_column = column;
}

void resetRowColumnForPrintf()
{
    cursor_row = 0;
    cursor_column = 0;
}

void debugPrintf(string content, ...)
{
    va_list args;
    va_start(args, content);
    uint_16 cur = cursor_row * VGA_WIDTH + cursor_column;
    char buffer_itoa[10];
    int number;
    for (uint_8 i = 1; content[i] != '\0'; i++)
    {
        switch (content[i])
        {
        case 'd':
            number = va_arg(args, int);
            itoa(number, buffer_itoa, 10);
            for (uint_8 j = 0; buffer_itoa[j] != '\0'; j++)
            {
                putc(buffer_itoa[j], cur);
                cur++;
            }
            break;

        case 's':
            string str = va_arg(args, string);
            for (uint_8 j = 0; str[j] != '\0'; j++)
            {
                putc(str[j], cur);
                cur++;
            }
            break;

        case 'c':
            putc(va_arg(args, char), cur);
            cur++;
            break;

        case 'x':
            number = va_arg(args, int);
            itoa(number, buffer_itoa, 16);
            for (uint_8 j = 0; buffer_itoa[j] != '\0'; j++)
            {
                putc(buffer_itoa[j], cur);
                cur++;
            }
            break;
        }
    }
    va_end(args);
    resetRowColumnForPrintf();
}


void greeting()
{
    cprintf((string) "Welcome to DrawinX OS!", 0x12);
}