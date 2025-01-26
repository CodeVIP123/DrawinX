#include "DrX.h"

void kernel_main()
{
    loadGdt();
    initIDT();
    setIDTGatesSystemReserved();
    initTimer();
    setIDTKeyboardGate();
    cprintf((string) "Welcome to DrawinX OS!", 0x12);
    printf((string) "DrawinX>", 2, 0);
    while (1);
}

