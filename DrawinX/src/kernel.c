#include "DrX.h"

void kernel_main()
{
    cprintf((string) "Welcome to DrawinX OS!", 0x12);

    while (1)
    {
        char Key = checkForKeys();
        if (Key >= 0)
            cprintc(Key, 0x0F);
    }
}

