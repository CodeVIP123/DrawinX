#include "../types.h"
#define KEYBOARD_PORT 0x60

bool isShift = false;

bool checkShift()
{
    if (isShift == true)
        return true;
    else
        return false;
}

char checkKeys()
{
    uint_8 key = ipd(KEYBOARD_PORT);
    switch (key)
    {
        case 0x2A: 
            key = 0;
            outb(KEYBOARD_PORT, 0);
            isShift = true;
            break;
        case 0x36: 
            key = 0;
            outb(KEYBOARD_PORT, 0);
            isShift = true;
            break;

        case 0x1E: 
            key = 0;
	        outb(KEYBOARD_PORT, 0); 
            if (checkShift() == true)
            {
                isShift = false;
                return 'A';
            }
            else
                return 'a';
            break;
        case 0x30: 
            key = 0;
	        outb(KEYBOARD_PORT, 0);
            if (checkShift() == true)
            {
                isShift = false;
                return 'B';
            }
            else
                return 'b';
            break;
        case 0x2E: 
            key = 0;
	        outb(KEYBOARD_PORT, 0);
            if (checkShift() == true)
            {
                isShift = false;
                return 'C';
            }
            else
                return 'c'; 
            break;
        case 0x39:  
	        key = 0;
            outb(KEYBOARD_PORT, 0);
            return ' '; 
            break;
        case 0x20:
            key = 0;
            outb(KEYBOARD_PORT, 0);
            if (checkShift() == true)
            {
                isShift = false;
                return 'D';
            }
            else
                return 'd';
            break;
        case 0x12:
            key = 0;
            outb(KEYBOARD_PORT, 0);
            if (checkShift() == true)
            {
                isShift = false;
                return 'E';
            }
            else
                return 'e';
            break;
        
        default: return 0; break;
    }
}

