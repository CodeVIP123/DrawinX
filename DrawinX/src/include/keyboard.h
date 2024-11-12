#include "types.h"

extern void keycodePolls();
extern uint_8 keycode;

static char key_map[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8',  // 0-9
        '9', '0', '-', '=', 0, '\t', 'q', 'w', 'e', 'r',  // 10-19
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,  // 20-29
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 30-39
        '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', // 40-49
        'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0,         // 50-59
    };

char keycodeToAscii(uint_8 code)
{
    if (code < 128)
        return key_map[code];
    return -1; // -1 on failure
}

char checkForKeys()
{
    keycodePolls();
    char key = keycodeToAscii(keycode);
    if (key < 0)
        return -1;
    else
        return key;
}