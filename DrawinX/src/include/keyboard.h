#include "types.h"
#define KEYBOARD_PORT 0x60

uint_8 ipd(uint_16 port)
{
    uint_8 result = 0;
    //     ( "string : in %dx, %al"  : "=a" (result) output "will be stored in al" : "d" (port) input "port in dx")
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void outb(uint_16 port, uint_8 data)
{
    __asm__ volatile("out %0, %1" : : "a" (data), "Nd" (port));
}

char checkKeys()
{
    uint_8 key = ipd(KEYBOARD_PORT);
    switch (key)
    {
        case 0x1E: 
            ipd(KEYBOARD_PORT); 
            return 'A'; 
            break;
        case 0x30: 
            ipd(KEYBOARD_PORT);
            return 'B'; 
            break;
        case 0x2E: 
            ipd(KEYBOARD_PORT);
            return 'C'; 
            break;
        
        case 0x39: 
            ipd(KEYBOARD_PORT);
            return ' '; 
            break;

        default: return 0; break;
    }
}


